
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/dynamic/dynamic_vector.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/io/log.h"
#include "foundation/utilities/md5_hash.h"

//-------------- constants

static_const int32 k_target_nth_key= 64;

//-------------- prototypes

struct s_triple_record
{
	s_triple_record(int32 index, char character) : index(index), character(character), is_key(false) {}

	int32 index;
	char character;
	bool is_key;
};

typedef c_dynamic_vector<s_triple_record> c_triple_record_vector;

void analyze_hash(const s_md5_digest& hash, int32 iteration_index, c_triple_record_vector* inout_record_vector, int32* key_count);

void execute_2016_day14_part1(FILE* file);
void execute_2016_day14_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day14_exercise
{
	2016,
	14,
	"One-Time Pad",
	execute_2016_day14_part1,
	execute_2016_day14_part2
};

//-------------- definitions

void execute_2016_day14_part1(FILE* file)
{
	c_small_string salt_input;
	fgets(salt_input.get_string(), c_kilo_string::k_capacity, file);

	int32 iteration_index = 0;
	int32 key_count = 0;
	int32 iteration_buffer = NONE;

	c_triple_record_vector triple_records(10000);

	// After hitting k_target_nth_key keys, we must at least iterate 1000 more times in case that causes
	// an earlier triplet to become a valid key
	for (; key_count < k_target_nth_key || iteration_buffer != 0; iteration_index++)
	{
		c_small_string key;
		key.print("%s%d", salt_input.get_string(), iteration_index);

		s_md5_digest current_hash = c_md5_hash::hash(key.get_string());

		analyze_hash(current_hash, iteration_index, &triple_records, &key_count);

		if (key_count >= k_target_nth_key && iteration_buffer == NONE)
		{
			iteration_buffer = 1000;
		}
		else if (iteration_buffer > 0)
		{
			iteration_buffer--;
		}
	}

	int32 index_of_last_key = NONE;
	int32 skip_count = key_count - k_target_nth_key;

	for (int32 triple_index = triple_records.get_size() - 1; triple_index >= 0; triple_index--)
	{
		if (triple_records[triple_index].is_key)
		{
			if (skip_count > 0)
			{
				skip_count--;
			}
			else
			{
				index_of_last_key = triple_records[triple_index].index;
				break;
			}
		}
	}

	n_log::output("The 64th key was produced at index {}\n", index_of_last_key);
}

void execute_2016_day14_part2(FILE* file)
{
	c_small_string salt_input;
	fgets(salt_input.get_string(), c_kilo_string::k_capacity, file);

	int32 iteration_index = 0;
	int32 key_count = 0;
	int32 iteration_buffer = NONE;

	c_triple_record_vector triple_records(10000);

	// After hitting k_target_nth_key keys, we must at least iterate 1000 more times in case that causes
	// an earlier triplet to become a valid key
	for (; key_count < k_target_nth_key || iteration_buffer != 0; iteration_index++)
	{
		c_small_string key;
		key.print("%s%d", salt_input.get_string(), iteration_index);
		s_md5_digest current_hash;

		for (int32 hash_index = 0; hash_index < 2017; hash_index++)
		{
			current_hash = c_md5_hash::hash(key.get_string());
			n_string::print_bytes(current_hash.digest, s_md5_digest::k_digest_size, key.get_string(), key.k_capacity);
		}

		analyze_hash(current_hash, iteration_index, &triple_records, &key_count);

		if (key_count >= k_target_nth_key && iteration_buffer == NONE)
		{
			iteration_buffer = 1000;
		}
		else if (iteration_buffer > 0)
		{
			iteration_buffer--;
		}
	}

	int32 index_of_last_key = NONE;
	int32 skip_count = key_count - k_target_nth_key;

	for (int32 triple_index = triple_records.get_size() - 1; triple_index >= 0; triple_index--)
	{
		if (triple_records[triple_index].is_key)
		{
			if (skip_count > 0)
			{
				skip_count--;
			}
			else
			{
				index_of_last_key = triple_records[triple_index].index;
				break;
			}
		}
	}

	n_log::output("The 64th stretched key was produced at index {}\n", index_of_last_key);
}

void analyze_hash(const s_md5_digest& hash, int32 iteration_index, c_triple_record_vector* inout_record_vector, int32* key_count)
{
	int32 streak_count = 0;
	char previous_char = '\0';
	bool found_triple = false;
	for (int32 byte_index = 0; byte_index < s_md5_digest::k_digest_size; byte_index++)
	{
		byte current_byte = hash.digest[byte_index];
		char left_char = current_byte >> 4;
		char right_char = 0x0F & current_byte;

		streak_count = left_char == previous_char ? streak_count + 1 : 1;
		previous_char = left_char;

		if (streak_count == 3 && !found_triple)
		{
			inout_record_vector->emplace_back(iteration_index, previous_char);
			found_triple = true;
		}
		else if (streak_count == 5)
		{
			c_triple_record_vector::reverse_iterator record_iter = inout_record_vector->rbegin();
			c_triple_record_vector::reverse_iterator last_record = inout_record_vector->rend();

			for (; record_iter != last_record; record_iter++)
			{
				if (record_iter->index <= (iteration_index - 1000))
				{
					break;
				}
				else if (record_iter->index != iteration_index &&
					record_iter->character == previous_char &&
					!record_iter->is_key)
				{
					record_iter->is_key = true;
					(*key_count)++;
				}
			}
		}

		streak_count = right_char == previous_char ? streak_count + 1 : 1;
		previous_char = right_char;

		if (streak_count == 3 && !found_triple)
		{
			inout_record_vector->emplace_back(iteration_index, previous_char);
			found_triple = true;
		}
		else if (streak_count == 5)
		{
			c_triple_record_vector::reverse_iterator record_iter = inout_record_vector->rbegin();
			c_triple_record_vector::reverse_iterator last_record = inout_record_vector->rend();

			for (; record_iter != last_record; record_iter++)
			{
				if (record_iter->index <= (iteration_index - 1000))
				{
					break;
				}
				else if (record_iter->index != iteration_index &&
					record_iter->character == previous_char &&
					!record_iter->is_key)
				{
					record_iter->is_key = true;
					(*key_count)++;
				}
			}
		}
	}
}
