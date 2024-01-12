
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_bit_grid.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_grid.h"
#include "foundation/math/int_math.h"
#include "foundation/utilities/string_utilities.h"

#include <iostream>

//-------------- prototypes

enum class e_light_operation
{
	on,
	off,
	toggle
};

struct s_light_change
{
	e_light_operation operation;
	uint32 start_x;
	uint32 start_y;
	uint32 end_x;
	uint32 end_y;
};

void execute_day6_part_one(FILE* file);
void execute_day6_part_two(FILE* file);

static_function void parse_light_change(const c_medium_string& line, s_light_change* out_change);

//-------------- globals

extern s_aoc_exercise_data day6
{
	6,
	"Doesn't He Have Intern-Elves For This?",
	execute_day6_part_one,
	execute_day6_part_two
};

//-------------- public implementations

void execute_day6_part_one(FILE* file)
{
	c_static_bit_grid<1000, 1000>* light_grid = new c_static_bit_grid<1000, 1000>();
	c_medium_string line_buffer;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_small_string::k_capacity, file);

		s_light_change light_change;
		parse_light_change(line_buffer, &light_change);

		// Apply the change
		if (light_change.operation == e_light_operation::on)
		{
			light_grid->set_area(
				light_change.start_x,
				light_change.start_y,
				light_change.end_x,
				light_change.end_y);
		}
		else if (light_change.operation == e_light_operation::off)
		{
			light_grid->clear_area(
				light_change.start_x,
				light_change.start_y,
				light_change.end_x,
				light_change.end_y);
		}
		else if (light_change.operation == e_light_operation::toggle)
		{
			light_grid->toggle_area(
				light_change.start_x,
				light_change.start_y,
				light_change.end_x,
				light_change.end_y);
		}
	}

	std::cout << "There are " << light_grid->count() << " lights turned on." << std::endl;

	delete light_grid;
}

void execute_day6_part_two(FILE* file)
{
	c_static_grid<int32, 1000, 1000>* grid = new c_static_grid<int32, 1000, 1000>(0);

	c_medium_string line_buffer;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_small_string::k_capacity, file);

		s_light_change light_change;
		parse_light_change(line_buffer, &light_change);

		int32 light_offset = 0;

		if (light_change.operation == e_light_operation::on)
		{
			light_offset = 1;
		}
		else if (light_change.operation == e_light_operation::off)
		{
			light_offset = -1;
		}
		else if (light_change.operation == e_light_operation::toggle)
		{
			light_offset = 2;
		}

		grid->for_each(
			light_change.start_x,
			light_change.start_y,
			light_change.end_x,
			light_change.end_y,
			[&](int32 value, uint32 x, uint32 y)
			{
				grid->set_value(x, y, int_max(value + light_offset, 0));
			});
	}

	int32 total_brightness = 0;

	grid->for_each([&](int32 value, uint32 x, uint32 y)
		{
			UNUSED(x); UNUSED(y);
			total_brightness += value;
		});

	std::cout << "The total brightness value of all the lights is " << total_brightness << "." << std::endl << std::endl;

	delete grid;
}

//-------------- private implementations

static_function void parse_light_change(const c_medium_string& line, s_light_change* out_change)
{
	e_light_operation light_operation = e_light_operation::on;

	c_utf8_tokenizer line_tokens(line.get_string(), " ,");
	s_string_view current_token = line_tokens.get_token();

	// Figure out what we're doing with the grid
	if (c_string_utilities::equals(current_token.string, "turn", current_token.length))
	{
		line_tokens.advance();
		current_token = line_tokens.get_token();
		if (c_string_utilities::equals(current_token.string, "off", current_token.length))
		{
			light_operation = e_light_operation::off;
		}
	}
	else if (c_string_utilities::equals(current_token.string, "toggle", current_token.length))
	{
		light_operation = e_light_operation::toggle;
	}

	// Get the coordinates
	line_tokens.advance();
	current_token = line_tokens.get_token();
	int64 start_x = c_string_utilities::to_integer(current_token.string, current_token.length);

	line_tokens.advance();
	current_token = line_tokens.get_token();
	int64 start_y = c_string_utilities::to_integer(current_token.string, current_token.length);

	line_tokens.advance();

	line_tokens.advance();
	current_token = line_tokens.get_token();
	int64 end_x = c_string_utilities::to_integer(current_token.string, current_token.length);

	line_tokens.advance();
	current_token = line_tokens.get_token();
	int64 end_y = c_string_utilities::to_integer(current_token.string, current_token.length);

	out_change->operation = light_operation;
	out_change->start_x = static_cast<uint32>(start_x);
	out_change->start_y = static_cast<uint32>(start_y);
	out_change->end_x = static_cast<uint32>(end_x);
	out_change->end_y = static_cast<uint32>(end_y);
}
