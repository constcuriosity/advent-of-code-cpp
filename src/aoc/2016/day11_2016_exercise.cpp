
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/dynamic/dynamic_set.h"
#include "foundation/containers/dynamic/dynamic_vector.h"
#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_set.h"
#include "foundation/containers/static/static_vector.h"
#include "foundation/io/log.h"
#include "foundation/utilities/wyhash.h"

#include <unordered_set>

//-------------- constants

static_const int32 k_floor_count = 4;

//-------------- prototypes

enum e_item_type
{
	_item_type_generator,
	_item_type_microchip,

	k_item_type
};

enum e_element
{
	// My input
	_element_promethium,
	_element_cobalt,
	_element_curium,
	_element_ruthenium,
	_element_plutonium,
	_element_elerium,
	_element_dilithium,

	// Test input
	/*_element_hydrogen,
	_element_lithium,*/

	k_element
};

struct s_item
{
	bool is_generator() const { return type == _item_type_generator; }
	bool is_microchip() const { return type == _item_type_microchip; }

	uint64 to_hash() const { return 1ULL << (type * k_element + element); }

	void print() const { n_log::output(" {}{} ", is_generator() ? "g" : "m", static_cast<int32>(element)); }

	bool operator==(const s_item& other) const { return type == other.type && element == other.element; }
	bool operator<(const s_item& other) const
	{
		return type < other.type || (type == other.type && element < other.element);
	}

	e_item_type type;
	e_element element;
};

typedef c_static_set<s_item, k_item_type * k_element> c_floor_vector;

struct s_building_state
{
	s_building_state()
	{
		c_memory_utilities::zero<s_building_state>(this);
	}

	s_building_state(const s_building_state& other)
	{
		c_memory_utilities::zero<s_building_state>(this);
		memcpy(this, &other, sizeof(s_building_state));
	}

	uint64 to_hash()
	{
		uint64 hash = 0;
		for (int32 floor_index = 0; floor_index < floors.get_size(); floor_index++)
		{
			for (int32 item_index = 0; item_index < floors[floor_index].get_size(); item_index++)
			{
				hash |= floors[floor_index][item_index].to_hash() << k_element * k_item_type * floor_index;
			}
		}

		hash |= current_floor << k_element * k_item_type * k_floor_count;

		return hash;
	}

	bool finished() const
	{
		return floors[0].is_empty() && floors[1].is_empty() && floors[2].is_empty();
	}

	bool is_valid() const
	{
		bool result = true;

		for (int32 floor_index = 0; floor_index < k_floor_count && result; floor_index++)
		{
			const c_floor_vector& floor = floors[floor_index];
			bool has_generator = false;
			bool has_microchip = false;

			// It's fine for a floor to be empty, all chips, or all generators. Weed that out first.
			for (int32 item_index = 0; item_index < floor.get_size(); item_index++)
			{
				if (floor[item_index].is_generator())
				{
					has_generator = true;
				}
				else if (floor[item_index].is_microchip())
				{
					has_microchip = true;
				}
			}

			// Check if need to guarantee microchip & generator pairs.
			if (has_generator && has_microchip)
			{
				for (int32 chip_index = 0; chip_index < floor.get_size(); chip_index++)
				{
					if (floor[chip_index].is_microchip())
					{
						e_element target_element = floor[chip_index].element;
						bool found_element = floor.any([&](const s_item& item)
						{
							return item.is_generator() && item.element == target_element;
						});

						if (!found_element)
						{
							result = false;
							break;
						}
					}
				}
			}
		}

		return result;
	}

	void print() const
	{
		n_log::output("{} ", current_floor);
		for (int32 floor_index = 0; floor_index < k_floor_count; floor_index++)
		{
			n_log::output("|{}|", floor_index);
			for (int32 item_index = 0; item_index < floors[floor_index].get_size(); item_index++)
			{
				floors[floor_index][item_index].print();
			}
		}
		n_log::output("\n");
	}

	c_static_array<c_floor_vector, k_floor_count> floors;
	int32 current_floor;
};

struct s_step_state
{
	void print() const
	{
		n_log::output("{} ", distance);
		state.print();
	}

	s_building_state state;
	int32 distance;
};

typedef c_dynamic_set<uint64> c_floor_set;
typedef c_dynamic_vector<s_step_state> c_state_queue;

static_function int32 move_items(c_state_queue *state_queue);

void execute_2016_day11_part1(FILE* file);
void execute_2016_day11_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2016_day11_exercise
{
	2016,
	11,
	"Radioisotope Thermoelectric Generators",
	execute_2016_day11_part1,
	execute_2016_day11_part2
};

//-------------- definitions

void execute_2016_day11_part1(FILE* file)
{
	int32 steps = 0;
	std::unordered_set<uint64> seen_floor_set;
	c_state_queue state_queue;

	s_building_state starting_state;
	starting_state.current_floor = 0;

	// My input
	starting_state.floors[0].add({ _item_type_generator, _element_promethium });
	starting_state.floors[0].add({ _item_type_microchip, _element_promethium });
	starting_state.floors[1].add({ _item_type_generator, _element_cobalt });
	starting_state.floors[1].add({ _item_type_generator, _element_curium });
	starting_state.floors[1].add({ _item_type_generator, _element_ruthenium });
	starting_state.floors[1].add({ _item_type_generator, _element_plutonium });
	starting_state.floors[2].add({ _item_type_microchip, _element_cobalt });
	starting_state.floors[2].add({ _item_type_microchip, _element_curium });
	starting_state.floors[2].add({ _item_type_microchip, _element_ruthenium });
	starting_state.floors[2].add({ _item_type_microchip, _element_plutonium });

	// Test input
	/*starting_state.floors[0].add({ _item_type_microchip, _element_hydrogen });
	starting_state.floors[0].add({ _item_type_microchip, _element_lithium });
	starting_state.floors[1].add({ _item_type_generator, _element_hydrogen });
	starting_state.floors[2].add({ _item_type_generator, _element_lithium });*/

	s_step_state first_step;
	first_step.state = starting_state;
	first_step.distance = 0;

	state_queue.push(first_step);

	steps = move_items(&state_queue);

	n_log::output("It took {} steps to move all of the chips and generators to the top floor\n", steps);
}

void execute_2016_day11_part2(FILE* file)
{
	int32 steps = 0;
	std::unordered_set<uint64> seen_floor_set;
	c_state_queue state_queue;

	s_building_state starting_state;
	starting_state.current_floor = 0;

	// My input
	starting_state.floors[0].add({ _item_type_generator, _element_promethium });
	starting_state.floors[0].add({ _item_type_microchip, _element_promethium });
	starting_state.floors[0].add({ _item_type_generator, _element_elerium });
	starting_state.floors[0].add({ _item_type_microchip, _element_elerium });
	starting_state.floors[0].add({ _item_type_generator, _element_dilithium });
	starting_state.floors[0].add({ _item_type_microchip, _element_dilithium });
	starting_state.floors[1].add({ _item_type_generator, _element_cobalt });
	starting_state.floors[1].add({ _item_type_generator, _element_curium });
	starting_state.floors[1].add({ _item_type_generator, _element_ruthenium });
	starting_state.floors[1].add({ _item_type_generator, _element_plutonium });
	starting_state.floors[2].add({ _item_type_microchip, _element_cobalt });
	starting_state.floors[2].add({ _item_type_microchip, _element_curium });
	starting_state.floors[2].add({ _item_type_microchip, _element_ruthenium });
	starting_state.floors[2].add({ _item_type_microchip, _element_plutonium });

	// Test input
	/*starting_state.floors[0].add({ _item_type_microchip, _element_hydrogen });
	starting_state.floors[0].add({ _item_type_microchip, _element_lithium });
	starting_state.floors[1].add({ _item_type_generator, _element_hydrogen });
	starting_state.floors[2].add({ _item_type_generator, _element_lithium });*/

	s_step_state first_step;
	first_step.state = starting_state;
	first_step.distance = 0;

	state_queue.push(first_step);

	steps = move_items(&state_queue);

	n_log::output("It took {} steps to move all of the chips and generators to the top floor\n", steps);
}

static_function int32 move_items(c_state_queue* state_queue)
{
	int32 result = 0;
	std::unordered_set<uint64> seen_floor_set;
	seen_floor_set.insert(state_queue->at(0).state.to_hash());

	while (!state_queue->is_empty())
	{
		const s_step_state current_step = state_queue->at(0);
		const s_building_state& current_state = current_step.state;
		state_queue->erase(0);

		// current_step.print();

		if (current_state.finished())
		{
			result = current_step.distance;
			break;
		}

		// Can take 1 or 2 items and move up/down floors
		for (int32 floor_mod = -1; floor_mod < 2; floor_mod++)
		{
			int32 new_floor = current_state.current_floor + floor_mod;
			if (floor_mod == 0 || !VALID_INDEX(new_floor, k_floor_count))
			{
				continue;
			}

			// Move 1 item
			const c_floor_vector& floor = current_state.floors[current_state.current_floor];
			for (int32 item_index = 0; item_index < floor.get_size(); item_index++)
			{
				s_building_state next_state = current_state;
				next_state.current_floor = new_floor;

				s_item move_item = next_state.floors[current_state.current_floor][item_index];
				next_state.floors[current_state.current_floor].erase(item_index);
				next_state.floors[next_state.current_floor].add(move_item);

				if (next_state.is_valid())
				{
					uint64 next_state_hash = next_state.to_hash();

					if (seen_floor_set.find(next_state_hash) == seen_floor_set.end())
					{
						s_step_state new_step;
						new_step.state = next_state;
						new_step.distance = current_step.distance + 1;

						state_queue->push(new_step);
						seen_floor_set.insert(next_state_hash);
					}
				}
			}

			// Move 2 items
			for (int32 item_index = 0; item_index < floor.get_size(); item_index++)
			{
				for (int32 other_item_index = item_index + 1; other_item_index < floor.get_size(); other_item_index++)
				{
					s_building_state next_state = current_state;
					next_state.current_floor = new_floor;

					s_item move_item1 = next_state.floors[current_state.current_floor][item_index];
					s_item move_item2 = next_state.floors[current_state.current_floor][other_item_index];
					// make sure to erase the greater index first to avoid shifting indexes while erasing
					next_state.floors[current_state.current_floor].erase(other_item_index);
					next_state.floors[current_state.current_floor].erase(item_index);
					next_state.floors[next_state.current_floor].add(move_item1);
					next_state.floors[next_state.current_floor].add(move_item2);

					if (next_state.is_valid())
					{
						uint64 next_state_hash = next_state.to_hash();

						if (seen_floor_set.find(next_state_hash) == seen_floor_set.end())
						{
							s_step_state new_step;
							new_step.state = next_state;
							new_step.distance = current_step.distance + 1;

							state_queue->push(new_step);
							seen_floor_set.insert(next_state_hash);
						}
					}
				}
			}
		}
	}

	return result;
}
