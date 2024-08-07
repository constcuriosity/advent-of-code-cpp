
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_grid.h"
#include "foundation/io/log.h"
#include "foundation/math/basic_math.h"
#include "foundation/memory/memory_utilities.h"

//-------------- constants

//-------------- prototypes

void execute_2023_day3_part1(FILE* file);
void execute_2023_day3_part2(FILE* file);

typedef c_static_grid<char, 140, 140> c_engine_schematic;

c_engine_schematic* generate_engine_schematic(FILE* file);
uint64 capture_number_at_coord(const c_engine_schematic* schematic, uint32 column_index, uint32 row_index);

//-------------- globals

extern const s_aoc_exercise_data k_2023_day3_exercise
{
	2023,
	3,
	"Gear Ratios",
	execute_2023_day3_part1,
	execute_2023_day3_part2
};

//-------------- definitions

void execute_2023_day3_part1(FILE* file)
{
	c_engine_schematic* schematic = generate_engine_schematic(file);

	uint64 part_number_total = 0;
	bool recording_number = false;
	bool current_number_valid_part = false;
	c_small_string number_string;

	for (int32 row_index = 0; row_index < c_engine_schematic::k_height; row_index++)
	{
		for (int32 column_index = 0; column_index < c_engine_schematic::k_width; column_index++)
		{
			char current_char = schematic->get_value(column_index, row_index);

			if (n_string::is_digit(current_char))
			{
				recording_number = true;
				number_string.append_char(current_char);

				for (int32 row_offset = -1; row_offset <= 1 && !current_number_valid_part; row_offset++)
				{
					for (int32 column_offset = -1; column_offset <= 1 && !current_number_valid_part; column_offset++)
					{
						uint32 neighbor_column = static_cast<uint32>(column_index + column_offset);
						uint32 neighbor_row = static_cast<uint32>(row_index + row_offset);

						if (schematic->is_valid_coord(neighbor_column, neighbor_row))
						{
							char neighbor_char = schematic->get_value(neighbor_column, neighbor_row);
							current_number_valid_part = neighbor_char != '.' && !n_string::is_digit(neighbor_char);
						}
					}
				}
			}
			else
			{
				if (recording_number && current_number_valid_part)
				{
					uint64 number = n_string::to_unsigned_integer(number_string.get_string());
					part_number_total += number;
				}

				number_string.clear();
				recording_number = false;
				current_number_valid_part = false;
			}
		}
	}

	n_log::output("Som of all of the part numbers is {}\n", part_number_total);

	n_memory::free<c_engine_schematic>(schematic);
}

void execute_2023_day3_part2(FILE* file)
{
	c_engine_schematic* schematic = generate_engine_schematic(file);

	uint64 gear_ratio_total = 0;
	c_small_string number_string;

	for (int32 row_index = 0; row_index < c_engine_schematic::k_height; row_index++)
	{
		for (int32 column_index = 0; column_index < c_engine_schematic::k_width; column_index++)
		{
			char current_char = schematic->get_value(column_index, row_index);

			if (current_char == '*')
			{
				uint64 first_part_number = k_uint64_max;
				uint64 second_part_number = k_uint64_max;
				bool just_recorded_number = false;

				for (int32 row_offset = -1; row_offset <= 1; row_offset++)
				{
					for (int32 column_offset = -1; column_offset <= 1; column_offset++)
					{
						uint32 neighbor_column = static_cast<uint32>(column_index + column_offset);
						uint32 neighbor_row = static_cast<uint32>(row_index + row_offset);

						if (schematic->is_valid_coord(neighbor_column, neighbor_row))
						{
							char neighbor_char = schematic->get_value(neighbor_column, neighbor_row);
							bool char_is_digit = n_string::is_digit(neighbor_char);

							if (char_is_digit && !just_recorded_number)
							{
								uint64 part_number = capture_number_at_coord(schematic, neighbor_column, neighbor_row);

								if (first_part_number == k_uint64_max)
								{
									first_part_number = part_number;
								}
								else if (second_part_number == k_uint64_max)
								{
									second_part_number = part_number;
								}
								
								just_recorded_number = true;
							}
							else if (!char_is_digit && just_recorded_number)
							{
								just_recorded_number = false;
							}
						}
					}

					just_recorded_number = false;
				}

				if (first_part_number != k_uint64_max && second_part_number != k_uint64_max)
				{
					gear_ratio_total += first_part_number * second_part_number;
				}
			}
		}
	}

	n_log::output("Som of all of the gear ratios is {}\n", gear_ratio_total);

	n_memory::free<c_engine_schematic>(schematic);
}

c_engine_schematic* generate_engine_schematic(FILE* file)
{
	c_engine_schematic* schematic = n_memory::allocate<c_engine_schematic>();

	int32 current_column = 0;
	int32 current_row = 0;

	c_medium_string line_buffer;
	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

		c_utf8_character_iterator char_iter(line_buffer.get_string());

		do
		{
			schematic->set_value(current_column, current_row, static_cast<char>(char_iter.get_codepoint()));
			current_column++;
		}
		while (char_iter.advance() && current_column < c_engine_schematic::k_width);

		current_column = 0;
		current_row++;
	}

	return schematic;
}

uint64 capture_number_at_coord(
	const c_engine_schematic* schematic,
	uint32 column_index,
	uint32 row_index)
{
	uint32 current_column_index = column_index;

	assert(n_string::is_digit(schematic->get_value(current_column_index, row_index)));

	while (schematic->is_valid_coord(current_column_index - 1, row_index) &&
		n_string::is_digit(schematic->get_value(current_column_index - 1, row_index)))
	{
		current_column_index--;
	}

	assert(n_string::is_digit(schematic->get_value(current_column_index, row_index)));

	c_small_string number_string;

	while (schematic->is_valid_coord(current_column_index, row_index) &&
		n_string::is_digit(schematic->get_value(current_column_index, row_index)))
	{
		number_string.append_char(schematic->get_value(current_column_index, row_index));
		current_column_index++;
	}

	return n_string::to_unsigned_integer(number_string.get_string());
}
