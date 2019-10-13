#include <stdbool.h>

#ifndef TRIE_H
#define TRIE_H
#define NUMBER_OF_STATES 4

struct Trie {
	bool is_energy_declared;
	uint64_t energy;
	struct Trie *next_energy;
	struct Trie *children[NUMBER_OF_STATES];
};

typedef struct Trie Trie;

Trie* createNode(void);

void deleteTrie(Trie *current_node);

void moveNode(Trie **current_node, char quantum_state);

bool moveToTheNode(Trie **current_node, char *path, int length);

void removeNode(Trie *histories);

void removeNodes(Trie *acceptable_histories, char *quantum_states, int length);

void changeAllHistories(Trie *first_node);

#endif // TRIE_H
