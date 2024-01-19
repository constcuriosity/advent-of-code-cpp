
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_grid.h"

#include "foundation/math/vector_math.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

void execute_2016_day2_part1(FILE* file);
void execute_2016_day2_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day2_exercise
{
	2016,
	2,
	"Bathroom Security",
	execute_2016_day2_part1,
	execute_2016_day2_part2
};

//-------------- definitions

void execute_2016_day2_part1(FILE* file)
{
	c_static_grid<char, 3, 3> keypad;
	keypad.set_value(0, 0, '1');
	keypad.set_value(1, 0, '2');
	keypad.set_value(2, 0, '3');
	keypad.set_value(0, 1, '4');
	keypad.set_value(1, 1, '5');
	keypad.set_value(2, 1, '6');
	keypad.set_value(0, 2, '7');
	keypad.set_value(1, 2, '8');
	keypad.set_value(2, 2, '9');

	c_small_string keypad_code;
	s_point2b current_coord(1, 1);

	c_kilo_string line_buffer;
	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

		c_utf8_character_iterator instruction_iter(line_buffer.get_string());

		do
		{
			t_unicode_codepoint current_codepoint = instruction_iter.get_codepoint();
			s_vector2 direction;

			switch (current_codepoint)
			{
			case 'U':
				direction.y = -1;
				break;
			case 'D':
				direction.y = 1;
				break;
			case 'L':
				direction.x = -1;
				break;
			case 'R':
				direction.x = 1;
				break;
			default:
				while (false) { void; }
			}

			s_point2b new_target = current_coord + direction;

			if (keypad.is_valid_coord(new_target.x, new_target.y))
			{
				current_coord = new_target;
			}

		}
		while (instruction_iter.advance());

		keypad_code.append_char(keypad.get_value(current_coord.x, current_coord.y));
	}

	std::cout << "The keycode to the theoretical bathroom lock is " << keypad_code.get_string() << std::endl;
}

void execute_2016_day2_part2(FILE* file)
{
	c_static_grid<char, 5, 5> keypad;
	keypad.set_value(0, 0, 'x');
	keypad.set_value(1, 0, 'x');
	keypad.set_value(2, 0, '1');
	keypad.set_value(3, 0, 'x');
	keypad.set_value(4, 0, 'x');
	keypad.set_value(0, 1, 'x');
	keypad.set_value(1, 1, '2');
	keypad.set_value(2, 1, '3');
	keypad.set_value(3, 1, '4');
	keypad.set_value(4, 1, 'x');
	keypad.set_value(0, 2, '5');
	keypad.set_value(1, 2, '6');
	keypad.set_value(2, 2, '7');
	keypad.set_value(3, 2, '8');
	keypad.set_value(4, 2, '9');
	keypad.set_value(0, 3, 'x');
	keypad.set_value(1, 3, 'A');
	keypad.set_value(2, 3, 'B');
	keypad.set_value(3, 3, 'C');
	keypad.set_value(4, 3, 'x');
	keypad.set_value(0, 4, 'x');
	keypad.set_value(1, 4, 'x');
	keypad.set_value(2, 4, 'D');
	keypad.set_value(3, 4, 'x');
	keypad.set_value(4, 4, 'x');

	c_small_string keypad_code;
	s_point2b current_coord(0, 2);

	c_kilo_string line_buffer;
	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

		c_utf8_character_iterator instruction_iter(line_buffer.get_string());

		do
		{
			t_unicode_codepoint current_codepoint = instruction_iter.get_codepoint();
			s_vector2 direction;

			switch (current_codepoint)
			{
			case 'U':
				direction.y = -1;
				break;
			case 'D':
				direction.y = 1;
				break;
			case 'L':
				direction.x = -1;
				break;
			case 'R':
				direction.x = 1;
				break;
			default:
				while (false) { void; }
			}

			s_point2b new_target = current_coord + direction;

			if (keypad.is_valid_coord(new_target.x, new_target.y) && keypad.get_value(new_target.x, new_target.y) != 'x')
			{
				current_coord = new_target;
			}

		} while (instruction_iter.advance());

		keypad_code.append_char(keypad.get_value(current_coord.x, current_coord.y));
	}

	std::cout << "The keycode to the real complicated bathroom lock is " << keypad_code.get_string() << std::endl;
}
