
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/io/log.h"

#include <set>

//-------------- constants

const int32 k_line_size = k_kilobyte * 128;

//-------------- prototypes

void execute_2015_day_part1(FILE* file);
void execute_2015_day3_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2015_day3_exercise
{
	2015,
	3,
	"Perfectly Spherical Houses in a Vacuum",
	execute_2015_day_part1,
	execute_2015_day3_part2
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

void execute_2015_day_part1(FILE* file)
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

	n_log::output("Santa has visited {} unique houses\n", points.size());

	free(line);
}

void execute_2015_day3_part2(FILE* file)
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

	n_log::output("Santa and robo santa have visited {} unique houses\n", points.size());

	free(line);
}
