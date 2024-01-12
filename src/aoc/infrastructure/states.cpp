
//-------------- headers

#include "aoc/infrastructure/states.h"

#include "foundation/containers/static/static_string.h"

#include "aoc/infrastructure/application_data.h"
#include "aoc/infrastructure/exercise.h"

#include <iostream>
#include <limits>

//-------------- public definitions

void c_aoc_introduction_state::enter()
{
	std::cout << "Advent of Code Exercises" << std::endl;
	std::cout << "========================" << std::endl;

	std::cout << std::endl;

	e_aoc_states next_state = e_aoc_states::execute_day;

	if (m_data->year == NONE)
	{
		next_state = e_aoc_states::select_year;
	}
	else if (m_data->day == NONE)
	{
		next_state = e_aoc_states::select_day;
	}

	m_transistor->request_transition(next_state);
}

void c_aoc_control_flow_state::enter()
{
	std::cout << "Choose an option:" << std::endl;
	std::cout << "R - Repeat the last exercise" << std::endl;
	std::cout << "Y - decide on a new Year" << std::endl;
	std::cout << "D - decide on a new Day" << std::endl;
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
	else if (decision == 'Y' || decision == 'y')
	{
		m_transistor->request_transition(e_aoc_states::select_year);
	}
	else if (decision == 'D' || decision == 'd')
	{
		m_transistor->request_transition(e_aoc_states::select_day);
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

void c_aoc_select_year_state::enter()
{
	std::cout << "Available Years:" << std::endl;

	for (int32 year_index = 0; year_index < m_data->exercises.get_size(); year_index++)
	{
		std::cout << m_data->exercises[year_index].year << std::endl;
	}

	std::cout << std::endl << "Enter the year that you'd like to execute: ";
}

void c_aoc_select_year_state::update()
{
	int32 year = NONE;
	std::cin >> year;

	// Boilerplate to reset the input buffer and ignore any errors
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << std::endl;

	int32 year_index = m_data->exercises.find(
		[&](const s_aoc_year_exercises& exercise) { return exercise.year == year; });

	if (year_index != NONE)
	{
		m_data->year = year;

		if (m_data->exercises[year_index].exercises.is_valid_index(m_data->day))
		{
			m_transistor->request_transition(e_aoc_states::execute_day);
		}
		else
		{
			m_transistor->request_transition(e_aoc_states::select_day);
		}
	}
	else
	{
		std::cout << "A valid year wasn't entered. Try again: ";
	}
}

void c_aoc_select_day_state::enter()
{
	int32 year_index = m_data->exercises.find(
		[&](const s_aoc_year_exercises& exercise) { return exercise.year == m_data->year; });

	if (year_index != NONE)
	{
		std::cout << "Year " << m_data->year << " Exercises:" << std::endl;

		int32 exercise_count = m_data->exercises[year_index].exercises.get_size();
		for (int32 exercise_index = 0; exercise_index < exercise_count; exercise_index++)
		{
			const s_aoc_exercise_data* exercise = m_data->exercises[year_index].exercises[exercise_index];

			std::cout << "Day " << exercise->day << " - " << exercise->name << std::endl;
		}

		std::cout << std::endl << "Enter the day that you'd like to execute (1 - " << exercise_count << "): ";
	}
}

void c_aoc_select_day_state::update()
{
	int32 year_index = m_data->exercises.find(
		[&](const s_aoc_year_exercises& exercise) { return exercise.year == m_data->year; });

	if (year_index != NONE)
	{
		int32 day_index = NONE;
		std::cin >> day_index;

		// Boilerplate to reset the input buffer and ignore any errors
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cout << std::endl;

		if (m_data->exercises[year_index].exercises.is_valid_index(day_index - 1))
		{
			m_data->day = day_index;
			m_transistor->request_transition(e_aoc_states::execute_day);
		}
		else
		{
			int32 exercise_count = m_data->exercises.get_size();
			std::cout << "A valid day wasn't entered. Try again (1 - " << exercise_count << "): ";
		}
	}
	else
	{
		std::cout << "Can't select day because we don't have have a valid year" << std::endl << std::endl;
		m_transistor->request_transition(e_aoc_states::select_year);
	}
}

void c_aoc_execute_day_state::enter()
{
	int32 year_index = m_data->exercises.find(
		[&](const s_aoc_year_exercises& exercise) { return exercise.year == m_data->year; });
	int32 day_index = m_data->day - 1;
	const s_aoc_exercise_data* exercise_data = nullptr;

	if (year_index != NONE && m_data->exercises[year_index].exercises.is_valid_index(day_index))
	{
		exercise_data = m_data->exercises[year_index].exercises[day_index];
	}

	if (exercise_data != nullptr)
	{
		c_medium_string title_string;
		title_string.print("Executing %d Day %d - %s",
			exercise_data->year,
			exercise_data->day,
			exercise_data->name);

		std::cout << title_string.get_string() << std::endl;

		for (int32 char_index = 0; char_index < title_string.length(); char_index++)
		{
			std::cout << "-";
		}

		std::cout << std::endl;

		c_aoc_exercise_runner exercise_runner(*exercise_data);
		exercise_runner.initialize();
		exercise_runner.execute_part_one();
		exercise_runner.execute_part_two();
		exercise_runner.dispose();

		std::cout << "Exercise Complete!" << std::endl << std::endl;
	}
	else
	{
		std::cout << "Couldn't find exercise data for " << m_data->year << " day " << m_data->day << std::endl;
	}

	m_transistor->request_transition(e_aoc_states::control_flow);
}

void c_aoc_terminate_state::enter()
{
	std::cout << "Goodbye!" << std::endl;
}
