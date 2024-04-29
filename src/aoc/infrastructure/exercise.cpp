
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/io/io.h"

//-------------- public definitions

void c_aoc_exercise_runner::initialize()
{
	c_kilo_string file_path;
	file_path.print("data/%d/day%d.txt",
		m_data.year,
		m_data.day);

	if (n_io::file_exists(file_path.get_string()))
	{
		fopen_s(&m_file, file_path.get_string(), "r");
	}
}

void c_aoc_exercise_runner::dispose()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = nullptr;
	}
}

void c_aoc_exercise_runner::execute_part_one()
{
	executue_exercise(m_data.part_one);
}

void c_aoc_exercise_runner::execute_part_two()
{
	executue_exercise(m_data.part_two);
}

void c_aoc_exercise_runner::executue_exercise(t_exercise_function function)
{
	if (m_file)
	{
		function(m_file);
		rewind(m_file);
	}
}
