
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/dynamic/dynamic_vector.h"
#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/containers/view/string_view.h"
#include "foundation/utilities/bit_utilities.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

void execute_2016_day13_part1(FILE* file);
void execute_2016_day13_part2(FILE* file);

//-------------- globals

static_function bool is_wall(int64 x, int64 y, int64 seed);

extern const s_aoc_exercise_data k_2016_day13_exercise
{
	2016,
	13,
	"A Maze of Twisty Little Cubicles",
	execute_2016_day13_part1,
	execute_2016_day13_part2
};

//-------------- definitions

void execute_2016_day13_part1(FILE* file)
{
	c_kilo_string line_buffer;
	fgets(line_buffer.get_string(), c_kilo_string::k_capacity, file);

	int64 office_designer_favorite_number = n_string::to_integer(line_buffer.get_string());

	std::cout << "Day 13 part 1" << std::endl;
}

void execute_2016_day13_part2(FILE* file)
{
	std::cout << "Day 13 part 2" << std::endl;
}

static_function bool is_wall(int64 x, int64 y, int64 seed)
{
	bool is_wall = x < 0 || y < 0;

	if (!is_wall)
	{
		int64 sum = x*x + 3*x + 2*x*y + y + y*y + seed;

		is_wall = n_bit_utilities::count(reinterpret_cast<byte*>(&sum), sizeof(int64)) % 2 != 0;
	}

	return is_wall;
}
