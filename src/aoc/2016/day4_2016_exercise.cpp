
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/utilities/sort.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

bool is_room_real(const s_string_view& name, const s_string_view& checksum);
c_small_string decrypt_room_name(const s_string_view& name, uint64 sector_id);

void execute_2016_day4_part1(FILE* file);
void execute_2016_day4_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day4_exercise
{
	2016,
	4,
	"Security Through Obscurity",
	execute_2016_day4_part1,
	execute_2016_day4_part2
};

//-------------- definitions

void execute_2016_day4_part1(FILE* file)
{
	c_medium_string line_buffer;
	uint64 sector_id_sum = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

		int32 last_hyphen_index = n_array::last_index_of(line_buffer.get_string(), line_buffer.length(), '-');
		int32 first_bracket_index = n_array::last_index_of(line_buffer.get_string(), line_buffer.length(), '[');

		s_string_view room_name_view(line_buffer.get_string(), last_hyphen_index);
		s_string_view sector_id_view(line_buffer.get_string() + last_hyphen_index + 1, first_bracket_index - last_hyphen_index + 1);
		s_string_view checksum_view(line_buffer.get_string() + first_bracket_index + 1, 5);

		if (is_room_real(room_name_view, checksum_view))
		{
			uint64 sector_id = n_string::to_unsigned_integer(sector_id_view);

			sector_id_sum += sector_id;
		}
	}

	std::cout << "The sum of all sector ids with valid rooms is " << sector_id_sum << std::endl;
}

void execute_2016_day4_part2(FILE* file)
{
	c_medium_string line_buffer;
	c_small_string target_room_name;
	uint64 north_pole_sector_id = 0;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), line_buffer.k_capacity, file);

		int32 last_hyphen_index = n_array::last_index_of(line_buffer.get_string(), line_buffer.length(), '-');
		int32 first_bracket_index = n_array::last_index_of(line_buffer.get_string(), line_buffer.length(), '[');

		s_string_view room_name_view(line_buffer.get_string(), last_hyphen_index);
		s_string_view sector_id_view(line_buffer.get_string() + last_hyphen_index + 1, first_bracket_index - last_hyphen_index + 1);
		s_string_view checksum_view(line_buffer.get_string() + first_bracket_index + 1, 5);

		if (is_room_real(room_name_view, checksum_view))
		{
			uint64 sector_id = n_string::to_unsigned_integer(sector_id_view);
			c_small_string decrypted_name = decrypt_room_name(room_name_view, sector_id);

			if (n_string::index_of(decrypted_name.get_string(), "object") != NONE)
			{
				target_room_name.set(decrypted_name.get_string());
				north_pole_sector_id = sector_id;
				break;
			}
		}
	}

	std::cout << "Found room \"" << target_room_name.get_string() << "\" with sector id " << north_pole_sector_id << std::endl;
}

bool is_room_real(const s_string_view& name, const s_string_view& checksum)
{
	struct s_letter_count
	{
		s_letter_count() : count(0), letter('a') {}

		uint8 count;
		char letter;
	};

	c_static_array<s_letter_count, 26> counts;
	for (int32 letter_index = 0; letter_index < counts.get_size(); letter_index++)
	{
		counts[letter_index].letter = 'a' + static_cast<char>(letter_index);
	}

	for (int32 name_char_index = 0; name_char_index < name.length; name_char_index++)
	{
		if (name.string[name_char_index] != '-')
		{
			int32 letter_index = name.string[name_char_index] - 'a';
			counts[letter_index].count++;
		}
	}

	insertion_sort(
		counts.get_storage(), counts.get_size(),
		[](const s_letter_count& left, const s_letter_count& right) {
			return left.count > right.count ||
				(left.count == right.count && left.letter <= right.letter);
		});

	bool checksum_matches = true;
	for (int32 checksum_char_index = 0; checksum_char_index < checksum.length; checksum_char_index++)
	{
		if (checksum.string[checksum_char_index] != counts[checksum_char_index].letter)
		{
			checksum_matches = false;
			break;
		}
	}

	return checksum_matches;
}

c_small_string decrypt_room_name(const s_string_view& name, uint64 sector_id)
{
	c_small_string decrypted_name;

	for (int32 char_index = 0; char_index < name.length; char_index++)
	{
		if (name.string[char_index] == '-')
		{
			decrypted_name.append_char(' ');
		}
		else
		{
			int32 letter_index = name.string[char_index] - 'a';
			int32 decrypted_index = (letter_index + sector_id) % 26;
			char decrypted_letter = static_cast<char>(decrypted_index + 'a');
			
			decrypted_name.append_char(decrypted_letter);
		}
	}

	return decrypted_name;
}