
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_grid.h"
#include "foundation/containers/view/string_view.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

void execute_2016_day8_part1(FILE* file);
void execute_2016_day8_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day8_exercise
{
	2016,
	8,
	"Two-Factor Authentication",
	execute_2016_day8_part1,
	execute_2016_day8_part2
};

//-------------- definitions

void execute_2016_day8_part1(FILE* file)
{
	c_static_grid<char, 50, 6> screen;
	screen.fill('.');

	c_medium_string line_buffer;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

		c_utf8_tokenizer tokens(line_buffer.get_string(), " ");

		c_string_view command_view = tokens.get_token();

		if (command_view.equals("rect"))
		{
			tokens.advance();
			s_string_view dimension_view = tokens.get_token();

			c_utf8_tokenizer dimension_tokens(dimension_view.string, "x", dimension_view.length);
			c_string_view width_view = dimension_tokens.get_token();
			dimension_tokens.advance();
			c_string_view height_view = dimension_tokens.get_token();

			uint64 width = width_view.to_unsigned_integer();
			uint64 height = height_view.to_unsigned_integer();

			// Gotta subtract 1 to convert these to proper indicies
			screen.for_each(0, 0, static_cast<uint32>(width) - 1, static_cast<uint32>(height) - 1,
				[&](char& value, uint32 x, uint32 y) {
					value = '#';
				});
		}
		else if (command_view.equals("rotate"))
		{
			tokens.advance();
			c_string_view direction_view = tokens.get_token();
			bool rotating_column = false;

			if (direction_view.equals("column"))
			{
				rotating_column = true;
			}
			else if (direction_view.equals("row"))
			{
				rotating_column = false;
			}

			tokens.advance();
			s_string_view index_view = tokens.get_token();

			uint32 data_index = static_cast<uint32>(n_string::to_unsigned_integer(index_view.string + 2, index_view.length - 2));

			tokens.advance(); // by
			tokens.advance();
			c_string_view amount_view = tokens.get_token();

			uint32 rotate_amount = static_cast<uint32>(amount_view.to_unsigned_integer());

			if (rotating_column)
			{
				screen.rotate_column_down(data_index, rotate_amount);
			}
			else
			{
				screen.rotate_row_right(data_index, rotate_amount);
			}
		}
	}

	uint32 previous_row = 0;
	uint32 filled_amount = 0;
	screen.for_each([&](char value, uint32 x, uint32 y)
		{
			if (previous_row != y)
			{
				std::cout << std::endl;
				previous_row = y;
			}

			std::cout << value;

			if (value == '#')
			{
				filled_amount++;
			}
		});

	std::cout << std::endl << "There are " << filled_amount << " filled pixels" << std::endl;
}

void execute_2016_day8_part2(FILE* file)
{
	std::cout << "Look at the output of the previous day to get answer for part 2" << std::endl;
}
