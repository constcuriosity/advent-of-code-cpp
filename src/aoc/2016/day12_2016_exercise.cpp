
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/dynamic/dynamic_vector.h"
#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/containers/view/string_view.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

enum e_instruction_type
{
	_instruction_type_none,
	_instruction_type_copy,
	_instruction_type_increment,
	_instruction_type_decrement,
	_instruction_type_jump,

	k_instruction_type
};

enum e_argument_type
{
	_argument_type_none,
	_argument_type_constant,
	_argument_type_register,

	k_argument_type
};

struct s_argument
{
	s_argument() : type(_argument_type_none), value(0) {}

	e_argument_type type;
	int32 value;
};

struct s_instruction
{
	s_instruction() : type(_instruction_type_none) {}

	e_instruction_type type;
	s_argument arg_1;
	s_argument arg_2;
};

typedef c_dynamic_vector<s_instruction> c_instruction_vector;

class c_computer
{
public:
	c_computer(const c_instruction_vector *instructions) : m_instructions(instructions), m_values(0) {}

	void execute()
	{
		while (VALID_INDEX(m_instruction_index, m_instructions->get_size()))
		{
			const s_instruction& current_instruction = m_instructions->at(m_instruction_index);

			if (current_instruction.type == _instruction_type_copy)
			{
				int32 target_register = current_instruction.arg_2.value;
				if (current_instruction.arg_1.type == _argument_type_constant)
				{
					m_values[target_register] = current_instruction.arg_1.value;
				}
				else
				{
					m_values[target_register] = m_values[current_instruction.arg_1.value];
				}

				m_instruction_index++;
			}
			else if (current_instruction.type == _instruction_type_increment)
			{
				m_values[current_instruction.arg_1.value]++;
				m_instruction_index++;
			}
			else if (current_instruction.type == _instruction_type_decrement)
			{
				m_values[current_instruction.arg_1.value]--;
				m_instruction_index++;
			}
			else if (current_instruction.type == _instruction_type_jump)
			{
				int64 jump_value = current_instruction.arg_1.type == _argument_type_constant
					? current_instruction.arg_1.value
					: m_values[current_instruction.arg_1.value];

				if (jump_value != 0)
				{
					m_instruction_index += current_instruction.arg_2.value;
				}
				else
				{
					m_instruction_index++;
				}
			}
		}
	}

	void set_register_value(char reg, int64 value) { m_values[reg - 'a'] = value; }
	int64 get_register_value(char reg) const { return m_values[reg - 'a']; }

private:
	const c_instruction_vector* m_instructions;
	c_static_array<int32, 10> m_values;
	int32 m_instruction_index = 0;
};

static_function void build_instruction_vector(FILE* file, c_instruction_vector* out_instructions);

void execute_2016_day12_part1(FILE* file);
void execute_2016_day12_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day12_exercise
{
	2016,
	12,
	"Leonardo's Monorail",
	execute_2016_day12_part1,
	execute_2016_day12_part2
};

//-------------- definitions

void execute_2016_day12_part1(FILE* file)
{
	c_instruction_vector instruction_vector;
	build_instruction_vector(file, &instruction_vector);

	c_computer computer(&instruction_vector);

	computer.execute();

	std::cout << "The value within register a is " << computer.get_register_value('a') << std::endl;
}

void execute_2016_day12_part2(FILE* file)
{
	c_instruction_vector instruction_vector;
	build_instruction_vector(file, &instruction_vector);

	c_computer computer(&instruction_vector);
	computer.set_register_value('c', 1);

	computer.execute();

	std::cout << "The value within register a is when register c is initialized to 1 is " << computer.get_register_value('a') << std::endl;
}

static_function void build_instruction_vector(FILE* file, c_instruction_vector* out_instructions)
{
	c_medium_string line_buffer;
	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);
		c_utf8_tokenizer tokens(line_buffer.get_string(), " \n");

		s_instruction new_instruction;

		c_string_view instruction_token = tokens.get_token();
		if (instruction_token.equals("cpy"))
		{
			new_instruction.type = _instruction_type_copy;

			tokens.advance();
			c_string_view arg_1_token = tokens.get_token();

			if (arg_1_token.is_number())
			{
				new_instruction.arg_1.type = _argument_type_constant;
				new_instruction.arg_1.value = static_cast<int32>(arg_1_token.to_integer());
			}
			else
			{
				new_instruction.arg_1.type = _argument_type_register;
				new_instruction.arg_1.value = arg_1_token.at(0) - 'a';
			}

			tokens.advance();
			c_string_view arg_2_token = tokens.get_token();
			new_instruction.arg_2.type = _argument_type_register;
			new_instruction.arg_2.value = arg_2_token.at(0) - 'a';
		}
		else if (instruction_token.equals("inc"))
		{
			new_instruction.type = _instruction_type_increment;
			
			tokens.advance();
			c_string_view arg_1_token = tokens.get_token();

			assert(arg_1_token.length == 1);

			new_instruction.arg_1.type = _argument_type_register;
			new_instruction.arg_1.value = arg_1_token.at(0) - 'a';
		}
		else if (instruction_token.equals("dec"))
		{
			new_instruction.type = _instruction_type_decrement;

			tokens.advance();
			c_string_view arg_1_token = tokens.get_token();

			assert(arg_1_token.length == 1);

			new_instruction.arg_1.type = _argument_type_register;
			new_instruction.arg_1.value = arg_1_token.at(0) - 'a';
		}
		else if (instruction_token.equals("jnz"))
		{
			new_instruction.type = _instruction_type_jump;

			tokens.advance();
			c_string_view arg_1_token = tokens.get_token();

			if (arg_1_token.is_number())
			{
				new_instruction.arg_1.type = _argument_type_constant;
				new_instruction.arg_1.value = static_cast<int32>(arg_1_token.to_integer());
			}
			else
			{
				new_instruction.arg_1.type = _argument_type_register;
				new_instruction.arg_1.value = arg_1_token.at(0) - 'a';
			}

			tokens.advance();
			c_string_view arg_2_token = tokens.get_token();
			new_instruction.arg_2.type = _argument_type_constant;
			new_instruction.arg_2.value = static_cast<int32>(arg_2_token.to_integer());
		}

		if (new_instruction.type != _instruction_type_none)
		{
			out_instructions->push(new_instruction);
		}
	}
}
