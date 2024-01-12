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

int32 convert_to_id(s_string_view view);
int32 convert_to_id(const char *string);

void execute_day8_part_one(FILE* file);
void execute_day8_part_two(FILE* file);

//-------------- globals

extern s_aoc_exercise_data day8
{
	8,
	"Haunted Wasteland",
	execute_day8_part_one,
	execute_day8_part_two
};

//-------------- definitions

void execute_day8_part_one(FILE* file)
{
	c_kilo_string instruction_buffer;
	fgets(instruction_buffer.get_string(), instruction_buffer.k_capacity, file);

	c_medium_string node_buffer;
	fgets(node_buffer.get_string(), node_buffer.k_capacity, file); // Advance past empty line

	std::unordered_map<int32, s_node, c_wyhash<int32>> node_map;

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

void execute_day8_part_two(FILE* file)
{
	std::cout << "Day 8 Question 2" << std::endl;
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