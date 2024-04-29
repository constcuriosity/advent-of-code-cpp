
//-------------- headers

#include "aoc/infrastructure/states.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/io/log.h"

#include "aoc/infrastructure/application_data.h"
#include "aoc/infrastructure/exercise.h"

#include <iostream>
#include <limits>

//-------------- public definitions

void c_aoc_introduction_state::enter()
{
	n_log::output("Advent of Code Exercises\n");
	n_log::output("========================\n\n");

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
	n_log::output("Choose an option:\n");
	n_log::output("R - Repeat the last exercise\n");
	n_log::output("Y - decide on a new Year\n");
	n_log::output("D - decide on a new Day\n");
	n_log::output("E - End your session\n");
}

void c_aoc_control_flow_state::update()
{
	char decision;
	std::cin >> decision;

	// Boilerplate to reset the input buffer and ignore any errors
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	n_log::output("\n");

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
		n_log::output("Input not recognized. Try again: ");
	}
}

void c_aoc_select_year_state::enter()
{
	n_log::output("Available Years:\n");

	for (int32 year_index = 0; year_index < m_data->exercises.get_size(); year_index++)
	{
		n_log::output("{}\n", m_data->exercises[year_index].year);
	}

	n_log::output("\nEnter the year that you'd like to execute: ");
}

void c_aoc_select_year_state::update()
{
	int32 year = NONE;
	std::cin >> year;

	// Boilerplate to reset the input buffer and ignore any errors
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	n_log::output("\n");

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
		n_log::output("A valid year wasn't entered. Try again: ");
	}
}

void c_aoc_select_day_state::enter()
{
	int32 year_index = m_data->exercises.find(
		[&](const s_aoc_year_exercises& exercise) { return exercise.year == m_data->year; });

	if (year_index != NONE)
	{
		n_log::output("Year {} Exercises:\n", m_data->year);

		int32 exercise_count = m_data->exercises[year_index].exercises.get_size();
		for (int32 exercise_index = 0; exercise_index < exercise_count; exercise_index++)
		{
			const s_aoc_exercise_data* exercise = m_data->exercises[year_index].exercises[exercise_index];

			n_log::output("Day {} - {}\n", exercise->day, exercise->name);
		}

		n_log::output("\nEnter the day that you'd like to execute (1 - {}): ", exercise_count);
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

		n_log::output("\n");

		if (m_data->exercises[year_index].exercises.is_valid_index(day_index - 1))
		{
			m_data->day = day_index;
			m_transistor->request_transition(e_aoc_states::execute_day);
		}
		else
		{
			int32 exercise_count = m_data->exercises[year_index].exercises.get_size();
			n_log::output("A valid day wasn't entered. Try again (1 - {}): ", exercise_count);
		}
	}
	else
	{
		n_log::output("Can't select day because we don't have have a valid year\n\n");
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

		n_log::output("{}\n", title_string.get_string());

		for (int32 char_index = 0; char_index < title_string.length(); char_index++)
		{
			n_log::output("-");
		}

		n_log::output("\n");

		c_aoc_exercise_runner exercise_runner(*exercise_data);
		exercise_runner.initialize();
		exercise_runner.execute_part_one();
		exercise_runner.execute_part_two();
		exercise_runner.dispose();

		n_log::output("Exercise Complete!\n\n");
	}
	else
	{
		n_log::output("Couldn't find exercise data for {} day {}\n", m_data->year, m_data->day);
	}

	m_transistor->request_transition(e_aoc_states::control_flow);
}

void c_aoc_terminate_state::enter()
{
	n_log::output("Goodbye!\n");
}
