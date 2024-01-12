//-------------- headers

#include "aoc/infrastructure/exercise.h"

#include "foundation/containers/static/static_array.h"
#include "foundation/containers/static/static_string.h"
#include "foundation/containers/dynamic/dynamic_vector.h"
#include "foundation/containers/static/static_set.h"
#include "foundation/math/int_math.h"
#include "foundation/utilities/sort.h"

#include <iostream>

//-------------- constants

//-------------- prototypes

enum e_card_value
{
	_card_value_joker,
	_card_value_two,
	_card_value_three,
	_card_value_four,
	_card_value_five,
	_card_value_six,
	_card_value_seven,
	_card_value_eight,
	_card_value_nine,
	_card_value_ten,
	_card_value_jack,
	_card_value_queen,
	_card_value_king,
	_card_value_ace,

	k_card_value_count
};

const char k_card_value_lookup[] =
{
	'J',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'T',
	'J',
	'Q',
	'K',
	'A',
};

enum e_j_mode
{
	_j_mode_jack,
	_j_mode_joker,

	k_j_mode_count
};

enum e_card_hand
{
	_card_hand_high_card,
	_card_hand_one_pair,
	_card_hand_two_pair,
	_card_hand_three_of_kind,
	_card_hand_full_house,
	_card_hand_four_of_kind,
	_card_hand_five_of_kind,

	k_card_hand_count
};

typedef c_static_array<e_card_value, 5> c_card_array;

struct s_hand
{
	c_card_array cards;
	e_card_hand hand;
	uint32 bid;
};

class c_hand_comparator
{
public:
	bool is_ordered(const s_hand& left, const s_hand& right) const
	{
		bool ordered = left.hand < right.hand;

		if (!ordered && left.hand == right.hand)
		{
			for (int32 card_index = 0; card_index < 5; card_index++)
			{
				if (left.cards[card_index] > right.cards[card_index])
				{
					ordered = false;
					break;
				}
				else if (left.cards[card_index] < right.cards[card_index])
				{
					ordered = true;
					break;
				}
			}
		}

		return ordered; 
	}

	bool equals(const s_hand& left, const s_hand& right) const
	{
		return left.hand == right.hand &&
			left.bid == right.bid;
	}

	bool operator()(const s_hand& left, const s_hand& right) const
	{
		return is_ordered(left, right);
	}
};

e_card_value get_card_value_for_char(char value, e_j_mode mode);
e_card_hand get_hand_for_cards(const c_card_array& cards);

void execute_2023_day7_part1(FILE* file);
void execute_2023_day7_part2(FILE* file);

//-------------- globals

extern const s_aoc_exercise_data k_2023_day7_exercise
{
	2023,
	7,
	"Camel Cards",
	execute_2023_day7_part1,
	execute_2023_day7_part2
};

//-------------- definitions

void execute_2023_day7_part1(FILE* file)
{
	c_medium_string line_buffer;

	c_dynamic_vector<s_hand> hands_vector;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);
		c_utf8_tokenizer hand_tokenizer(line_buffer.get_string(), " ");
		s_hand hand;

		c_utf8_character_iterator card_tokenizer(hand_tokenizer.get_token().string, hand_tokenizer.get_token().length);
		int32 current_card_index = 0;

		do
		{
			char card = static_cast<char>(card_tokenizer.get_codepoint());
			e_card_value value = get_card_value_for_char(card, _j_mode_jack);

			hand.cards[current_card_index] = value;
			current_card_index++;
		}
		while (card_tokenizer.advance());

		hand.hand = get_hand_for_cards(hand.cards);
		
		hand_tokenizer.advance();

		uint64 bid = c_string_utilities::to_unsigned_integer(hand_tokenizer.get_token());
		hand.bid = static_cast<uint32>(bid);

		hands_vector.push(hand);
	}

	quick_sort(hands_vector.get_storage(), hands_vector.get_size(), c_hand_comparator());

	uint64 total_winnings = 0;

	for (int32 hand_index = 0; hand_index < hands_vector.get_size(); hand_index++)
	{
		const s_hand& current_hand = hands_vector[hand_index];

		total_winnings += current_hand.bid * (hand_index + 1);
	}

	std::cout << "Total winnings of all hands w/ Jack rules: " << total_winnings << std::endl;
}

void execute_2023_day7_part2(FILE* file)
{
	c_medium_string line_buffer;

	c_dynamic_vector<s_hand> hands_vector;

	while (feof(file) == 0)
	{
		fgets(line_buffer.get_string(), c_medium_string::k_capacity, file);
		c_utf8_tokenizer hand_tokenizer(line_buffer.get_string(), " ");
		s_hand hand;

		c_utf8_character_iterator card_tokenizer(hand_tokenizer.get_token().string, hand_tokenizer.get_token().length);
		int32 current_card_index = 0;

		do
		{
			char card = static_cast<char>(card_tokenizer.get_codepoint());
			e_card_value value = get_card_value_for_char(card, _j_mode_joker);

			hand.cards[current_card_index] = value;
			current_card_index++;
		}
		while (card_tokenizer.advance());

		hand.hand = get_hand_for_cards(hand.cards);

		hand_tokenizer.advance();

		uint64 bid = c_string_utilities::to_unsigned_integer(hand_tokenizer.get_token());
		hand.bid = static_cast<uint32>(bid);

		hands_vector.push(hand);
	}

	quick_sort(hands_vector.get_storage(), hands_vector.get_size(), c_hand_comparator());

	uint64 total_winnings = 0;

	for (int32 hand_index = 0; hand_index < hands_vector.get_size(); hand_index++)
	{
		const s_hand& current_hand = hands_vector[hand_index];

		total_winnings += current_hand.bid * (hand_index + 1);
	}

	std::cout << "Total winnings of all hands w/ Joker rules: " << total_winnings << std::endl;
}

e_card_value get_card_value_for_char(char value, e_j_mode mode)
{
	int32 card_index = (value == 'J' && mode == _j_mode_joker)
		? n_array_utilities::index_of(k_card_value_lookup, ARRAY_SIZE(k_card_value_lookup), value)
		: n_array_utilities::last_index_of(k_card_value_lookup, ARRAY_SIZE(k_card_value_lookup), value);

	return card_index != NONE ? static_cast<e_card_value>(card_index) : k_card_value_count;
}

e_card_hand get_hand_for_cards(const c_card_array& cards)
{
	struct s_card_count
	{
		s_card_count() : value(_card_value_two), count(1) {}
		s_card_count(e_card_value card) : value(card), count(1) {}

		e_card_value value;
		int32 count;
	};

	c_static_vector<s_card_count, 5> card_set;
	int32 joker_count = 0;

	for (int32 card_index = 0; card_index < cards.get_size(); card_index++)
	{
		e_card_value card = cards[card_index];
		s_card_count card_count(card);
		
		int32 found_index = card_set.find([&](const s_card_count& in)
		{
			return in.value == card;
		});

		if (card == _card_value_joker)
		{
			joker_count++;
		}
		else if (found_index == NONE)
		{
			card_set.push(card_count);
		}
		else
		{
			card_set[found_index].count++;
		}
	}

	int32 unique_card_count = card_set.get_size();
	e_card_hand result = k_card_hand_count;

	switch (unique_card_count)
	{
	case 0:
		assert(joker_count == 5);
		result = _card_hand_five_of_kind;
		break;
	case 1:
		result = _card_hand_five_of_kind;
		break;
	case 2:
		if (joker_count == 3 || joker_count == 2)
		{
			result = _card_hand_four_of_kind;
		}
		else
		{
			result = (card_set[0].count == 2 || card_set[1].count == 2)
				? _card_hand_full_house
				: _card_hand_four_of_kind;
		}
		break;
	case 3:
		result = (joker_count > 0 || card_set[0].count == 3 || card_set[1].count == 3 || card_set[2].count == 3)
			? _card_hand_three_of_kind
			: _card_hand_two_pair;
		break;
	case 4:
		result = _card_hand_one_pair;
		break;
	case 5:
		result = _card_hand_high_card;
		break;
	default:
		assert(false);
		break;
	}

	return result;
}
