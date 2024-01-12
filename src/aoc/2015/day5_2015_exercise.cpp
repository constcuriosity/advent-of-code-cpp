
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"

#include <iostream>

//-------------- prototypes

void execute_day5_part_one(FILE* file);
void execute_day5_part_two(FILE* file);

static_function bool is_nice_string_v1(const c_small_string& string);
static_function bool is_nice_string_v2(const c_small_string& string);

static_function bool is_vowel(char character);
static_function bool is_banned_sequence(char first_char, char second_char);

//-------------- globals

extern s_aoc_exercise_data day5
{
	5,
	"Doesn't He Have Intern-Elves For This?",
	execute_day5_part_one,
	execute_day5_part_two
};

//-------------- public implementations

void execute_day5_part_one(FILE* file)
{
	c_small_string line_buffer;
	int32 nice_count = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_small_string::k_capacity, file);

		if (is_nice_string_v1(line_buffer)) { nice_count++; }
	}

	std::cout << "There are " << nice_count << " nice strings in the file using the first ruleset." << std::endl << std::endl;
}

void execute_day5_part_two(FILE* file)
{
	c_small_string line_buffer;
	int32 nice_count = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_small_string::k_capacity, file);

		if (is_nice_string_v2(line_buffer)) { nice_count++; }
	}

	std::cout << "There are " << nice_count << " nice strings in the file using the second ruleset." << std::endl << std::endl;
}

//-------------- private implementations

static_function bool is_nice_string_v1(const c_small_string& string)
{
	char previous_char = '\0';
	int32 vowel_count = 0;
	bool has_letter_chain = false;
	bool has_banned_sequence = false;

	uint32 line_length = string.length();
	for (int32 char_index = 0; char_index < line_length && !has_banned_sequence; char_index++)
	{
		char current_char = string.at(char_index);

		if (is_vowel(current_char)) { vowel_count++; }
		has_letter_chain |= (previous_char == current_char);
		has_banned_sequence = is_banned_sequence(previous_char, current_char);

		previous_char = current_char;
	}
	
	return !has_banned_sequence && has_letter_chain && vowel_count >= 3;
}

static_function bool is_nice_string_v2(const c_small_string& string)
{
	char first_char = '\0';
	char second_char = '\0';

	bool character_mirrored = false;
	bool pair_found = false;

	uint32 line_length = string.length();
	for (int32 char_index = 0; char_index < line_length; char_index++)
	{
		if (char_index >= 2)
		{
			character_mirrored |= string.at(char_index) == string.at(char_index - 2);
		}

		for (int32 reverse_index = char_index - 3; reverse_index >= 0; reverse_index--)
		{
			pair_found |= (string.at(reverse_index) == string.at(char_index - 1) &&
				string.at(reverse_index + 1) == string.at(char_index));
		}
	}

	return pair_found && character_mirrored;
}

static_function bool is_vowel(char character)
{
	return character == 'a' ||
		character == 'e' ||
		character == 'i' ||
		character == 'o' ||
		character == 'u';
}

static_function bool is_banned_sequence(char first_char, char second_char)
{
	return (first_char == 'a' && second_char == 'b') ||
		(first_char == 'c' && second_char == 'd') ||
		(first_char == 'p' && second_char == 'q') ||
		(first_char == 'x' && second_char == 'y');
}
