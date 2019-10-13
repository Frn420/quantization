#include <stdbool.h>

#ifndef ORDER_OPERATIONS_H
#define ORDER_OPERATIONS_H

int getChar();

char* getOrder(int *order_length);

bool checkOrder(char *order, char *correct_order,
                int order_length, int correct_length);

void putError();

bool checkHistory(char *quantum_states, int length);

bool checkEnergy(char *energy_sequence, int length);

#endif // ORDER_OPERATIONS_H
