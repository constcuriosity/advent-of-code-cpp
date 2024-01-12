#ifndef H_AOC_APPLICATION_DATA
#define H_AOC_APPLICATION_DATA

//-------------- headers

#include "foundation/core.h"
#include "foundation/containers/view/array_view.h"

//-------------- forward_declarations

struct s_aoc_exercise_data;
typedef c_array_view<const s_aoc_exercise_data*> c_exercise_view;

//-------------- definitions

struct s_application_data
{
	s_application_data()
		: day_index(0)
		, year(2015)
	{}

	c_exercise_view exercises;
	int32 day_index;
	int32 year;
};

#endif // H_AOC_APPLICATION_DATA
