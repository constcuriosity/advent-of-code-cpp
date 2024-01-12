
#include "aoc/aoc_machine.h"

#include "foundation/utilities/string_utilities.h"
#include "foundation/containers/view/array_view.h"

extern s_aoc_exercise_data day1;

const s_aoc_exercise_data* exercises[] =
{
	&day1,
};

int main(int argc, const char *argv[])
{
	int32 inputted_day = 0;
	if (argc >= 2)
	{
		inputted_day = static_cast<int32>(c_string_utilities::to_integer(argv[1]));
	}

	c_aoc_machine machine;
	machine.initialize(inputted_day, 2024, c_exercise_view(exercises));
	machine.run();

	return 0;
}
