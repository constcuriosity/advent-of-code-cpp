//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/containers/static/static_vector.h"
#include "foundation/io/log.h"
#include "foundation/math/basic_math.h"

//-------------- constants

//-------------- prototypes

void execute_2023_day6_part1(FILE* file);
void execute_2023_day6_part2(FILE* file);

struct s_race
{
	int32 total_time;
	int32 best_distance;
	int32 winning_time_range;
};

//-------------- globals

extern const s_aoc_exercise_data k_2023_day6_exercise
{
	2023,
	6,
	"Wait For It",
	execute_2023_day6_part1,
	execute_2023_day6_part2
};

//-------------- definitions

void execute_2023_day6_part1(FILE* file)
{
	c_medium_string line_buffer;

	fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

	c_utf8_tokenizer time_tokens(line_buffer.get_string(), " ");
	c_static_vector<int32, 4> times;

	time_tokens.advance(); // Skip first token

	do
	{
		int32 time = static_cast<int32>(n_string::to_integer(time_tokens.get_token()));
		times.push(time);
	}
	while (time_tokens.advance());

	fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);

	c_utf8_tokenizer distance_tokens(line_buffer.get_string(), " ");
	c_static_vector<int32, 4> distances;

	distance_tokens.advance(); // Skip first token

	do
	{
		int32 distance = static_cast<int32>(n_string::to_integer(distance_tokens.get_token()));
		distances.push(distance);
	}
	while (distance_tokens.advance());

	c_static_array<s_race, 4> races;
	uint64 margin_of_error = 1;

	for (int32 race_index = 0; race_index < times.get_size(); race_index++)
	{
		s_race race;
		race.total_time = times[race_index];
		race.best_distance = distances[race_index];

		// Solve for y, where y = x(t - x)
		// y = total distance
		// x = time held button
		// t = total time for race
		//
		// x^2 - tx + y = 0
		// Lookup the quadratic equation and solve for each intersection point.
		// Then round as necessary and subtract the distance between each point.

		real32 square_root_result = sqrtf(race.total_time * race.total_time - 4.0f * race.best_distance);
		real32 first_intersection = ceil((-race.total_time + square_root_result) / 2.0f);
		real32 second_intersection = ceil((-race.total_time - square_root_result) / 2.0f);

		race.winning_time_range = static_cast<int32>(fabsf(first_intersection - second_intersection));

		margin_of_error *= race.winning_time_range;
	}

	n_log::output("The margin of error across all races is {}\n", margin_of_error);
}

void execute_2023_day6_part2(FILE* file)
{
	c_medium_string line_buffer;
	c_medium_string number_buffer;

	fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);
	c_utf8_tokenizer time_tokens(line_buffer.get_string(), " ");

	time_tokens.advance(); // Skip first token

	do
	{
		number_buffer.append(time_tokens.get_token().string, time_tokens.get_token().length);
	} while (time_tokens.advance());

	int64 total_time = n_string::to_integer(number_buffer.get_string());
	number_buffer.clear();

	fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);
	c_utf8_tokenizer distance_tokens(line_buffer.get_string(), " ");

	distance_tokens.advance(); // Skip first token

	do
	{
		number_buffer.append(distance_tokens.get_token().string, distance_tokens.get_token().length);
	} while (distance_tokens.advance());

	int64 best_distance = n_string::to_integer(number_buffer.get_string());
	number_buffer.clear();


	// Solve for y, where y = x(t - x)
	// y = total distance
	// x = time held button
	// t = total time for race
	//
	// x^2 - tx + y = 0
	// Lookup the quadratic equation and solve for each intersection point.
	// Then round as necessary and subtract the distance between each point.

	real64 square_root_result = sqrt(total_time * total_time - 4.0 * best_distance);
	real64 first_intersection = (-total_time + square_root_result) / 2.0f;
	real64 second_intersection = (-total_time - square_root_result) / 2.0f;

	real64 difference = first_intersection - second_intersection;
	int64 winning_time_range = static_cast<int64>(abs(difference));

	n_log::output("There are {} toal number of ways to beat the best distance\n", winning_time_range);
}
