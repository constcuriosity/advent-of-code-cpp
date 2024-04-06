
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/containers/view/string_view.h"

#include <iostream>
#include <unordered_map>

//-------------- constants

//-------------- prototypes

struct s_bot
{
	s_bot() : id(NONE), low_target_id(NONE), high_target_id(NONE), low_is_bot(false), high_is_bot(false), dispatched(false)
	{
		chips.fill(NONE);
	}

	bool has_all_values() const { return chips[0] != NONE && chips[1] != NONE; }

	void give_value(int16 value)
	{
		chips[0] = value;
		if (chips[0] > chips[1])
		{
			chips[0] = chips[1];
			chips[1] = value;
		}
	}

	int16 id;
	c_static_array<int16, 2> chips;
	int16 low_target_id;
	int16 high_target_id;
	bool low_is_bot;
	bool high_is_bot;
	bool dispatched;
};

void build_bot_map(FILE* file, std::unordered_map<int16, s_bot>* out_map);
void process_bot_map(std::unordered_map<int16, s_bot>* inout_map);

void execute_2016_day10_part1(FILE* file);
void execute_2016_day10_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day10_exercise
{
	2016,
	10,
	"Balance Bots",
	execute_2016_day10_part1,
	execute_2016_day10_part2
};

//-------------- definitions

void execute_2016_day10_part1(FILE* file)
{
	int16 target_bot_id = NONE;
	const int16 k_target_low_chip = 17;
	const int16 k_target_high_chip = 61;

	std::unordered_map<int16, s_bot> bot_map;
	build_bot_map(file, &bot_map);
	process_bot_map(&bot_map);

	auto bot_iter = bot_map.begin();
	auto last_bot = bot_map.end();

	for (; bot_iter != last_bot; bot_iter++)
	{
		if (bot_iter->second.chips[0] == k_target_low_chip &&
			bot_iter->second.chips[1] == k_target_high_chip)
		{
			target_bot_id = bot_iter->first;
			break;
		}
	}

	std::cout << "Bot " << target_bot_id << " is the one that processes chips 17 and 61" << std::endl;
}

void execute_2016_day10_part2(FILE* file)
{
	int32 result = 1;

	std::unordered_map<int16, s_bot> bot_map;
	build_bot_map(file, &bot_map);
	process_bot_map(&bot_map);

	auto bot_iter = bot_map.begin();
	auto last_bot = bot_map.end();

	for (; bot_iter != last_bot; bot_iter++)
	{
		s_bot& current_bot = bot_iter->second;

		if (!current_bot.low_is_bot &&
			(current_bot.low_target_id == 0 ||
				current_bot.low_target_id == 1 ||
				current_bot.low_target_id == 2))
		{
			result *= current_bot.chips[0];
		}
		else if (!current_bot.high_is_bot &&
			(current_bot.high_target_id == 0 ||
				current_bot.high_target_id == 1 ||
				current_bot.high_target_id == 2))
		{
			result *= current_bot.chips[1];
		}
	}

	std::cout << "Multiplying output bins 0, 1, and 2 altogether gives " << result << std::endl;
}

void build_bot_map(FILE* file, std::unordered_map<int16, s_bot>* out_map)
{
	c_medium_string line_buffer;
	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);
		c_utf8_tokenizer tokens(line_buffer.get_string(), " ");

		c_string_view first_token = tokens.get_token();
		if (first_token.equals("value"))
		{
			tokens.advance();
			c_string_view second_token = tokens.get_token();
			int16 value = static_cast<int16>(second_token.to_integer());

			tokens.last();
			c_string_view last_token = tokens.get_token();
			int16 bot_id = static_cast<int16>(last_token.to_integer());

			s_bot& target_bot = (*out_map)[bot_id];
			if (target_bot.id == NONE)
			{
				target_bot.id = bot_id;
			}

			target_bot.give_value(value);
		}
		else
		{
			tokens.advance();
			c_string_view bot_id_token = tokens.get_token();
			int16 bot_id = static_cast<int16>(bot_id_token.to_integer());

			s_bot& target_bot = (*out_map)[bot_id];
			if (target_bot.id == NONE)
			{
				target_bot.id = bot_id;
			}

			tokens.advance(); // gives
			tokens.advance(); // low
			tokens.advance(); // to

			tokens.advance(); // bot / output
			c_string_view low_bot_or_output_token = tokens.get_token();
			target_bot.low_is_bot = low_bot_or_output_token.equals("bot");

			tokens.advance();
			c_string_view low_target_id_token = tokens.get_token();
			target_bot.low_target_id = static_cast<int16>(low_target_id_token.to_integer());

			tokens.advance(); // and
			tokens.advance(); // high
			tokens.advance(); // to

			tokens.advance(); // bot / output
			c_string_view high_bot_or_output_token = tokens.get_token();
			target_bot.high_is_bot = high_bot_or_output_token.equals("bot");

			tokens.advance();
			c_string_view high_target_id_token = tokens.get_token();
			target_bot.high_target_id = static_cast<int16>(high_target_id_token.to_integer());
		}
	}
}

void process_bot_map(std::unordered_map<int16, s_bot>* inout_map)
{
	bool has_undispatched_bots = true;
	while (has_undispatched_bots)
	{
		has_undispatched_bots = false;

		auto bot_iter = inout_map->begin();
		auto last_bot = inout_map->end();

		for (; bot_iter != last_bot; bot_iter++)
		{
			s_bot& current_bot = bot_iter->second;

			if (!current_bot.dispatched &&
				current_bot.has_all_values())
			{
				if (current_bot.low_is_bot)
				{
					s_bot& low_target = (*inout_map)[current_bot.low_target_id];
					low_target.give_value(current_bot.chips[0]);
				}
				
				if (current_bot.high_is_bot)
				{
					s_bot& high_target = (*inout_map)[current_bot.high_target_id];
					high_target.give_value(current_bot.chips[1]);
				}

				current_bot.dispatched = true;
			}
			else if (!current_bot.dispatched)
			{
				has_undispatched_bots = true;
			}
		}
	}
}
