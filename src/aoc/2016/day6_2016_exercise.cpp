
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_grid.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

typedef c_static_grid<uint32, 8, 26> c_character_counts;

void gather_character_counts(FILE* file, c_static_grid<uint32, 8, 26>* out_counts);

void execute_2016_day6_part1(FILE* file);
void execute_2016_day6_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day6_exercise
{
	2016,
	6,
	"How About a Nice Game of Chess?",
	execute_2016_day6_part1,
	execute_2016_day6_part2
};

//-------------- definitions

void execute_2016_day6_part1(FILE* file)
{
	c_static_grid<uint32, 8, 26> message_grid(0);
	gather_character_counts(file, &message_grid);

	// Gather message by MOST common characters
	c_small_string final_message;
	for (int32 message_char_index = 0; message_char_index < message_grid.k_width; message_char_index++)
	{
		char most_common_character = 'a';
		uint32 most_common_count = 0;

		for (int32 alphabet_index = 0; alphabet_index < message_grid.k_height; alphabet_index++)
		{
			uint32 char_count = message_grid.get_value(message_char_index, alphabet_index);
			if (char_count > most_common_count)
			{
				most_common_character = 'a' + alphabet_index;
				most_common_count = char_count;
			}
		}

		final_message.append_char(most_common_character);
	}

	std::cout << "Error corrected message reads: " << final_message.get_string() << std::endl;
}

void execute_2016_day6_part2(FILE* file)
{
	c_static_grid<uint32, 8, 26> message_grid(0);
	gather_character_counts(file, &message_grid);

	// Gather message by LEAST common characters
	c_small_string final_message;
	for (int32 message_char_index = 0; message_char_index < message_grid.k_width; message_char_index++)
	{
		char least_common_character = 'a';
		uint32 least_common_count = k_uint32_max;

		for (int32 alphabet_index = 0; alphabet_index < message_grid.k_height; alphabet_index++)
		{
			uint32 char_count = message_grid.get_value(message_char_index, alphabet_index);
			if (char_count < least_common_count)
			{
				least_common_character = 'a' + alphabet_index;
				least_common_count = char_count;
			}
		}

		final_message.append_char(least_common_character);
	}

	std::cout << "Error corrected message reads: " << final_message.get_string() << std::endl;
}

void gather_character_counts(FILE* file, c_static_grid<uint32, 8, 26>* out_counts)
{
	c_medium_string line_buffer;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

		for (int32 message_char_index = 0; message_char_index < line_buffer.length(); message_char_index++)
		{
			char current_char = line_buffer.at(message_char_index);
			int32 alphabet_index = current_char - 'a';

			if (VALID_INDEX(message_char_index, out_counts->k_width) &&
				VALID_INDEX(alphabet_index, out_counts->k_height))
			{
				uint32 value = out_counts->get_value(message_char_index, alphabet_index);
				value++;
				out_counts->set_value(message_char_index, alphabet_index, value);
			}
		}
	}
}
