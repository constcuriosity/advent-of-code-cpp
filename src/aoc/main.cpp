
//-------------- headers

#include "aoc/infrastructure/aoc_machine.h"

#include "foundation/utilities/string_utilities.h"
#include "foundation/containers/view/array_view.h"

//-------------- data

extern const s_aoc_exercise_data* k_2015_exercises[];
extern const s_aoc_exercise_data* k_2016_exercises[];
extern const s_aoc_exercise_data* k_2023_exercises[];
extern const uint32 k_2015_exercise_count;
extern const uint32 k_2016_exercise_count;
extern const uint32 k_2023_exercise_count;

const s_aoc_year_exercises exercises[] =
{
	{ 2015, c_exercise_view(k_2015_exercises, static_cast<int32>(k_2015_exercise_count)) },
	{ 2016, c_exercise_view(k_2016_exercises, static_cast<int32>(k_2016_exercise_count)) },
	{ 2023, c_exercise_view(k_2023_exercises, static_cast<int32>(k_2023_exercise_count)) },
};

//-------------- public implementations

int main(int argc, const char *argv[])
{
	int32 inputted_year = NONE;
	int32 inputted_day = NONE;

	if (argc >= 2)
	{
		inputted_year = static_cast<int32>(c_string_utilities::to_integer(argv[1]));
	}

	if (argc >= 3)
	{
		inputted_day = static_cast<int32>(c_string_utilities::to_integer(argv[2]));
	}

	c_aoc_machine machine;
	machine.initialize(inputted_day, inputted_year, c_aoc_year_exercises_view(exercises));
	machine.run();

	return 0;
}
