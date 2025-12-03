#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FIVE_KIND 7
#define FOUR_KIND 6
#define FULL_HOUSE 5
#define THREE_KIND 4
#define TWO_PAIR 3
#define ONE_PAIR 2
#define HIGH_CARD 1

typedef struct {
	char hand[5];
	char sorted[5];
	unsigned rank;
	unsigned bid;
} HAND_T;

int compar(const void *, const void *);
unsigned check_jack(char *);

int str_cmp(const void *a, const void *b) { return *(char *)a - *(char *)b; }

char *CARDS = "23456789TQKA";

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	HAND_T *hands = NULL;
	unsigned num_hands = 0;

	char *line = NULL;
	size_t n = 0;

	while (getline(&line, &n, infile) > 1) {
		hands = realloc(hands, (num_hands + 1) * sizeof(*hands));
		memcpy(hands[num_hands].hand, line, 5);
		memcpy(hands[num_hands].sorted, line, 5);
		qsort(hands[num_hands].sorted, 5, sizeof(char), &str_cmp);
		hands[num_hands].rank = check_jack(hands[num_hands].sorted);
		hands[num_hands].bid = strtoul(&line[6], NULL, 10);
		num_hands++;
	}

	fclose(infile);
	free(line);

	qsort(hands, num_hands, sizeof(*hands), &compar);

	uint64_t sum = 0;
	unsigned rank = 0;

	for (unsigned i = 0; i < num_hands; i++) {
		sum += hands[i].bid * ++rank;
	}

	free(hands);

	printf("Total winnings: %lu\n", sum);

	return 0;
}

unsigned rank_hand(char *hand) {
	if (hand[0] == hand[1]) {
		if (hand[1] == hand[2]) {
			if (hand[2] == hand[3]) {
				if (hand[3] == hand[4]) return FIVE_KIND;
				return FOUR_KIND;
			}
			if (hand[3] == hand[4]) return FULL_HOUSE;
			return THREE_KIND;
		}
		if (hand[2] == hand[3]) {
			if (hand[3] == hand[4]) return FULL_HOUSE;
			return TWO_PAIR;
		}
		if (hand[3] == hand[4])	return TWO_PAIR;
		return ONE_PAIR;
	}

	if (hand[1] == hand[2]) {
		if (hand[2] == hand[3]) {
			if (hand[3] == hand[4]) return FOUR_KIND;
			return THREE_KIND;
		}
		if (hand[3] == hand[4]) return TWO_PAIR;
		return ONE_PAIR;
	}

	if (hand[2] == hand[3]) {
		if (hand[3] == hand[4]) return THREE_KIND;
		return ONE_PAIR;
	}

	if (hand[3] == hand[4]) return ONE_PAIR;

	return HIGH_CARD;
}

unsigned check_jack(char *hand) {
	unsigned highest = 0, rank = 0;

	for (unsigned i = 0; i < 5; i++) {
		if (hand[i] != 'J') continue;
		
		char temp[5];
		memcpy(temp, hand, 5);

		for (unsigned j = 0; j < 12; j++) {
			hand[i] = CARDS[j];
			qsort(hand, 5, sizeof(char), &str_cmp);
			rank = check_jack(hand);
			memcpy(hand, temp, 5);

			if (rank > highest) highest = rank;
			if (highest == FIVE_KIND) return highest;
		}

	}

	if (! highest) return rank_hand(hand);

	return highest;
}

int get_card_val(char card) {
	if (card <= '9') return (int)(card - '0');
	if (card == 'T') return 10;
	if (card == 'J') return 1;
	if (card == 'Q') return 12;
	if (card == 'K') return 13;
	if (card == 'A') return 14;
}

int compar(const void *a, const void *b) {
	HAND_T hand1 = *(HAND_T *)a, hand2 = *(HAND_T *)b;
	
	if (hand1.rank != hand2.rank) return hand1.rank - hand2.rank;

	for (unsigned i = 0; i < 5; i++) {
		if (hand1.hand[i] == hand2.hand[i]) continue;

		return get_card_val(hand1.hand[i]) - get_card_val(hand2.hand[i]);
	}

	return 0;
}
