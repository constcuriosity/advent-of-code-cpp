
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_grid.h"
#include "foundation/containers/dynamic/string.h"
#include "foundation/io/log.h"
#include "foundation/math/vector_math.h"
#include "foundation/memory/memory_utilities.h"

//-------------- constants

//-------------- prototypes

struct s_triangle
{
	union
	{
		struct
		{
			int32 a;
			int32 b;
			int32 c;
		};

		int32 values[3];
	};
};

bool is_triangle_possible(const s_triangle& t0);

void execute_2016_day3_part1(FILE* file);
void execute_2016_day3_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day3_exercise
{
	2016,
	3,
	"Squares With Three Sides",
	execute_2016_day3_part1,
	execute_2016_day3_part2
};

//-------------- definitions

void execute_2016_day3_part1(FILE* file)
{
	int32 possible_triangles = 0;

	s_triangle current_triangle = { 0, 0, 0 };

	while (feof(file) == 0 &&
		fscanf(file, "%d %d %d", &current_triangle.a, &current_triangle.b, &current_triangle.c) != EOF)
	{
		if (is_triangle_possible(current_triangle))
		{
			possible_triangles++;
		}
	}

	n_log::output("There are {} possible triangles if you count by rows\n", possible_triangles);
}

void execute_2016_day3_part2(FILE* file)
{
	int32 possible_triangles = 0;
	int32 index_offset = 0;
	s_triangle current_triangle = { 0, 0, 0 };
	
	for (int32 column_offset = 0; column_offset < 3; column_offset++)
	{
		int32 values[3];

		while (feof(file) == 0 &&
			fscanf(file, "%d %d %d", &values[0], &values[1], &values[2]) != EOF)
		{
			current_triangle.values[index_offset] = values[column_offset];
			index_offset++;

			if (index_offset >= 3)
			{
				if (is_triangle_possible(current_triangle))
				{
					possible_triangles++;
				}

				index_offset = 0;
			}
		}

		rewind(file);
	}

	n_log::output("There are {} possible triangles if you count by columns\n", possible_triangles);
}

bool is_triangle_possible(const s_triangle& t0)
{
	return t0.a + t0.b > t0.c &&
		t0.a + t0.c > t0.b &&
		t0.b + t0.c > t0.a;
}
