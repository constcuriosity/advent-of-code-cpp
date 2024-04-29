
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/dynamic/dynamic_set.h"
#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/io/log.h"
#include "foundation/math/vector_math.h"
#include "foundation/utilities/bit_utilities.h"
#include "foundation/utilities/wyhash.h"

#include <unordered_map>

//-------------- constants

static_const int32 k_neighbor_offset_count = 4;

//-------------- prototypes

class c_a_star_point_comparator
{
public:
	bool is_ordered(const s_point2i& left, const s_point2i& right) const;

	bool equals(const s_point2i& left, const s_point2i& right) const
	{
		return left == right;
	}

	bool operator()(const s_point2i& left, const s_point2i& right) const
	{
		return is_ordered(left, right);
	}
};

class c_point_comparator
{
public:
	bool is_ordered(const s_point2i& left, const s_point2i& right) const
	{
		return left.x < right.x || (left.x == right.x && left.y < right.y);
	}

	bool equals(const s_point2i& left, const s_point2i& right) const
	{
		return left == right;
	}

	bool operator()(const s_point2i& left, const s_point2i& right) const
	{
		return is_ordered(left, right);
	}
};

typedef std::unordered_map<s_point2i, int32, c_wyhash<s_point2i>> c_path_cost_map;

static_function int32 distance_heuristic(const s_point2i& current_position, const s_point2i& target_position);
static_function bool is_wall(const s_point2i& p, int64 seed);

void execute_2016_day13_part1(FILE* file);
void execute_2016_day13_part2(FILE* file);

//-------------- globals

static_global void* g_total_cost_lookup = nullptr;

extern const s_aoc_exercise_data k_2016_day13_exercise
{
	2016,
	13,
	"A Maze of Twisty Little Cubicles",
	execute_2016_day13_part1,
	execute_2016_day13_part2
};

//-------------- definitions

bool c_a_star_point_comparator::is_ordered(const s_point2i& left, const s_point2i& right) const
{
	const c_path_cost_map* total_cost_map = reinterpret_cast<c_path_cost_map*>(g_total_cost_lookup);
	int32 left_cost = total_cost_map->at(left);
	int32 right_cost = total_cost_map->at(right);

	return left_cost > right_cost;
}

void execute_2016_day13_part1(FILE* file)
{
	c_kilo_string line_buffer;
	fgets(line_buffer.get_string(), c_kilo_string::k_capacity, file);

	int64 office_designer_favorite_number = n_string::to_integer(line_buffer.get_string());
	s_point2i target_position(31, 39);
	s_point2i start_position(1, 1);

	// $TODO This uses a slightly bastardized version of the A* pathfinding algorithm.
	// Generalize this and move it to a utility.

	c_dynamic_set<s_point2i, c_a_star_point_comparator> candidate_positions;
	candidate_positions.add(start_position);

	c_path_cost_map current_cost_map;
	current_cost_map[start_position] = 0;

	c_path_cost_map total_cost_map;
	total_cost_map[start_position] = distance_heuristic(start_position, target_position);
	g_total_cost_lookup = &total_cost_map;

	std::unordered_map<s_point2i, s_point2i, c_wyhash<s_point2i>> previous_position_map;
	c_dynamic_vector<s_point2i> final_path;

	c_static_array<s_vector2, k_neighbor_offset_count> position_offsets;
	position_offsets[0] = s_vector2(1.0f, 0.0f);
	position_offsets[1] = s_vector2(-1.0f, 0.0f);
	position_offsets[2] = s_vector2(0.0f, 1.0f);
	position_offsets[3] = s_vector2(0.0f, -1.0f);

	while (!candidate_positions.is_empty())
	{
		c_dynamic_set<s_point2i>::iterator candidate_position_iter = candidate_positions.end() - 1;
		s_point2i candidate_position = *candidate_position_iter;
		if (candidate_position == target_position)
		{
			// Found the target node. Now remake the path.
			s_point2i tail_position = candidate_position;
			auto previous_position_iter = previous_position_map.find(tail_position);

			while (previous_position_iter != previous_position_map.end())
			{
				final_path.push(tail_position);
				tail_position = previous_position_iter->second;
				previous_position_iter = previous_position_map.find(tail_position);
			}

			final_path.push(start_position);
			break;
		}

		candidate_positions.erase(candidate_position_iter);

		int32 candidate_cost = current_cost_map[candidate_position];

		for (int32 offset_index = 0; offset_index < k_neighbor_offset_count; offset_index++)
		{
			s_point2i neighbor_position = candidate_position + position_offsets[offset_index];
			int32 traversal_cost = is_wall(neighbor_position, office_designer_favorite_number) ? k_int32_max : 1;
			if (traversal_cost != k_int32_max)
			{
				int32 neighbor_current_cost = candidate_cost + traversal_cost;

				auto cost_map_iter = current_cost_map.find(neighbor_position);
				int32 neighbor_recorded_cost = cost_map_iter != current_cost_map.end() ? cost_map_iter->second : k_int32_max;
				if (neighbor_current_cost < neighbor_recorded_cost)
				{
					previous_position_map.insert_or_assign(neighbor_position, candidate_position);
					current_cost_map.insert_or_assign(neighbor_position, neighbor_current_cost);
					total_cost_map.insert_or_assign(neighbor_position, neighbor_current_cost + distance_heuristic(neighbor_position, target_position));
					candidate_positions.add(neighbor_position);
				}
			}
		}
	}

	int32 steps_to_target = final_path.get_size() - 1;

	n_log::output("It took {} steps to get to the target position\n", steps_to_target);
}

void execute_2016_day13_part2(FILE* file)
{
	c_kilo_string line_buffer;
	fgets(line_buffer.get_string(), c_kilo_string::k_capacity, file);

	int64 office_designer_favorite_number = n_string::to_integer(line_buffer.get_string());
	s_point2i start_position(1, 1);

	c_dynamic_set<s_point2i, c_point_comparator> candidate_positions;
	candidate_positions.add(start_position);

	c_path_cost_map current_cost_map;
	current_cost_map[start_position] = 0;

	c_static_array<s_vector2, k_neighbor_offset_count> position_offsets;
	position_offsets[0] = s_vector2(1.0f, 0.0f);
	position_offsets[1] = s_vector2(-1.0f, 0.0f);
	position_offsets[2] = s_vector2(0.0f, 1.0f);
	position_offsets[3] = s_vector2(0.0f, -1.0f);

	while (!candidate_positions.is_empty())
	{
		c_dynamic_set<s_point2i>::iterator candidate_position_iter = candidate_positions.end() - 1;
		s_point2i candidate_position = *candidate_position_iter;
		candidate_positions.erase(candidate_position_iter);

		int32 candidate_cost = current_cost_map[candidate_position];

		for (int32 offset_index = 0; offset_index < k_neighbor_offset_count; offset_index++)
		{
			s_point2i neighbor_position = candidate_position + position_offsets[offset_index];
			int32 neighbor_current_cost = candidate_cost + 1;
			if (!is_wall(neighbor_position, office_designer_favorite_number) && neighbor_current_cost <= 50)
			{
				auto cost_map_iter = current_cost_map.find(neighbor_position);
				int32 neighbor_recorded_cost = cost_map_iter != current_cost_map.end() ? cost_map_iter->second : k_int32_max;
				if (neighbor_current_cost < neighbor_recorded_cost)
				{
					current_cost_map.insert_or_assign(neighbor_position, neighbor_current_cost);
					candidate_positions.add(neighbor_position);
				}
			}
		}
	}

	int32 unique_location_count = static_cast<int32>(current_cost_map.size());

	n_log::output("There are {} unique locations within 50 steps\n", unique_location_count);
}

static_function int32 distance_heuristic(const s_point2i& current_position, const s_point2i& target_position)
{
	return n_math::abs(current_position.x - target_position.x) + n_math::abs(current_position.y - target_position.y);
}

static_function bool is_wall(const s_point2i &p, int64 seed)
{
	bool is_wall = p.x < 0 || p.y < 0;

	if (!is_wall)
	{
		int64 sum = p.x*p.x + 3*p.x + 2*p.x*p.y + p.y + p.y*p.y + seed;

		is_wall = n_bit_utilities::count(reinterpret_cast<byte*>(&sum), sizeof(int64)) % 2 != 0;
	}

	return is_wall;
}
