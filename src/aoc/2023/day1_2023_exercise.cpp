
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_array.h"
#include "foundation/io/log.h"
#include "foundation/math/basic_math.h"

//-------------- constants

//-------------- prototypes

void execute_2023_day1_part1(FILE* file);
void execute_2023_day1_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2023_day1_exercise
{
	2023,
	1,
	"Trebuchet?!",
	execute_2023_day1_part1,
	execute_2023_day1_part2
};

//-------------- definitions

void execute_2023_day1_part1(FILE* file)
{
	c_medium_string line_buffer;

	int32 total = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		uint32 first_digit = 0;
		uint32 second_digit = 0;

		uint32 length = line_buffer.length();

		for (uint32 char_index = 0; char_index < length; char_index++)
		{
			char current_character= line_buffer.at(char_index);
			if (n_string::is_digit(current_character))
			{
				first_digit = n_string::to_digit(current_character);
				break;
			}
		}

		for (int32 char_index = length - 1; char_index >= 0; char_index--)
		{
			char current_character = line_buffer.at(char_index);
			if (n_string::is_digit(current_character))
			{
				second_digit = n_string::to_digit(current_character);
				break;
			}
		}

		total += first_digit * 10 + second_digit;
	}

	n_log::output("The total callibration amount for the first question is {}\n", total);
}

void execute_2023_day1_part2(FILE* file)
{
	c_medium_string line_buffer;

	const char* k_keys[] =
	{
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
	};

	c_static_array<s_string_view, 20> key_views;
	for (int32 key_index = 0; key_index < 20; key_index++)
	{
		key_views[key_index].string = k_keys[key_index];
		key_views[key_index].length = n_string::length(k_keys[key_index]);
	}

	int32 total = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		s_string_view first_key_view = n_string::find_first_key(line_buffer.get_string(), key_views.view());
		s_string_view second_key_view = n_string::find_last_key(line_buffer.get_string(), key_views.view());
		uint32 first_digit = key_views.index_of(first_key_view) % 10;
		uint32 second_digit = key_views.index_of(second_key_view) % 10;

		total += first_digit * 10 + second_digit;
	}

	n_log::output("The total callibration amount for the second question is {}\n", total);
}
