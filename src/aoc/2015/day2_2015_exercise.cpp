
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/io/log.h"
#include "foundation/math/basic_math.h"

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
		fscanf_s(file, "%dx%dx%d", &current_length, &current_width, &current_height) != EOF)
	{
		int32 side_lw = current_length * current_width;
		int32 side_lh = current_length * current_height;
		int32 side_wh = current_width * current_height;

		int32 area = (2 * side_lw) + (2 * side_lh) + (2 * side_wh);
		int32 smallest_side = n_math::min(side_lw, n_math::min(side_lh, side_wh));

		total_sqft += area + smallest_side;
	}

	n_log::output("The elves need {} sqft of wrapping paper\n", total_sqft);
}

void execute_2015_day2_part2(FILE* file)
{
	int32 total_feet = 0;
	int32 current_length = 0;
	int32 current_width = 0;
	int32 current_height = 0;

	while (feof(file) == 0 &&
		fscanf_s(file, "%dx%dx%d", &current_length, &current_width, &current_height) != EOF)
	{
		int32 perimeter = current_length + current_width + current_height;
		perimeter -= n_math::max(current_length, n_math::max(current_width, current_height));
		perimeter *= 2;

		int32 volume = current_length * current_width * current_height;

		total_feet += perimeter + volume;
	}

	n_log::output("The elves need {} feet of wrapping ribbon\n", total_feet);
}
