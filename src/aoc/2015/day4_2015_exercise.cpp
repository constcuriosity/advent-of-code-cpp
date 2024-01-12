
//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_string.h"
#include "foundation/utilities/md5_hash.h"

#include <iostream>
#include <iomanip>

//-------------- constants

static_const int32 k_line_size = k_kilobyte;

//-------------- prototypes

void execute_2015_day4_part1(FILE* file);
void execute_2015_day4_part2(FILE* file);

static_function void print_hash(const s_md5_digest& digest);

//-------------- globals

extern const s_aoc_exercise_data k_2015_day4_exercise
{
	2015,
	4,
	"The Ideal Stocking Stuffer",
	execute_2015_day4_part1,
	execute_2015_day4_part2
};

//-------------- public implementations

void execute_2015_day4_part1(FILE* file)
{
	// $TODO This should be able to allocate the correct amount of space for the line
	char* line = static_cast<char*>(malloc(k_line_size));
	fgets(line, k_line_size, file);

	int32 iteration = NONE;
	c_md5_hash test_hash;
	s_md5_digest test_digest;
	c_static_string<32> iteration_string;

	do
	{
		iteration_string.print("%d", ++iteration);
		test_hash.clear();
		test_hash.append(line);
		test_hash.append(iteration_string.get_string());
		test_digest = test_hash.finalize();
	} while (!(test_digest.digest[0] == 0 &&
		test_digest.digest[1] == 0 &&
		(test_digest.digest[2] & 0xF0) == 0));

	std::cout << "MD5 Hash with 5 leading zeros: ";
	print_hash(test_digest);
	std::cout << std::endl;
	std::cout << "Generated from source string: " << line << iteration_string.get_string() << std::endl;

	free(line);
}

void execute_2015_day4_part2(FILE* file)
{
	// $TODO This should be able to allocate the correct amount of space for the line
	char* line = static_cast<char*>(malloc(k_line_size));
	fgets(line, k_line_size, file);

	int32 iteration = NONE;
	c_md5_hash test_hash;
	s_md5_digest test_digest;
	c_static_string<32> iteration_string;

	do
	{
		iteration_string.print("%d", ++iteration);
		test_hash.clear();
		test_hash.append(line);
		test_hash.append(iteration_string.get_string());
		test_digest = test_hash.finalize();
	} while (!(test_digest.digest[0] == 0 &&
		test_digest.digest[1] == 0 &&
		test_digest.digest[2] == 0));

	std::cout << "MD5 Hash with 6 leading zeros: ";
	print_hash(test_digest);
	std::cout << std::endl;
	std::cout << "Generated from source string: " << line << iteration_string.get_string() << std::endl;

	free(line);
}

//-------------- private implementations

static_function void print_hash(
	const s_md5_digest& digest)
{
	std::cout << std::hex;
	for (int32 digest_index = 0; digest_index < 16; digest_index++)
	{
		std::cout << std::setfill('0') << std::setw(2) << (int)digest.digest[digest_index];
	}
	std::cout << std::dec;
}
