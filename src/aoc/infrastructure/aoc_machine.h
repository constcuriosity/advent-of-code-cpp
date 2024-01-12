#ifndef H_AOC_AOC_MACHINE
#define H_AOC_AOC_MACHINE

//-------------- headers

#include "foundation/core.h"
#include "foundation/containers/view/array_view.h"

#include "aoc/infrastructure/application_data.h"
#include "aoc/infrastructure/exercise.h"
#include "aoc/infrastructure/states.h"

//-------------- definitions

class c_aoc_machine
{
public:
	c_aoc_machine();

	void initialize(int32 starting_day, int32 year, c_exercise_view exercises);

	void run();

private:
	s_application_data m_data;
	c_aoc_introduction_state m_introduction_state;
	c_aoc_control_flow_state m_control_flow_state;
	c_aoc_gather_day_state m_gather_day_state;
	c_aoc_execute_day_state m_execute_day_state;
	c_aoc_terminate_state m_terminate_state;
	c_state_machine<e_aoc_states, (int32)e_aoc_states::k_count> m_machine;
};

#endif // H_AOC_AOC_MACHINE
