#include <stdint.h>
#include "trie.h"

#ifndef ORDERS_H
#define ORDERS_H

void declare(Trie *acceptable_histories, char *quantum_states, int length);

void declareHistories(Trie *acceptable_histories);

void removeHistories(Trie *acceptable_histories);

void checkState(Trie *acceptable_histories, char *quantum_states, int length);

void checkIfHistoryExists(Trie *acceptable_histories);

void showEnergy(Trie *acceptable_histories, char *quantum_states, int length);

bool getValue(uint64_t *energy, char *energy_sequence, int length);

void assignEnergy(Trie *histories, char *history, int length, uint64_t energy);

void energyOrder(Trie *acceptable_histories);

void addEqualEnergy(Trie *node, Trie *node2);

bool areEqual(char *history1, char *history2, int length1, int length2);

void equalHistories(Trie *histories, char *history,
                    char *history2, int length, int length2);

void equalEnergies(Trie *acceptable_histories);


void doOrder(Trie *acceptable_histories, char *order, int order_length);

#endif // ORDERS_H
