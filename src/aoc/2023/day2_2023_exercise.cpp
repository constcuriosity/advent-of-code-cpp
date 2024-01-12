
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_array.h"
#include "foundation/math/int_math.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

void execute_day2_part_one(FILE* file);
void execute_day2_part_two(FILE* file);

//-------------- globals

extern s_aoc_exercise_data day2
{
	2,
	"Cube Conundrum",
	execute_day2_part_one,
	execute_day2_part_two
};

//-------------- definitions

void execute_day2_part_one(FILE* file)
{
	c_medium_string line_buffer;

	uint64 total_game_ids = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		bool game_viable = true;

		const char* line= line_buffer.get_string();
		line += 5; // Increment to get past "Game "
		int32 colon_index = c_string_utilities::index_of(line, ":");

		uint64 game_id = c_string_utilities::to_unsigned_integer(line, colon_index);

		line += colon_index + 2; // Move past the game id and the ": "

		c_utf8_tokenizer game_sets(line, ";\n\r");

		do
		{
			s_string_view game_set = game_sets.get_token();

			c_utf8_tokenizer cubes(game_set.string, " ,", game_set.length);
			bool reading_number = true;
			uint64 cube_amount = 0;

			do
			{
				s_string_view cube_info = cubes.get_token();

				if (reading_number)
				{
					cube_amount = c_string_utilities::to_unsigned_integer(cube_info.string, cube_info.length);
				}
				else
				{
					if (c_string_utilities::equals(cube_info.string, "red", cube_info.length))
					{
						game_viable = cube_amount <= 12;
					}
					else if (c_string_utilities::equals(cube_info.string, "green", cube_info.length))
					{
						game_viable = cube_amount <= 13;
					}
					else if (c_string_utilities::equals(cube_info.string, "blue", cube_info.length))
					{
						game_viable = cube_amount <= 14;
					}
				}

				reading_number = !reading_number;
			}
			while (game_viable && cubes.advance());
		}
		while (game_viable && game_sets.advance());

		if (game_viable)
		{
			total_game_ids += game_id;
		}
	}

	std::cout << "The total sum of all viable games with 12 red, 13 green, and 14 blue cubes is: " << total_game_ids << std::endl;
}

void execute_day2_part_two(FILE* file)
{
	c_medium_string line_buffer;

	uint64 total_power = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		const char* line = line_buffer.get_string();

		int32 colon_index = c_string_utilities::index_of(line, ":");
		line += colon_index + 2; // Move past the game id and the ": "

		uint64 red_max = 0;
		uint64 green_max = 0;
		uint64 blue_max = 0;

		c_utf8_tokenizer game_sets(line, ";\n\r");

		do
		{
			s_string_view game_set = game_sets.get_token();

			c_utf8_tokenizer cubes(game_set.string, " ,", game_set.length);
			bool reading_number = true;
			uint64 cube_amount = 0;

			do
			{
				s_string_view cube_info = cubes.get_token();

				if (reading_number)
				{
					cube_amount = c_string_utilities::to_unsigned_integer(cube_info.string, cube_info.length);
				}
				else
				{
					if (c_string_utilities::equals(cube_info.string, "red", cube_info.length))
					{
						red_max = int_max(cube_amount, red_max);
					}
					else if (c_string_utilities::equals(cube_info.string, "green", cube_info.length))
					{
						green_max = int_max(cube_amount, green_max);
					}
					else if (c_string_utilities::equals(cube_info.string, "blue", cube_info.length))
					{
						blue_max = int_max(cube_amount, blue_max);
					}
				}

				reading_number = !reading_number;
			}
			while (cubes.advance());
		}
		while (game_sets.advance());

		uint64 game_power = red_max * green_max * blue_max;
		total_power += game_power;
	}

	std::cout << "The sum of all powers for all games is: " << total_power << std::endl;
}