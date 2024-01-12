
//-------------- headers

#include "aoc/infrastructure/aoc_machine.h"

//-------------- public implementations

c_aoc_machine::c_aoc_machine()
	: m_introduction_state(&m_data)
	, m_control_flow_state(&m_data)
	, m_select_year_state(&m_data)
	, m_select_day_state(&m_data)
	, m_execute_day_state(&m_data)
	, m_machine(e_aoc_states::introduction)
{}

void c_aoc_machine::initialize(
	int32 starting_day,
	int32 year,
	c_aoc_year_exercises_view exercises)
{
	m_data.day = starting_day;
	m_data.year = year;
	m_data.exercises = exercises;

	m_machine.register_state(&m_introduction_state);
	m_machine.register_state(&m_control_flow_state);
	m_machine.register_state(&m_select_year_state);
	m_machine.register_state(&m_select_day_state);
	m_machine.register_state(&m_execute_day_state);
	m_machine.register_state(&m_terminate_state);
}

void c_aoc_machine::run()
{
	m_machine.start();

	while (m_machine.get_current_state() != e_aoc_states::terminate)
	{
		m_machine.update();
	}
}