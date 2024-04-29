
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/dynamic/dynamic_vector.h"
#include "foundation/containers/dynamic/string.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/containers/view/string_view.h"
#include "foundation/io/log.h"
#include "foundation/memory/memory_utilities.h"

//-------------- constants

//-------------- prototypes

struct s_marker
{
	int32 repeat_count;
	int32 last_index;
};

int32 parse_marker(const utf8* string, int32* out_length, int32* out_count);

void execute_2016_day9_part1(FILE* file);
void execute_2016_day9_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day9_exercise
{
	2016,
	9,
	"Explosives in Cyberspace",
	execute_2016_day9_part1,
	execute_2016_day9_part2
};

//-------------- definitions

void execute_2016_day9_part1(FILE* file)
{
	c_string decompressed_buffer;

	typedef c_static_string<20 * k_kilobyte> c_file_string;
	
	// $TODO Wow, it really makes you think I should override new, doesn't it
	c_file_string *buffer = c_memory_utilities::allocate<c_file_string>();
	new(buffer) c_file_string();

	// Make sure that we load the whole file into memory
	fgets(buffer->get_string(), buffer->k_capacity, file);
	assert(feof(file) != 0);

	int32 uncompressed_index = 0;

	int32 buffer_length = buffer->length();
	for (int32 buffer_index = 0; buffer_index < buffer_length; buffer_index++)
	{
		char current_char = buffer->at(buffer_index);

		// Look for start of marker
		if (current_char == '(')
		{
			// $TODO Better functions for getting a string view from an existing string

			// When found, push the uncompressed section we've been travelling into the output buffer
			decompressed_buffer.append(buffer->get_string() + uncompressed_index, buffer_index - uncompressed_index);

			int32 repeat_length = 0;
			int32 repeat_count = 0;
			int32 marker_length = parse_marker(buffer->get_string() + buffer_index, &repeat_length, &repeat_count);

			// Make a view of the buffer of the segment to repeat
			int32 repeat_start_index = buffer_index + marker_length;
			int32 repeat_end_index = n_math::min(repeat_start_index + repeat_length, buffer_length);
			c_string_view repeat_view(buffer->get_string() + repeat_start_index, repeat_end_index - repeat_start_index);

			// Append the repeat view as many times as requested
			for (int32 repeat_index = 0; repeat_index < repeat_count; repeat_index++)
			{
				decompressed_buffer.append(repeat_view.get_string(), repeat_view.get_size());
			}

			// minus one to account for the increment in the for loop
			buffer_index += marker_length + repeat_length - 1;
			uncompressed_index = buffer_index + 1;
		}
		else if (buffer_index == (buffer_length - 1))
		{
			// Copy over the last remaining uncompressed part
			decompressed_buffer.append(buffer->get_string() + uncompressed_index, buffer_index + 1 - uncompressed_index);
		}
	}

	n_log::output("Decompressed length of the file with version 1 compression is {}\n", decompressed_buffer.length());

	buffer->~c_file_string();
	c_memory_utilities::free(buffer);
}

void execute_2016_day9_part2(FILE* file)
{
	typedef c_static_string<20 * k_kilobyte> c_file_string;

	// $TODO Wow, it really makes you think I should override new, doesn't it
	c_file_string* buffer = c_memory_utilities::allocate<c_file_string>();
	new(buffer) c_file_string();

	// Make sure that we load the whole file into memory
	fgets(buffer->get_string(), buffer->k_capacity, file);
	assert(feof(file) != 0);

	int64 decompressed_length = 0;
	int32 cached_multiplication_factor = 1;
	c_dynamic_vector<s_marker> marker_stack;

	int32 buffer_length = buffer->length();
	for (int32 buffer_index = 0; buffer_index < buffer_length; buffer_index++)
	{
		while (!marker_stack.is_empty() &&
			marker_stack.back().last_index <= buffer_index)
		{
			marker_stack.pop();

			int32 repeat_factor = 1;
			marker_stack.each([&](const s_marker& m) { repeat_factor *= m.repeat_count; });
			cached_multiplication_factor = repeat_factor;
		}

		char current_char = buffer->at(buffer_index);

		if (current_char == '(')
		{
			// Process marker
			int32 repeat_length = 0;
			int32 repeat_count = 0;
			int32 marker_length = parse_marker(buffer->get_string() + buffer_index, &repeat_length, &repeat_count);

			s_marker new_marker;
			new_marker.repeat_count = repeat_count;
			new_marker.last_index = buffer_index + marker_length + repeat_length;
			marker_stack.push(new_marker);

			int32 repeat_factor = 1;
			marker_stack.each([&](const s_marker& m) { repeat_factor *= m.repeat_count; });
			cached_multiplication_factor = repeat_factor;

			// minus one to account for the increment in the for loop
			buffer_index += marker_length - 1;
		}
		else
		{
			decompressed_length += cached_multiplication_factor;
		}
	}

	n_log::output("Decompressed length of the file with version 2 compression is {}\n", decompressed_length);

	buffer->~c_file_string();
	c_memory_utilities::free(buffer);
}

int32 parse_marker(const utf8* string, int32* out_length, int32* out_count)
{
	assert(out_length);
	assert(out_count);

	int32 marker_index = 0;
	char current_char = string[0];

	assert(current_char == '(');

	for (; current_char != ')'; marker_index++)
	{
		current_char = string[marker_index];
	}
	int32 marker_length = marker_index;

	// Make a view of marker and extract the repeat length and count
	c_utf8_tokenizer marker_tokens(string, "()x", marker_length);
	c_string_view repeat_length_token = marker_tokens.get_token();
	marker_tokens.advance();
	c_string_view repeat_count_token = marker_tokens.get_token();

	*out_length = static_cast<int32>(repeat_length_token.to_integer());
	*out_count = static_cast<int32>(repeat_count_token.to_integer());

	return marker_length;
}
