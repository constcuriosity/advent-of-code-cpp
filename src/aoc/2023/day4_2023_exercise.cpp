
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_set.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/io/log.h"
#include "foundation/math/basic_math.h"

//-------------- constants

//-------------- prototypes

void execute_2023_day4_part1(FILE* file);
void execute_2023_day4_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2023_day4_exercise
{
	2023,
	4,
	"Scratchcards",
	execute_2023_day4_part1,
	execute_2023_day4_part2
};

//-------------- definitions

void execute_2023_day4_part1(FILE* file)
{
	int32 total_points = 0;
	c_medium_string line_buffer;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		c_utf8_tokenizer line_tokens(line_buffer.get_string(), " :\n\r");

		line_tokens.advance(2); // Advance past "Game" and game id

		c_static_set<uint8, 10> winning_numbers;
		c_static_set<uint8, 25> owned_numbers;

		for (int32 winning_number_index = 0; winning_number_index < winning_numbers.get_capacity(); winning_number_index++)
		{
			uint64 number = n_string::to_unsigned_integer(line_tokens.get_token().string, line_tokens.get_token().length);
			winning_numbers.add(static_cast<uint8>(number));
			line_tokens.advance();
		}

		line_tokens.advance(); // Go past the "|" barrier

		for (int32 owned_number_index = 0; owned_number_index < owned_numbers.get_capacity(); owned_number_index++)
		{
			uint64 number = n_string::to_unsigned_integer(line_tokens.get_token().string, line_tokens.get_token().length);
			owned_numbers.add(static_cast<uint8>(number));
			line_tokens.advance();
		}

		int32 winning_matches = 0;

		// Ideally this would be made into a generic utility, but gotta go fast right now.
		// Here's a union function
		for (int32 winning_number_index = 0; winning_number_index < winning_numbers.get_size(); winning_number_index++)
		{
			int32 index = owned_numbers.index_of(winning_numbers[winning_number_index]);

			if (index != NONE)
			{
				winning_matches++;
			}
		}

		int32 points = winning_matches > 0 ? static_cast<int32>(n_math::int_pow_two(winning_matches - 1)) : 0;

		total_points += points;
	}

	n_log::output("The number of earned points across all games is {}\n", total_points);
}

void execute_2023_day4_part2(FILE* file)
{
	c_medium_string line_buffer;

	// There's 205 lines in the file, and I'm too lazy to determine this procedurally
	c_static_array<uint32, 205> scorecard_counts(1);
	int32 scorecard_index = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		c_utf8_tokenizer line_tokens(line_buffer.get_string(), " :\n\r");

		line_tokens.advance(2); // Advance past "Game" and game id

		c_static_set<uint8, 10> winning_numbers;
		c_static_set<uint8, 25> owned_numbers;

		for (int32 winning_number_index = 0; winning_number_index < winning_numbers.get_capacity(); winning_number_index++)
		{
			uint64 number = n_string::to_unsigned_integer(line_tokens.get_token().string, line_tokens.get_token().length);
			winning_numbers.add(static_cast<uint8>(number));
			line_tokens.advance();
		}

		line_tokens.advance(); // Go past the "|" barrier

		for (int32 owned_number_index = 0; owned_number_index < owned_numbers.get_capacity(); owned_number_index++)
		{
			uint64 number = n_string::to_unsigned_integer(line_tokens.get_token().string, line_tokens.get_token().length);
			owned_numbers.add(static_cast<uint8>(number));
			line_tokens.advance();
		}

		int32 winning_matches = 0;

		// Ideally this would be made into a generic utility, but gotta go fast right now.
		// Here's a union function
		for (int32 winning_number_index = 0; winning_number_index < winning_numbers.get_size(); winning_number_index++)
		{
			int32 index = owned_numbers.index_of(winning_numbers[winning_number_index]);

			if (index != NONE)
			{
				winning_matches++;
			}
		}

		for (int32 scorecard_offset = 1; scorecard_offset <= winning_matches; scorecard_offset++)
		{
			if (VALID_INDEX(scorecard_index + scorecard_offset, static_cast<int32>(scorecard_counts.get_size())))
			{
				scorecard_counts[scorecard_index + scorecard_offset] += scorecard_counts[scorecard_index];
			}
		}

		scorecard_index++;
	}

	uint64 total_cards = 0;
	for (scorecard_index = 0; scorecard_index < static_cast<int32>(scorecard_counts.get_size()); scorecard_index++)
	{
		total_cards += scorecard_counts[scorecard_index];
	}

	n_log::output("The number of earned scratchards {}\n", total_cards);
}
