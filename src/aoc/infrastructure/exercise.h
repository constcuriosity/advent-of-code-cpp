#ifndef H_AOC_EXERCISE
#define H_AOC_EXERCISE

//-------------- headers

#include "foundation/core.h"

#include <cstdio>

//-------------- forward declarations

typedef void (*t_exercise_function)(FILE* file);

//-------------- definitions

struct s_aoc_exercise_data
{
	int32 year;
	int32 day;
	const char* name;
	t_exercise_function part_one;
	t_exercise_function part_two;
};

class c_aoc_exercise_runner
{
public:
	c_aoc_exercise_runner(const s_aoc_exercise_data &data)
		: m_data(data)
		, m_file(nullptr)
	{ }

	int32 get_day_index() const { m_data.day; }
	const char* get_day_name() const { return m_data.name; }

	void initialize();
	void dispose();

	void execute_part_one();
	void execute_part_two();

private:
	void executue_exercise(t_exercise_function function);

	const s_aoc_exercise_data& m_data;
	FILE *m_file;
};

#endif // H_AOC_EXERCISE
