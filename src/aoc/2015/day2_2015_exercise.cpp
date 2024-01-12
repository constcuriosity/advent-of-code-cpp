
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/math/int_math.h"

#include <iostream>

//-------------- prototypes

void execute_2015_day2_part1(FILE* file);
void execute_2015_day2_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2015_day2_exercise
{
	2015,
	2,
	"I Was Told There Would Be No Math",
	execute_2015_day2_part1,
	execute_2015_day2_part2
};

//-------------- definitions

void execute_2015_day2_part1(FILE* file)
{
	int32 total_sqft = 0;
	int32 current_length = 0;
	int32 current_width = 0;
	int32 current_height = 0;

	while (feof(file) == 0 &&
		fscanf(file, "%dx%dx%d", &current_length, &current_width, &current_height) != EOF)
	{
		int32 side_lw = current_length * current_width;
		int32 side_lh = current_length * current_height;
		int32 side_wh = current_width * current_height;

		int32 area = (2 * side_lw) + (2 * side_lh) + (2 * side_wh);
		int32 smallest_side = int_min(side_lw, int_min(side_lh, side_wh));

		total_sqft += area + smallest_side;
	}

	std::cout << "The elves need " << total_sqft << " sqft of wrapping paper" << std::endl;
}

void execute_2015_day2_part2(FILE* file)
{
	int32 total_feet = 0;
	int32 current_length = 0;
	int32 current_width = 0;
	int32 current_height = 0;

	while (feof(file) == 0 &&
		fscanf(file, "%dx%dx%d", &current_length, &current_width, &current_height) != EOF)
	{
		int32 perimeter = current_length + current_width + current_height;
		perimeter -= int_max(current_length, int_max(current_width, current_height));
		perimeter *= 2;

		int32 volume = current_length * current_width * current_height;

		total_feet += perimeter + volume;
	}

	std::cout << "The elves need " << total_feet << " feet of wrapping ribbon" << std::endl;
}
