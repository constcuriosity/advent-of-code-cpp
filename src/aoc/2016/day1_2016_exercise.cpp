
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/dynamic/dynamic_set.h"
#include "foundation/memory/memory_utilities.h"
#include "foundation/math/basic_math.h"

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

struct s_point
{
	s_point() : x(0), y(0) {}
	s_point(int32 in_x, int32 in_y) : x(in_x), y(in_y) {}

	int32 x;
	int32 y;
};

bool operator==(const s_point& left, const s_point& right)
{
	return left.x == right.x && left.y == right.y;
}

bool operator!=(const s_point& left, const s_point& right)
{
	return !(left == right);
}

bool operator<(const s_point& left, const s_point& right)
{
	return (left.x < right.x) ||
		(left.x == right.x && (left.y < right.y));
}

bool operator>(const s_point& left, const s_point& right)
{
	return (left.x > right.x) ||
		(left.x == right.x && (left.y > right.y));
}

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

		 }
		 else if (current_token.string[0] == 'R')
		 {
			 current_facing = k_right_facing[current_facing];
		 }
		 else
		 {
			 haltv("Unaccounted for direction");
		 }

		 s_string_view distance_view = current_token;
		 distance_view.string++;

		 uint32 distance = static_cast<uint32>(n_string::to_unsigned_integer(distance_view));

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

	std::cout << "Start position is " << n_math::abs(x) + n_math::abs(y) << " blocks away from Easter Bunny HQ" << std::endl;
}

void execute_2016_day1_part2(FILE* file)
{
	c_kilo_string line_buffer;

	s_point target_point;
	bool target_found = false;
	e_facing_direction current_facing = _facing_direction_north;
	int32 start_x = 0;
	int32 start_y = 0;

	c_dynamic_set<s_point> traveled_set;
	traveled_set.add(s_point());

	fgets(line_buffer.get_string(), c_kilo_string::k_capacity, file);

	c_utf8_tokenizer direction_tokens(line_buffer.get_string(), " ,");

	do
	{
		s_string_view current_token = direction_tokens.get_token();

		if (current_token.string[0] == 'L')
		{
			current_facing = k_left_facing[current_facing];
		}
		else if (current_token.string[0] == 'R')
		{
			current_facing = k_right_facing[current_facing];
		}
		else
		{
			haltv("Unaccounted for direction");
		}

		s_string_view distance_view = current_token;
		distance_view.string++;

		int32 distance = static_cast<int32>(n_string::to_unsigned_integer(distance_view));
		int32 mod_x = 0;
		int32 mod_y = 0;

		switch (current_facing)
		{
		case _facing_direction_north:
			mod_y = 1;
			break;
		case _facing_direction_south:
			mod_y = -1;
			break;
		case _facing_direction_east:
			mod_x = 1;
			break;
		case _facing_direction_west:
			mod_x = -1;
			break;
		default:
			haltv("Unaccounted for facing");
		}

		for (int32 offset = 1; offset <= distance; offset++)
		{
			s_point current_point(start_x + mod_x * offset, start_y + mod_y * offset);
			if (traveled_set.contains(current_point))
			{
				target_point = current_point;
				target_found = true;
				break;
			}
			else
			{
				traveled_set.add(current_point);
			}
		}

		start_x += mod_x * distance;
		start_y += mod_y * distance;
	}
	while (!target_found && direction_tokens.advance());

	std::cout << "First location visited twice is " << n_math::abs(target_point.x) + n_math::abs(target_point.y) << " blocks from the starting location" << std::endl;
}
