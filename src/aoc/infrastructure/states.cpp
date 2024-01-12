
//-------------- headers

#include "aoc/infrastructure/states.h"

#include "aoc/infrastructure/application_data.h"
#include "aoc/infrastructure/exercise.h"

#include <iostream>
#include <limits>

//-------------- public definitions

void c_aoc_introduction_state::enter()
{
	std::cout << "Advent of Code " << m_data->year << " Exercises" << std::endl;
	std::cout << "=============================" << std::endl;

	int32 exercise_count = m_data->exercises.get_size();
	for (int32 exercise_index = 0; exercise_index < exercise_count; exercise_index++)
	{
		const s_aoc_exercise_data* data = m_data->exercises[exercise_index];

		std::cout << "Day " << data->day_index << " " <<  data->name << std::endl;
	}

	std::cout << std::endl;

	m_transistor->request_transition(
		m_data->day_index > 0 ? e_aoc_states::execute_day : e_aoc_states::gather_day);
}

void c_aoc_control_flow_state::enter()
{
	std::cout << "Choose an option:" << std::endl;
	std::cout << "R - Repeat the last exercise" << std::endl;
	std::cout << "D - Decide on a new day" << std::endl;
	std::cout << "E - End your session" << std::endl;
}

void c_aoc_control_flow_state::update()
{
	char decision;
	std::cin >> decision;

	// Boilerplate to reset the input buffer and ignore any errors
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << std::endl;

	if (decision == 'R' || decision == 'r')
	{
		m_transistor->request_transition(e_aoc_states::execute_day);
	}
	else if (decision == 'D' || decision == 'd')
	{
		m_transistor->request_transition(e_aoc_states::gather_day);
	}
	else if (decision == 'E' || decision == 'e')
	{
		m_transistor->request_transition(e_aoc_states::terminate);
	}
	else
	{
		std::cout << "Input not recognized. Try again: ";
	}
}

void c_aoc_gather_day_state::enter()
{
	int32 exercise_count = m_data->exercises.get_size();

	std::cout << "Enter the index of the day that you'd like to execute (1 - " << exercise_count << "): ";
}

void c_aoc_gather_day_state::update()
{
	int32 day_index = NONE;
	std::cin >> day_index;
	
	// Boilerplate to reset the input buffer and ignore any errors
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << std::endl;

	int32 exercise_count = m_data->exercises.get_size();

	if (day_index > 0 && day_index <= exercise_count)
	{
		m_data->day_index = day_index;
		m_transistor->request_transition(e_aoc_states::execute_day);
	}
	else
	{
		std::cout << "A valid day index wasn't entered. Try again (1 - " << exercise_count << "): ";
	}
}

void c_aoc_execute_day_state::enter()
{
	const s_aoc_exercise_data* exercise_data = m_data->exercises[m_data->day_index - 1];

	std::cout << "Executing Day " << m_data->day_index << " - " << exercise_data->name << std::endl << std::endl;

	if (exercise_data != nullptr)
	{
		c_aoc_exercise_runner exercise_runner(*exercise_data);
		exercise_runner.initialize();
		exercise_runner.execute_part_one();
		exercise_runner.execute_part_two();
		exercise_runner.dispose();
	}
	else
	{
		std::cout << "No exercise found for day " << m_data->day_index << std::endl;
	}

	m_transistor->request_transition(e_aoc_states::control_flow);
}

void c_aoc_execute_day_state::exit()
{
	std::cout << "Exercise Complete!" << std::endl << std::endl;
}

void c_aoc_terminate_state::enter()
{
	std::cout << "Goodbye!" << std::endl;
}
