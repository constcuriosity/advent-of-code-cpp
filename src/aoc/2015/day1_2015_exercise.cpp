
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/io/log.h"

//-------------- constants

static_const int32 k_line_size = k_kilobyte * 128;

//-------------- prototypes

void execute_2015_day1_part1(FILE* file);
void execute_2015_day1_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2015_day1_exercise
{
	2015,
	1,
	"Not Quite Lisp",
	execute_2015_day1_part1,
	execute_2015_day1_part2
};

//-------------- definitions

void execute_2015_day1_part1(FILE* file)
{
	// $TODO This should be able to allocate the correct amount of space for the line
	char* line = static_cast<char*>(malloc(k_line_size));
	fgets(line, k_line_size, file);

	int32 floor_index = 0;
	int32 line_length = static_cast<int32>(strlen(line));

	for (int32 char_index = 0; char_index < line_length; char_index++)
	{
		if (line[char_index] == '(')
		{
			floor_index++;
		}
		else if (line[char_index] == ')')
		{
			floor_index--;
		}
	}

	n_log::output("Santa is at floor {}\n", floor_index);

	free(line);
}

void execute_2015_day1_part2(FILE* file)
{
	// $TODO This should be able to allocate the correct amount of space for the line
	char* line = static_cast<char*>(malloc(k_line_size));
	fgets(line, k_line_size, file);

	int32 floor_index = 0;
	int32 basement_position = NONE;
	int32 line_length = static_cast<int32>(strlen(line));
	for (int32 char_index = 0; char_index < line_length; char_index++)
	{
		if (line[char_index] == '(')
		{
			floor_index++;
		}
		else if (line[char_index] == ')')
		{
			floor_index--;
		}

		if (floor_index == -1)
		{
			basement_position = char_index + 1;
			break;
		}
	}

	n_log::output("The basement is reached at position {}\n", basement_position);

	free(line);
}
