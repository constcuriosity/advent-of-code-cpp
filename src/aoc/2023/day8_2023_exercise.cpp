//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/dynamic/dynamic_set.h"
#include "foundation/math/int_math.h"
#include "foundation/utilities/sort.h"
#include "foundation/utilities/wyhash.h"

#include <iostream>
#include <unordered_map>

//-------------- constants

//-------------- prototypes

struct s_node
{
	s_node() : left_node(0), right_node(0) {}

	int32 left_node;
	int32 right_node;
};

typedef std::unordered_map<int32, s_node, c_wyhash<int32>> c_node_map;

int32 convert_to_id(s_string_view view);
int32 convert_to_id(const char *string);
bool id_ends_with_a(int32 node_id);
bool id_ends_with_z(int32 node_id);
bool ids_end_with_z(c_array_view<int32> ids);

void execute_2023_day8_part1(FILE* file);
void execute_2023_day8_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2023_day8_exercise
{
	2023,
	8,
	"Haunted Wasteland",
	execute_2023_day8_part1,
	execute_2023_day8_part2
};

//-------------- definitions

void execute_2023_day8_part1(FILE* file)
{
	c_kilo_string instruction_buffer;
	fgets(instruction_buffer.get_string(), instruction_buffer.k_capacity, file);

	c_medium_string node_buffer;
	fgets(node_buffer.get_string(), node_buffer.k_capacity, file); // Advance past empty line

	c_node_map node_map;

	while (feof(file) == 0)
	{
		s_node node;
		fgets(node_buffer.get_string(), node_buffer.k_capacity, file);

		c_utf8_tokenizer node_tokenizer(node_buffer.get_string(), " =(),\n\r");
		int32 node_key = convert_to_id(node_tokenizer.get_token());
		node_tokenizer.advance();
		node.left_node = convert_to_id(node_tokenizer.get_token());
		node_tokenizer.advance();
		node.right_node = convert_to_id(node_tokenizer.get_token());

		node_map[node_key] = node;
	}

	int32 current_key = convert_to_id("AAA");
	int32 target_key = convert_to_id("ZZZ");
	int32 steps = 0;

	c_utf8_character_iterator instruction_iter(instruction_buffer.get_string());

	while (current_key != target_key)
	{
		s_node current_node = node_map[current_key];
		char current_char = static_cast<char>(instruction_iter.get_codepoint());

		if (current_char == 'L')
		{
			current_key = current_node.left_node;
			steps++;
		}
		else if (current_char == 'R')
		{
			current_key = current_node.right_node;
			steps++;
		}
		else
		{
			// Filter out trailing new line characters
		}

		assert(current_key != 0);

		if (instruction_iter.can_advance())
		{
			instruction_iter.advance();
		}
		else
		{
			instruction_iter.set_current(instruction_buffer.get_string());
		}
	}

	std::cout << "It took " << steps << " steps to travel from node AAA to ZZZ." << std::endl;
}

void execute_2023_day8_part2(FILE* file)
{
	c_kilo_string instruction_buffer;
	fgets(instruction_buffer.get_string(), instruction_buffer.k_capacity, file);

	c_medium_string node_buffer;
	fgets(node_buffer.get_string(), node_buffer.k_capacity, file); // Advance past empty line

	c_node_map node_map;

	while (feof(file) == 0)
	{
		s_node node;
		fgets(node_buffer.get_string(), node_buffer.k_capacity, file);

		c_utf8_tokenizer node_tokenizer(node_buffer.get_string(), " =(),\n\r");
		int32 node_key = convert_to_id(node_tokenizer.get_token());

		node_tokenizer.advance();
		node.left_node = convert_to_id(node_tokenizer.get_token());
		node_tokenizer.advance();
		node.right_node = convert_to_id(node_tokenizer.get_token());

		node_map[node_key] = node;
	}

	c_dynamic_vector<int32> node_ids;

	for (c_node_map::const_iterator node_iter = node_map.begin();
		node_iter != node_map.end();
		node_iter++)
	{
		if (id_ends_with_a(node_iter->first))
		{
			node_ids.push(node_iter->first);
		}
	}

	uint64 steps = 0;

	c_utf8_character_iterator instruction_iter(instruction_buffer.get_string());

	while (!ids_end_with_z(node_ids.view()))
	{
		char current_char = static_cast<char>(instruction_iter.get_codepoint());
		bool is_instruction_char = current_char == 'L' || current_char == 'R';

		for (int32 id_index = 0; id_index < node_ids.get_size() && is_instruction_char; id_index++)
		{
			s_node current_node = node_map[node_ids[id_index]];

			if (current_char == 'L')
			{
				node_ids[id_index] = current_node.left_node;
			}
			else if (current_char == 'R')
			{
				node_ids[id_index] = current_node.right_node;
			}
		}

		if (is_instruction_char)
		{
			steps++;
		}

		if (instruction_iter.can_advance())
		{
			instruction_iter.advance();
		}
		else
		{
			instruction_iter.set_current(instruction_buffer.get_string());
		}
	}

	std::cout << "It took " << steps << " steps for all ids ending with A to now end with Z." << std::endl;
}

int32 convert_to_id(s_string_view view)
{
	int32 result = 0;
	for (int32 char_index = 0; char_index < view.length; char_index++)
	{
		result |= static_cast<int32>(view.string[char_index]) << (8 * char_index);
	}

	return result;
}

int32 convert_to_id(const char* string)
{
	int32 result = 0;
	for (int32 char_index = 0; string[char_index] != '\0'; char_index++)
	{
		result |= static_cast<int32>(string[char_index]) << (8 * char_index);
	}

	return result;
}

bool id_ends_with_a(int32 node_id)
{
	return (node_id >> 16) == 'A';
}

bool id_ends_with_z(int32 node_id)
{
	return static_cast<char>(node_id >> 16) == 'Z';
}

bool ids_end_with_z(c_array_view<int32> ids)
{
	bool result = true;
	for (int32 index = 0; index < ids.get_size(); index++)
	{
		if (!id_ends_with_z(ids[index]))
		{
			result = false;
		}
		else
		{
			while (0) { void; }
		}
	}

	return result;
	/*return ids.every([](int32 id)
	{
		return id_ends_with_z(id);
	});*/
}