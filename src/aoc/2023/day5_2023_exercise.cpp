
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/dynamic/dynamic_vector.h"
#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_set.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/math/basic_math.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

void execute_2023_day5_part1(FILE* file);
void execute_2023_day5_part2(FILE* file);

struct s_transform_range
{
	union
	{
		struct
		{
			uint64 destination;
			uint64 source;
			uint64 range;
		};

		uint64 values[3];
	};
};

class c_transform_comparator
{
public:
	bool is_ordered(const s_transform_range& left, const s_transform_range& right) const
	{
		return left.source < right.source; // Can't be <=, or else the quicksort algo fails
	}

	bool equals(const s_transform_range& left, const s_transform_range& right) const
	{
		return left.source == right.source &&
			left.destination == right.destination &&
			left.range == right.range;
	}

	bool operator()(const s_transform_range& left, const s_transform_range& right) const
	{
		return is_ordered(left, right);
	}
};

struct s_seed_range
{
	uint64 start;
	uint64 size;
};

class c_seed_comparator
{
public:
	bool is_ordered(const s_seed_range& left, const s_seed_range& right) const
	{
		return left.start < right.start; // Can't be <=, or else the quicksort algo fails
	}

	bool equals(const s_seed_range& left, const s_seed_range& right) const
	{
		return left.start == right.start &&
			left.size == right.size;
	}

	bool operator()(const s_seed_range& left, const s_seed_range& right) const
	{
		return is_ordered(left, right);
	}
};

//-------------- globals

extern const s_aoc_exercise_data k_2023_day5_exercise
{
	2023,
	5,
	"If You Give A Seed A Fertilizer",
	execute_2023_day5_part1,
	execute_2023_day5_part2
};

//-------------- definitions

void execute_2023_day5_part1(FILE* file)
{
	c_medium_string line_buffer;

	// Gather seeds
	fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

	c_dynamic_vector<uint64> seed_vector;
	c_utf8_tokenizer seed_tokenizer(line_buffer.get_string(), " ");
	seed_tokenizer.advance(); // Skip past seeds

	do
	{
		uint64 seed = n_string::to_unsigned_integer(seed_tokenizer.get_token().string, seed_tokenizer.get_token().length);
		seed_vector.push(seed);
	} while (seed_tokenizer.advance());

	// Read a blank line
	fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

	while (feof(file) == 0)
	{
		// Read a transform name... and ditch it
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file); // Read the first map line

		// Generate the current map
		c_dynamic_vector<s_transform_range> value_map;
		while (line_buffer.length() > 4 && feof(file) == 0) // Super jank way to tell if not yet at a blank line
		{
			c_utf8_tokenizer map_tokenizer(line_buffer.get_string(), " ");
			s_transform_range range;
			uint64 value_index = 0;

			do
			{
				uint64 value = n_string::to_unsigned_integer(map_tokenizer.get_token().string, map_tokenizer.get_token().length);
				range.values[value_index] = value;
				value_index++;

			} while (map_tokenizer.advance());

			value_map.push(range);

			// Get the next line
			fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);
		}

		quick_sort(value_map.get_storage(), value_map.get_size(), c_transform_comparator());

		for (int32 seed_index = 0; seed_index < seed_vector.get_size(); seed_index++)
		{
			uint64 current_seed = seed_vector[seed_index];

			int32 best_range_index = NONE;
			for (int32 value_index = 0; value_index < value_map.get_size(); value_index++)
			{
				if (value_map[value_index].source > current_seed)
				{
					best_range_index = value_index - 1;
					break;
				}

				if (value_index == value_map.get_size() - 1)
				{
					best_range_index = value_index;
				}
			}

			if (best_range_index == NONE)
			{
				// Leave seed value alone
			}
			else
			{
				const s_transform_range& range = value_map[best_range_index];

				if ((range.source + range.range) <= current_seed)
				{
					// Still leave the seed alone
				}
				else
				{
					seed_vector[seed_index] = range.destination + (current_seed - range.source);
				}
			}
		}
	}

	uint64 lowest_location = k_uint64_max;

	for (int32 seed_index = 0; seed_index < seed_vector.get_size(); seed_index++)
	{
		lowest_location = n_math::min(lowest_location, seed_vector[seed_index]);
	}

	std::cout << "The lowest location after all those locations is: " << lowest_location << std::endl;
}

void execute_2023_day5_part2(FILE* file)
{
	c_medium_string line_buffer;

	// Gather seeds
	fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

	c_dynamic_vector<s_seed_range> seed_vector;
	c_utf8_tokenizer seed_tokenizer(line_buffer.get_string(), " ");
	seed_tokenizer.advance(); // Skip past seeds

	do
	{
		uint64 seed_start = n_string::to_unsigned_integer(seed_tokenizer.get_token().string, seed_tokenizer.get_token().length);
		seed_tokenizer.advance();
		uint64 seed_length = n_string::to_unsigned_integer(seed_tokenizer.get_token().string, seed_tokenizer.get_token().length);

		s_seed_range range;
		range.start = seed_start;
		range.size = seed_length;

		seed_vector.push(range);
	} while (seed_tokenizer.advance());

	// Read a blank line
	fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

	while (feof(file) == 0)
	{
		// Read a transform name... and ditch it
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file); // Read the first map line

		// Generate the current map
		c_dynamic_vector<s_transform_range> value_map;
		while (line_buffer.length() > 4 && feof(file) == 0) // Super jank way to tell if not yet at a blank line
		{
			c_utf8_tokenizer map_tokenizer(line_buffer.get_string(), " ");
			s_transform_range range;
			uint64 value_index = 0;

			do
			{
				uint64 value = n_string::to_unsigned_integer(map_tokenizer.get_token().string, map_tokenizer.get_token().length);
				range.values[value_index] = value;
				value_index++;

			} while (map_tokenizer.advance());

			value_map.push(range);

			// Get the next line
			fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);
		}

		quick_sort(value_map.get_storage(), value_map.get_size(), c_transform_comparator());

		c_dynamic_vector<s_seed_range> local_seeds = seed_vector;
		seed_vector.clear();

		while (!local_seeds.is_empty())
		{
			s_seed_range seed_range = *local_seeds.rbegin();

			int32 best_value_index = NONE;
			for (int32 value_index = 0; value_index < value_map.get_size(); value_index++)
			{
				if (value_map[value_index].source > seed_range.start)
				{
					best_value_index = value_index - 1;
					break;
				}

				if (value_index == value_map.get_size() - 1)
				{
					best_value_index = value_index;
				}
			}

			if (best_value_index == NONE)
			{
				const s_transform_range& above_range = value_map[0];

				// Handle being completely under the lowest range
				if (seed_range.start + seed_range.size <= above_range.source)
				{
					seed_vector.push(seed_range);
					local_seeds.pop();
				}
				// Handle being split across the below and first range
				else
				{
					// Split the range, push one to the finished vector, and the other for consideration.
					s_seed_range front_range;
					front_range.start = seed_range.start;
					front_range.size = above_range.source - seed_range.start;
					seed_vector.push(front_range);

					c_dynamic_vector<s_seed_range>::reverse_iterator back_range = local_seeds.rbegin();
					(*back_range).start = above_range.source;
					(*back_range).size = seed_range.size - (above_range.source - seed_range.start);
				}
			}
			else if (best_value_index == value_map.get_size() - 1)
			{
				const s_transform_range& current_range = value_map[best_value_index];

				// Check if we're beyond the max range
				if ((current_range.source + current_range.range) <= seed_range.start)
				{
					seed_vector.push(seed_range);
					local_seeds.pop();
				}
				// Check if we're fully contained within this range
				else if (seed_range.start + seed_range.size <= current_range.source + current_range.range)
				{
					s_seed_range range;
					range.start = current_range.destination + (seed_range.start - current_range.source);
					range.size = seed_range.size;
					seed_vector.push(range);
					local_seeds.pop();
				}
				// Handle being split across this range and max range
				else
				{
					// Split the range, push one to the finished vector, and the other for consideration.
					s_seed_range front_range;
					front_range.start = current_range.destination + (seed_range.start - current_range.source);
					front_range.size = current_range.range - (seed_range.start - current_range.source);
					seed_vector.push(front_range);

					s_seed_range back_range;
					front_range.start = current_range.source + current_range.range;
					front_range.size = (seed_range.start + seed_range.size) - (current_range.source + current_range.range);
					seed_vector.push(back_range);

					local_seeds.pop();
				}
			}
			else
			{
				const s_transform_range& current_range = value_map[best_value_index];
				const s_transform_range& above_range = value_map[best_value_index + 1];

				// Check if we're completely between the current_range and the above_range
				if (current_range.source + current_range.range <= seed_range.start &&
					seed_range.start + seed_range.size <= above_range.source)
				{
					seed_vector.push(seed_range);
					local_seeds.pop();
				}
				// Check if we're fully contained within this range
				else if (seed_range.start + seed_range.size <= current_range.source + current_range.range)
				{
					s_seed_range range;
					range.start = current_range.destination + (seed_range.start - current_range.source);
					range.size = seed_range.size;
					seed_vector.push(range);
					local_seeds.pop();
				}
				// Check if we're split across current_range
				else if (seed_range.start < current_range.source + current_range.range)
				{
					s_seed_range front_range;
					front_range.start = current_range.destination + (seed_range.start - current_range.source);
					front_range.size = current_range.range - (seed_range.start - current_range.source);
					seed_vector.push(front_range);

					c_dynamic_vector<s_seed_range>::reverse_iterator back_range = local_seeds.rbegin();
					(*back_range).start = current_range.source + current_range.range;
					(*back_range).size = (seed_range.start + seed_range.size) - (current_range.source + current_range.range);
				}
				// Check if we're split across above_range
				else if (above_range.source < seed_range.start + seed_range.size)
				{
					s_seed_range front_range;
					front_range.start = seed_range.start;
					front_range.size = above_range.source - seed_range.start;
					seed_vector.push(front_range);

					c_dynamic_vector<s_seed_range>::reverse_iterator back_range = local_seeds.rbegin();
					(*back_range).start = above_range.source;
					(*back_range).size = (seed_range.start + seed_range.size) - above_range.source;
				}
				else
				{
					// We shouldn't get here at all. If we did, bad things happened.
				}
			}
		}

		// Sort the ranges by start, then merge overlapping ranges.
		quick_sort(seed_vector.get_storage(), seed_vector.get_size(), c_seed_comparator());

		c_dynamic_vector<s_seed_range>::reverse_iterator seed_iter = seed_vector.rbegin();
		c_dynamic_vector<s_seed_range>::reverse_iterator next_iter = seed_iter;
		next_iter++;
		c_dynamic_vector<s_seed_range>::reverse_iterator last_seed = seed_vector.rend();

		while (next_iter != last_seed)
		{
			if ((*seed_iter).start <= (*next_iter).start + (*next_iter).size)
			{
				(*next_iter).size = ((*seed_iter).start + (*seed_iter).size) - (*next_iter).start;
				seed_vector.erase(seed_iter.to_forward());
			}

			seed_iter++;
			next_iter++;
		}
	}

	uint64 lowest_location = k_uint64_max;

	for (int32 seed_index = 0; seed_index < seed_vector.get_size(); seed_index++)
	{
		lowest_location = n_math::min(lowest_location, seed_vector[seed_index].start);
	}

	std::cout << "The lowest location after all those locations is: " << lowest_location << std::endl;
}
