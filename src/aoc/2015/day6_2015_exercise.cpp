
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_bit_grid.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_grid.h"
#include "foundation/io/log.h"
#include "foundation/math/basic_math.h"
#include "foundation/utilities/string_utilities.h"

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

void execute_2015_day6_part1(FILE* file);
void execute_2015_day6_part2(FILE* file);

static_function void parse_light_change(const c_medium_string& line, s_light_change* out_change);

//-------------- globals

extern const s_aoc_exercise_data k_2015_day6_exercise
{
	2015,
	6,
	"Doesn't He Have Intern-Elves For This?",
	execute_2015_day6_part1,
	execute_2015_day6_part2
};

//-------------- public implementations

void execute_2015_day6_part1(FILE* file)
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

	n_log::output("There are {} lights turned on\n", light_grid->count());

	delete light_grid;
}

void execute_2015_day6_part2(FILE* file)
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
				grid->set_value(x, y, n_math::max(value + light_offset, 0));
			});
	}

	int32 total_brightness = 0;

	grid->for_each([&](int32 value, uint32 x, uint32 y)
		{
			UNUSED(x); UNUSED(y);
			total_brightness += value;
		});

	n_log::output("The total brightness value of all the lights is {}\n", total_brightness);

	delete grid;
}

//-------------- private implementations

static_function void parse_light_change(const c_medium_string& line, s_light_change* out_change)
{
	e_light_operation light_operation = e_light_operation::on;

	c_utf8_tokenizer line_tokens(line.get_string(), " ,");
	s_string_view current_token = line_tokens.get_token();

	// Figure out what we're doing with the grid
	if (n_string::equals(current_token.string, "turn", current_token.length))
	{
		line_tokens.advance();
		current_token = line_tokens.get_token();
		if (n_string::equals(current_token.string, "off", current_token.length))
		{
			light_operation = e_light_operation::off;
		}
	}
	else if (n_string::equals(current_token.string, "toggle", current_token.length))
	{
		light_operation = e_light_operation::toggle;
	}

	// Get the coordinates
	line_tokens.advance();
	current_token = line_tokens.get_token();
	int64 start_x = n_string::to_integer(current_token.string, current_token.length);

	line_tokens.advance();
	current_token = line_tokens.get_token();
	int64 start_y = n_string::to_integer(current_token.string, current_token.length);

	line_tokens.advance();

	line_tokens.advance();
	current_token = line_tokens.get_token();
	int64 end_x = n_string::to_integer(current_token.string, current_token.length);

	line_tokens.advance();
	current_token = line_tokens.get_token();
	int64 end_y = n_string::to_integer(current_token.string, current_token.length);

	out_change->operation = light_operation;
	out_change->start_x = static_cast<uint32>(start_x);
	out_change->start_y = static_cast<uint32>(start_y);
	out_change->end_x = static_cast<uint32>(end_x);
	out_change->end_y = static_cast<uint32>(end_y);
}
