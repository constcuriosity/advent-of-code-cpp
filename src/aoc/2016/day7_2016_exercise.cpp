
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/dynamic/dynamic_set.h"
#include "foundation/io/log.h"

//-------------- constants

//-------------- prototypes

typedef c_static_string<4> c_triplet;
typedef c_dynamic_set<c_triplet> c_triplet_set;

bool ip_supports_tls(s_string_view ip_view);
bool constains_abba(s_string_view view);

bool ip_supports_ssl(s_string_view ip_view);
void gather_triplets(s_string_view view, c_triplet_set* inout_aba_set);

void execute_2016_day7_part1(FILE* file);
void execute_2016_day7_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day7_exercise
{
	2016,
	7,
	"Internet Protocol Version 7",
	execute_2016_day7_part1,
	execute_2016_day7_part2
};

//-------------- definitions

void execute_2016_day7_part1(FILE* file)
{
	c_medium_string line_buffer;
	int32 result = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

		s_string_view ip_view(line_buffer.get_string(), line_buffer.length());

		if (ip_supports_tls(ip_view))
		{
			result++;
		}
	}

	n_log::output("{} IPs support TLS\n", result);
}

void execute_2016_day7_part2(FILE* file)
{
	c_medium_string line_buffer;
	int32 result = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

		s_string_view ip_view(line_buffer.get_string(), line_buffer.length());

		if (ip_supports_ssl(ip_view))
		{
			result++;
		}
	}

	n_log::output("{} IPs support SSL\n", result);
}

bool ip_supports_tls(s_string_view ip_view)
{
	int32 view_start_index = 0;
	bool has_outer_abba = false;
	bool has_inner_abba = false;

	for (int32 char_index = 0; char_index < ip_view.length; char_index++)
	{
		char current_char = ip_view.string[char_index];

		if (current_char == ']')
		{
			int32 inner_length = char_index - view_start_index;
			if (inner_length >= 4)
			{
				s_string_view inner_view(ip_view.string + view_start_index, inner_length);
				if (constains_abba(inner_view))
				{
					has_inner_abba = true;
					break;
				}
			}

			view_start_index = char_index + 1;
		}
		else if (current_char == '[')
		{
			int32 outer_length = char_index - view_start_index;
			if (outer_length >= 4)
			{
				s_string_view outer_view(ip_view.string + view_start_index, outer_length);
				has_outer_abba = has_outer_abba || constains_abba(outer_view);
			}

			view_start_index = char_index + 1;
		}
		else if (char_index == ip_view.length - 1)
		{
			int32 outer_length = char_index - view_start_index + 1;
			if (outer_length >= 4)
			{
				s_string_view outer_view(ip_view.string + view_start_index, outer_length);
				has_outer_abba = has_outer_abba || constains_abba(outer_view);
			}
		}
	}

	return has_outer_abba && !has_inner_abba;
}

bool constains_abba(s_string_view view)
{
	bool result = false;

	for (int32 char_index = 3; char_index < view.length; char_index++)
	{
		if (view.string[char_index - 3] == view.string[char_index] &&
			view.string[char_index - 2] == view.string[char_index - 1] &&
			view.string[char_index] != view.string[char_index - 1])
		{
			result = true;
			break;
		}
	}

	return result;
}

bool ip_supports_ssl(s_string_view ip_view)
{
	int32 result = false;
	c_triplet_set aba_set;
	c_triplet_set bab_set;
	int32 view_start_index = 0;

	for (int32 char_index = 0; char_index < ip_view.length; char_index++)
	{
		char current_char = ip_view.string[char_index];

		if (current_char == ']')
		{
			int32 inner_length = char_index - view_start_index;
			if (inner_length >= 3)
			{
				s_string_view inner_view(ip_view.string + view_start_index, inner_length);
				gather_triplets(inner_view, &bab_set);
			}

			view_start_index = char_index + 1;
		}
		else if (current_char == '[')
		{
			int32 outer_length = char_index - view_start_index;
			if (outer_length >= 3)
			{
				s_string_view outer_view(ip_view.string + view_start_index, outer_length);
				gather_triplets(outer_view, &aba_set);
			}

			view_start_index = char_index + 1;
		}
		else if (char_index == ip_view.length - 1)
		{
			int32 outer_length = char_index - view_start_index + 1;
			if (outer_length >= 3)
			{
				s_string_view outer_view(ip_view.string + view_start_index, outer_length);
				gather_triplets(outer_view, &aba_set);
			}
		}
	}

	for (int32 aba_index = 0; aba_index < aba_set.get_size(); aba_index++)
	{
		c_triplet aba = aba_set[aba_index];
		c_triplet bab;
		bab.append_char(aba.at(1));
		bab.append_char(aba.at(0));
		bab.append_char(aba.at(1));

		if (bab_set.contains(bab))
		{
			result = true;
			break;
		}
	}

	return result;
}

void gather_triplets(s_string_view view, c_triplet_set* inout_triplet_set)
{
	for (int32 char_index = 2; char_index < view.length; char_index++)
	{
		if (view.string[char_index - 2] == view.string[char_index] &&
			view.string[char_index - 1] != view.string[char_index])
		{
			c_triplet new_triplet(view.string + char_index - 2, 3);

			inout_triplet_set->add(new_triplet);
		}
	}
}
