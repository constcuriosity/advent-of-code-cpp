
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include <iostream>
#include <set>

//-------------- constants

const int32 k_line_size = k_kilobyte * 128;

//-------------- prototypes

void execute_day3_part_one(FILE* file);
void execute_day3_part_two(FILE* file);

//-------------- globals

extern s_aoc_exercise_data day3
{
	3,
	"Perfectly Spherical Houses in a Vacuum",
	execute_day3_part_one,
	execute_day3_part_two
};

//-------------- private definitions

// $TODO Move this into the foundation's math folder
// Maybe make it templatized to support either int32, int64, real32
struct s_point2
{
	int32 x;
	int32 y;
};

bool operator <(const s_point2& lhs, const s_point2& rhs)
{
	return lhs.x < rhs.x
		? true
		: lhs.x == rhs.x
			? lhs.y < rhs.y
			: false;
}

//-------------- public implementations

void execute_day3_part_one(FILE* file)
{
	// $TODO This should be able to allocate the correct amount of space for the line
	char* line = static_cast<char*>(malloc(k_line_size));
	fgets(line, k_line_size, file);

	std::set<s_point2> points;
	int32 char_index = 0;
	s_point2 current_position = { 0, 0 };

	points.insert(current_position);

	for (; line[char_index] != '\0'; char_index++)
	{
		char current_char = line[char_index];

		switch (current_char)
		{
		case '^':
		{
			current_position.y += 1;
			break;
		}
		case 'v':
		{
			current_position.y -= 1;
			break;
		}
		case '>':
		{
			current_position.x += 1;
			break;
		}
		case '<':
		{
			current_position.x -= 1;
			break;
		}
		}

		points.insert(current_position);
	}

	std::cout << "Santa has visited " << points.size() << " unique houses" << std::endl;

	free(line);
}

void execute_day3_part_two(FILE* file)
{
	// $TODO This should be able to allocate the correct amount of space for the line
	char* line = static_cast<char*>(malloc(k_line_size));
	fgets(line, k_line_size, file);

	std::set<s_point2> points;
	int32 char_index = 0;
	s_point2 santa_position = { 0, 0 };
	s_point2 robo_position = { 0, 0 };
	bool move_santa = true;

	points.insert(santa_position);

	for (; line[char_index] != '\0'; char_index++)
	{
		char current_char = line[char_index];
		s_point2& current_position = move_santa ? santa_position : robo_position;

		switch (current_char)
		{
		case '^':
		{
			current_position.y += 1;
			break;
		}
		case 'v':
		{
			current_position.y -= 1;
			break;
		}
		case '>':
		{
			current_position.x += 1;
			break;
		}
		case '<':
		{
			current_position.x -= 1;
			break;
		}
		}

		points.insert(current_position);
		move_santa = !move_santa;
	}

	std::cout << "Santa and robo santa have visited " << points.size() << " unique houses" << std::endl;

	free(line);
}
