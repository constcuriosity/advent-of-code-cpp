
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_array.h"
#include "foundation/io/log.h"
#include "foundation/math/basic_math.h"

//-------------- constants

//-------------- prototypes

void execute_2023_day2_part1(FILE* file);
void execute_2023_day2_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2023_day2_exercise
{
	2023,
	2,
	"Cube Conundrum",
	execute_2023_day2_part1,
	execute_2023_day2_part2
};

//-------------- definitions

void execute_2023_day2_part1(FILE* file)
{
	c_medium_string line_buffer;

	uint64 total_game_ids = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		bool game_viable = true;

		const char* line= line_buffer.get_string();
		line += 5; // Increment to get past "Game "
		int32 colon_index = n_string::index_of(line, ":");

		uint64 game_id = n_string::to_unsigned_integer(line, colon_index);

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
					cube_amount = n_string::to_unsigned_integer(cube_info.string, cube_info.length);
				}
				else
				{
					if (n_string::equals(cube_info.string, "red", cube_info.length))
					{
						game_viable = cube_amount <= 12;
					}
					else if (n_string::equals(cube_info.string, "green", cube_info.length))
					{
						game_viable = cube_amount <= 13;
					}
					else if (n_string::equals(cube_info.string, "blue", cube_info.length))
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

	n_log::output("The total sum of all viable games with 12 red, 13 green, and 14 blue cubes is {}\n", total_game_ids);
}

void execute_2023_day2_part2(FILE* file)
{
	c_medium_string line_buffer;

	uint64 total_power = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		const char* line = line_buffer.get_string();

		int32 colon_index = n_string::index_of(line, ":");
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
					cube_amount = n_string::to_unsigned_integer(cube_info.string, cube_info.length);
				}
				else
				{
					if (n_string::equals(cube_info.string, "red", cube_info.length))
					{
						red_max = n_math::max(cube_amount, red_max);
					}
					else if (n_string::equals(cube_info.string, "green", cube_info.length))
					{
						green_max = n_math::max(cube_amount, green_max);
					}
					else if (n_string::equals(cube_info.string, "blue", cube_info.length))
					{
						blue_max = n_math::max(cube_amount, blue_max);
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

	n_log::output("The sum of all powers for all games is {}\n", total_power);
}
