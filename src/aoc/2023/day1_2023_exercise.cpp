
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_array.h"
#include "foundation/math/int_math.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

void execute_day1_part_one(FILE* file);
void execute_day1_part_two(FILE* file);

//-------------- globals

extern s_aoc_exercise_data day1
{
	1,
	"Trebuchet?!",
	execute_day1_part_one,
	execute_day1_part_two
};

//-------------- definitions

void execute_day1_part_one(FILE* file)
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
			if (c_string_utilities::is_digit(current_character))
			{
				first_digit = c_string_utilities::to_digit(current_character);
				break;
			}
		}

		for (int32 char_index = length - 1; char_index >= 0; char_index--)
		{
			char current_character = line_buffer.at(char_index);
			if (c_string_utilities::is_digit(current_character))
			{
				second_digit = c_string_utilities::to_digit(current_character);
				break;
			}
		}

		total += first_digit * 10 + second_digit;
	}

	std::cout << "The total callibration amount for the first question is: " << total << std::endl;
}

void execute_day1_part_two(FILE* file)
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
		key_views[key_index].length = c_string_utilities::length(k_keys[key_index]);
	}

	int32 total = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		s_string_view first_key_view = c_string_utilities::find_first_key(line_buffer.get_string(), key_views.view());
		s_string_view second_key_view = c_string_utilities::find_last_key(line_buffer.get_string(), key_views.view());
		uint32 first_digit = key_views.index_of(first_key_view) % 10;
		uint32 second_digit = key_views.index_of(second_key_view) % 10;

		total += first_digit * 10 + second_digit;
	}

	std::cout << "The total callibration amount for the second question is: " << total << std::endl;
}
