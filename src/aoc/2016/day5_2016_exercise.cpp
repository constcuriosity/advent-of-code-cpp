
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/utilities/md5_hash.h"
#include "foundation/memory/memory_utilities.h"

#include <iostream>

//-------------- constants

static_const int32 k_password_length = 8;

//-------------- prototypes

void execute_2016_day5_part1(FILE* file);
void execute_2016_day5_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day5_exercise
{
	2016,
	5,
	"How About a Nice Game of Chess?",
	execute_2016_day5_part1,
	execute_2016_day5_part2
};

//-------------- definitions

void execute_2016_day5_part1(FILE* file)
{
	c_medium_string line_buffer;
	fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

	c_small_string password;

	for (uint64 increment = 0; password.length() < k_password_length; increment++)
	{
		c_small_string combo;
		combo.print("%s%llu", line_buffer.get_string(), increment);

		s_md5_digest hash_digest = c_md5_hash::hash(combo.get_string());

		if (hash_digest.digest[0] == 0x00 &&
			hash_digest.digest[1] == 0x00 &&
			hash_digest.digest[2] <= 0x0F)
		{
			uint32 remainder = hash_digest.digest[2];
			if (remainder < 10)
			{
				password.append_char('0' + remainder);
			}
			else
			{
				password.append_char('a' + (remainder - 10));
			}
		}
	}

	std::cout << "The password for the security door is " << password.get_string() << std::endl;
}

void execute_2016_day5_part2(FILE* file)
{
	c_medium_string line_buffer;
	fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

	char password[k_password_length + 1];
	c_memory_utilities::zero(password, k_password_length + 1);
	uint32 increment = 0;

	while (true)
	{
		c_small_string combo;
		combo.print("%s%llu", line_buffer.get_string(), increment);

		s_md5_digest hash_digest = c_md5_hash::hash(combo.get_string());

		if (hash_digest.digest[0] == 0x00 &&
			hash_digest.digest[1] == 0x00 &&
			hash_digest.digest[2] < k_password_length &&
			password[hash_digest.digest[2]] == '\0')
		{
			int32 password_index = hash_digest.digest[2];

			uint32 remainder = hash_digest.digest[3] >> 4;
			if (remainder < 10)
			{
				password[hash_digest.digest[2]] = '0' + remainder;
			}
			else
			{
				password[hash_digest.digest[2]] = 'a' + (remainder - 10);
			}

			bool password_filled = true;
			for (int32 char_index = 0; char_index < k_password_length; char_index++)
			{
				if (password[char_index] == '\0')
				{
					password_filled = false;
				}
			}

			if (password_filled)
			{
				break;
			}
		}

		increment++;
	}

	std::cout << "The password for the security door with the more complicated security is " << password << std::endl;
}
