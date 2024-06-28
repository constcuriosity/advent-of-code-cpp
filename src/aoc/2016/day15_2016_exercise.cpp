
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_array.h"
#include "foundation/containers/dynamic/dynamic_vector.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/io/log.h"
#include "foundation/utilities/sort.h"

//-------------- constants

//-------------- prototypes

void execute_2016_day15_part1(FILE* file);
void execute_2016_day15_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day15_exercise
{
	2016,
	15,
	"Timing is Everything",
	execute_2016_day15_part1,
	execute_2016_day15_part2
};

//-------------- definitions

struct s_disc
{
	int32 index;
	int32 radius;
	int32 start;

	// You can calculate the position of a disc with `position = (time + start) % radius`.
	// The disc is aligned if it is at position 0 when the capsule gets to it at time + index.
	// This leads to the equation `0 = (time + start + index) & radius` to test whether a particular time 
	// is aligned or not.
	bool aligned(int32 time) const { return ((time + index + start) % radius) == 0; }

	// There is always some time t in the future that the disc first reaches its 0 position.
	// That's time = radius - start % radius.
	// But since we care about the first time when the disc is 0 when the capsule gets there, we actually want
	// need to offset it evern further with time = (radius - start - index) % radius
	int32 first_time() const { return (radius - start - index) % radius; }
};

class c_disc_size_comparator
{
public:
	bool is_ordered(const s_disc& left, const s_disc& right) const
	{
		return left.radius > right.radius; // Can't be <=, or else the quicksort algo fails
	}

	bool equals(const s_disc& left, const s_disc& right) const
	{
		return left.index == right.index;
	}

	bool operator()(const s_disc& left, const s_disc& right) const
	{
		return is_ordered(left, right);
	}
};

void execute_2016_day15_part1(FILE* file)
{
	c_dynamic_vector<s_disc> discs(10);

	c_medium_string line_buffer;
	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);
		c_utf8_tokenizer tokens(line_buffer.get_string(), " .#\n");

		s_disc disc;
		tokens.advance(1);
		disc.index = static_cast<int32>(n_string::to_integer(tokens.get_token()));
		tokens.advance(2);
		disc.radius = static_cast<int32>(n_string::to_integer(tokens.get_token()));
		tokens.advance(8);
		disc.start = static_cast<int32>(n_string::to_integer(tokens.get_token()));

		discs.push(disc);
	}

	quick_sort(discs.begin(), discs.get_size(), c_disc_size_comparator());

	// Initialize time to be the first time that the largest disc would pass through.
	int32 time = discs[0].first_time();

	bool found_time = false;
	while (!found_time)
	{
		time += discs[0].radius;

		found_time = discs.every([time](const s_disc& disc) {
			return disc.aligned(time);
		});
	}

	n_log::output("The first time a capsule can be retrieved is {}\n", time);
}

void execute_2016_day15_part2(FILE* file)
{
	c_dynamic_vector<s_disc> discs(10);

	c_medium_string line_buffer;
	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);
		c_utf8_tokenizer tokens(line_buffer.get_string(), " .#\n");

		s_disc disc;
		tokens.advance(1);
		disc.index = static_cast<int32>(n_string::to_integer(tokens.get_token()));
		tokens.advance(2);
		disc.radius = static_cast<int32>(n_string::to_integer(tokens.get_token()));
		tokens.advance(8);
		disc.start = static_cast<int32>(n_string::to_integer(tokens.get_token()));

		discs.push(disc);
	}

	// Extra disc for day 2
	discs.push({ 7, 11, 0 });

	quick_sort(discs.begin(), discs.get_size(), c_disc_size_comparator());

	// Initialize time to be the first time that the largest disc would pass through
	int32 time = discs[0].first_time();

	bool found_time = false;
	while (!found_time)
	{
		time += discs[0].radius;

		found_time = discs.every([time](const s_disc& disc) {
			return disc.aligned(time);
		});
	}

	n_log::output("With an extra disc the next time is {}\n", time);
}
