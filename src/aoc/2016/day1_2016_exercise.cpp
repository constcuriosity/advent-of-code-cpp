
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_bit_grid.h"
#include "foundation/memory/memory_utilities.h"
#include "foundation/math/int_math.h"

#include <iostream>

//-------------- constants

enum e_facing_direction
{
	_facing_direction_north,
	_facing_direction_south,
	_facing_direction_east,
	_facing_direction_west,

	k_facing_direction_count
};

e_facing_direction k_left_facing[] =
{
	_facing_direction_west,
	_facing_direction_east,
	_facing_direction_north,
	_facing_direction_south
};

e_facing_direction k_right_facing[] =
{
	_facing_direction_east,
	_facing_direction_west,
	_facing_direction_south,
	_facing_direction_north
};

//-------------- prototypes

void execute_2016_day1_part1(FILE* file);
void execute_2016_day1_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day1_exercise
{
	2016,
	1,
	"No Time for a Taxicab",
	execute_2016_day1_part1,
	execute_2016_day1_part2
};

//-------------- definitions

void execute_2016_day1_part1(FILE* file)
{
	c_kilo_string line_buffer;

	e_facing_direction current_facing = _facing_direction_north;
	int32 x = 0;
	int32 y = 0;

	fgets(line_buffer.get_string(), c_kilo_string::k_capacity, file);

	c_utf8_tokenizer direction_tokens(line_buffer.get_string(), " ,");

	do
	{
		 s_string_view current_token = direction_tokens.get_token();
		 
		 if (current_token.string[0] == 'L')
		 {
			 current_facing = k_left_facing[current_facing];
			 std::cout << "Turning Left" << std::endl;

		 }
		 else if (current_token.string[0] == 'R')
		 {
			 current_facing = k_right_facing[current_facing];
			 std::cout << "Turning Right" << std::endl;
		 }
		 else
		 {
			 haltv("Unaccounted for direction");
		 }

		 s_string_view distance_view = current_token;
		 distance_view.string++;

		 uint32 distance = static_cast<uint32>(c_string_utilities::to_unsigned_integer(distance_view));

		 std::cout << "Going distance " << distance << std::endl;

		 switch (current_facing)
		 {
		 case _facing_direction_north:
			 y += distance;
			 break;
		 case _facing_direction_south:
			 y -= distance;
			 break;
		 case _facing_direction_east:
			 x += distance;
			 break;
		 case _facing_direction_west:
			 x -= distance;
			 break;
		 default:
			 haltv("Unaccounted for facing");
		 }
	}
	while (direction_tokens.advance());

	std::cout << "Start position is " << int_abs(x) + int_abs(y) << " blocks away from Easter Bunny HQ" << std::endl;
}

void execute_2016_day1_part2(FILE* file)
{
#if 0 // Disable for now
	c_kilo_string line_buffer;

	e_facing_direction current_facing = _facing_direction_north;
	int32 start_x = 0;
	int32 start_y = 0;

	c_static_bit_grid<1000,1000> *grid = c_memory_utilities::allocate<c_static_bit_grid<1000, 1000>>();
	new(grid) c_static_bit_grid<1000,1000>();

	fgets(line_buffer.get_string(), c_kilo_string::k_capacity, file);

	c_utf8_tokenizer direction_tokens(line_buffer.get_string(), " ,");

	do
	{
		int32 current_x = start_x;
		int32 current_y = start_y;

		s_string_view current_token = direction_tokens.get_token();

		if (current_token.string[0] == 'L')
		{
			current_facing = k_left_facing[current_facing];
			std::cout << "Turning Left" << std::endl;

		}
		else if (current_token.string[0] == 'R')
		{
			current_facing = k_right_facing[current_facing];
			std::cout << "Turning Right" << std::endl;
		}
		else
		{
			haltv("Unaccounted for direction");
		}

		s_string_view distance_view = current_token;
		distance_view.string++;

		uint32 distance = static_cast<uint32>(c_string_utilities::to_unsigned_integer(distance_view));

		std::cout << "Going distance " << distance << std::endl;

		switch (current_facing)
		{
		case _facing_direction_north:
			current_y += distance;
			break;
		case _facing_direction_south:
			current_y -= distance;
			break;
		case _facing_direction_east:
			current_x += distance;
			break;
		case _facing_direction_west:
			current_x -= distance;
			break;
		default:
			haltv("Unaccounted for facing");
		}

		grid->test_area();

		start_x = current_x;
		start_y = current_y;
	}
	while (direction_tokens.advance());

	std::cout << "Start position is " << int_abs(x) + int_abs(y) << " blocks away from Easter Bunny HQ" << std::endl;
#endif // 0
	std::cout << "Part 2 disabled" << std::endl;
}
