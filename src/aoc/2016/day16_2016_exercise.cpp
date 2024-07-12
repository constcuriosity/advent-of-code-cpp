
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/memory/memory_utilities.h"
#include "foundation/io/log.h"

//-------------- constants

//-------------- prototypes

void execute_2016_day16_part1(FILE* file);
void execute_2016_day16_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day16_exercise
{
	2016,
	16,
	"Dragon Checksum",
	execute_2016_day16_part1,
	execute_2016_day16_part2
};

//-------------- definitions

void execute_2016_day16_part1(FILE* file)
{
	typedef c_static_string<600> c_exercise_string;
	const int32 k_target_disc_length = 272;

	c_exercise_string line1;
	c_exercise_string line2;

	fgets(line1.get_string(), line1.k_capacity, file);
	c_exercise_string* current_data= &line1;
	c_exercise_string* prev_data= &line2;

	// Build up data
	while (current_data->length() < k_target_disc_length)
	{
		c_exercise_string* temp = prev_data;
		prev_data = current_data;
		current_data = temp;

		current_data->set(prev_data->get_string());
		current_data->append_char('0');

		int32 prev_data_size = prev_data->length();
		n_string::reverse_ascii(prev_data->get_string(), prev_data_size);

		for (int32 flip_index = 0; flip_index < prev_data_size; flip_index++)
		{
			if ((*prev_data)[flip_index] == '0')
			{
				(*prev_data)[flip_index] = '1';
			}
			else
			{
				(*prev_data)[flip_index] = '0';
			}
		}

		current_data->append(prev_data->get_string(), prev_data->length());
	}

	// Generate checksum
	int32 current_data_size = k_target_disc_length;
	while (current_data_size % 2 == 0)
	{
		current_data_size = current_data_size >> 1;

		c_exercise_string* temp = prev_data;
		prev_data = current_data;
		current_data = temp;

		current_data->clear();
		for (int32 compress_index = 0; compress_index < current_data_size; compress_index++)
		{
			bool pair_matches = prev_data->at(compress_index * 2) == prev_data->at(compress_index * 2 + 1);
			(*current_data)[compress_index] = pair_matches ? '1' : '0';
		}

		(*current_data)[current_data_size] = '\0';
	}

	n_log::output("The checksum of the first disk to fill is {}\n", current_data->get_string());
}

void execute_2016_day16_part2(FILE* file)
{
	const int32 k_target_disc_length = 35651584;
	typedef c_static_string<35651584 * 2> c_exercise_string;

	// $NOTE I really need to get around overriding the new operator...
	c_exercise_string *line1 = n_memory::allocate<c_exercise_string>();
	c_exercise_string *line2 = n_memory::allocate<c_exercise_string>();
	new (line1) c_exercise_string();
	new (line2) c_exercise_string();

	fgets(line1->get_string(), line1->k_capacity, file);
	c_exercise_string* current_data = line1;
	c_exercise_string* prev_data = line2;

	// Build up data
	while (current_data->length() < k_target_disc_length)
	{
		c_exercise_string* temp = prev_data;
		prev_data = current_data;
		current_data = temp;

		current_data->set(prev_data->get_string());
		current_data->append_char('0');

		int32 prev_data_size = prev_data->length();
		n_string::reverse_ascii(prev_data->get_string(), prev_data_size);

		for (int32 flip_index = 0; flip_index < prev_data_size; flip_index++)
		{
			if ((*prev_data)[flip_index] == '0')
			{
				(*prev_data)[flip_index] = '1';
			}
			else
			{
				(*prev_data)[flip_index] = '0';
			}
		}

		current_data->append(prev_data->get_string(), prev_data->length());
	}

	// Generate checksum
	int32 current_data_size = k_target_disc_length;
	while (current_data_size % 2 == 0)
	{
		current_data_size = current_data_size >> 1;

		c_exercise_string* temp = prev_data;
		prev_data = current_data;
		current_data = temp;

		current_data->clear();
		for (int32 compress_index = 0; compress_index < current_data_size; compress_index++)
		{
			bool pair_matches = prev_data->at(compress_index * 2) == prev_data->at(compress_index * 2 + 1);
			(*current_data)[compress_index] = pair_matches ? '1' : '0';
		}

		(*current_data)[current_data_size] = '\0';
	}

	n_log::output("The checksum of the second disk to fill is {}\n", current_data->get_string());

	n_memory::free(line1);
	n_memory::free(line2);
}
