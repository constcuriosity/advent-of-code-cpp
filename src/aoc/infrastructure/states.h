#ifndef H_AOC_STATES
#define H_AOC_STATES

//-------------- headers

#include "foundation/core.h"
#include "foundation/containers/static/static_vector.h"
#include "foundation/utilities/state_machine.h"

#include "aoc/infrastructure/state_enum.h"

//-------------- forward declarations

struct s_application_data;

//-------------- definitions

class c_aoc_introduction_state : public c_state<e_aoc_states>
{
public:
	c_aoc_introduction_state(s_application_data* data) : m_data(data) {}

	e_aoc_states get_state_id() const override { return e_aoc_states::introduction; }
	void enter() override;

private:
	s_application_data* m_data;
};

class c_aoc_control_flow_state : public c_state<e_aoc_states>
{
public:
	c_aoc_control_flow_state(s_application_data* data) : m_data(data) {}

	e_aoc_states get_state_id() const override { return e_aoc_states::control_flow; }
	void enter() override;
	void update() override;

private:
	s_application_data* m_data;
};

class c_aoc_gather_day_state : public c_state<e_aoc_states>
{
public:
	c_aoc_gather_day_state(s_application_data* data) : m_data(data) {}

	e_aoc_states get_state_id() const override { return e_aoc_states::gather_day; }
	void enter() override;
	void update() override;

private:
	s_application_data* m_data;
};

class c_aoc_execute_day_state : public c_state<e_aoc_states>
{
public:
	c_aoc_execute_day_state(s_application_data* data) : m_data(data) {}

	e_aoc_states get_state_id() const override { return e_aoc_states::execute_day; }
	void enter() override;
	void exit() override;

private:
	s_application_data* m_data;
};

class c_aoc_terminate_state : public c_state<e_aoc_states>
{
public:
	c_aoc_terminate_state() {}

	e_aoc_states get_state_id() const override { return e_aoc_states::terminate; }
	void enter() override;
};

#endif // H_AOC_STATES
