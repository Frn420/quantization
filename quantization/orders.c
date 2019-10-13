#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "order-operations.h"
#include "orders.h"
#include "trie.h"

#define INT_LIMIT 8446744073709551616

/*Funkcja declare alokuje dla drzewa acceptable_histories wêz³y
dla nowych dopuszczalnych historii zapisanych w tablicy quantum_states.*/
void declare(Trie *acceptable_histories, char *quantum_states, int length) {
	for (int i = 0; i < length; i++) {
		if (!(acceptable_histories->children[quantum_states[i] - '0']))
			acceptable_histories->children[quantum_states[i] - '0']
		    = createNode();

		moveNode(&acceptable_histories, quantum_states[i]);
	}
}

/*Funkcja declareHistories wykonuje polecenie DECLARE i zapisuje
historie w drzewie acceptableHistories.*/
void declareHistories(Trie *acceptable_histories) {
	char *quantum_states;
	int tmp = getChar();

	if (tmp == ' ') {
		quantum_states = getOrder(&tmp);

		if (getChar() == '\n') {
			if (checkHistory(quantum_states, tmp)) {
				declare(acceptable_histories, quantum_states, tmp);
				puts("OK");
			}
			else {
				fprintf(stderr, "ERROR\n");
			}
		}
		else {
			putError();
		}

		free(quantum_states);
	}
	else if (tmp == '\n') {
		fprintf(stderr, "ERROR\n");
	}
	else {
		putError();
	}
}

/*Funkcja wykonuje polecenie REMOVE.*/
void removeHistories(Trie *acceptable_histories) {
	char *quantum_states;
	int tmp = getChar();

	if (tmp == ' ') {
		quantum_states = getOrder(&tmp);

		if (getChar() == '\n') {
			if (checkHistory(quantum_states, tmp)) {
				removeNodes(acceptable_histories, quantum_states, tmp);
				puts("OK");
			}
			else {
				fprintf(stderr, "ERROR\n");
			}
		}
		else {
			putError();
		}

		free(quantum_states);
	}
	else if (tmp == '\n') {
		fprintf(stderr, "ERROR\n");
	}
	else {
		putError();
	}
}

/*Funkcja sprawdza czy w drzewie acceptable_histories wystêpuje
historia podana w tablicy quantum_states.*/
void checkState(Trie *acceptable_histories, char *quantum_states, int length) {
	if (moveToTheNode(&acceptable_histories, quantum_states, length))
		puts("YES");
	else
		puts("NO");
}

/*Funkcja wykonuje polecenie VALID.*/
void checkIfHistoryExists(Trie *acceptable_histories) {
	char *quantum_states;
	int tmp = getChar();

	if (tmp == ' ') {
		quantum_states = getOrder(&tmp);

		if (getchar() == '\n') {
			if (checkHistory(quantum_states, tmp))
				checkState(acceptable_histories, quantum_states, tmp);
			else
				fprintf(stderr, "ERROR\n");
		}
		else {
			putError();
		}

		free(quantum_states);
	}
	else if (tmp == '\n') {
		fprintf(stderr, "ERROR\n");
	}
	else {
		putError();
	}
}

/*Funkcja pokazuje energiê danej historii lub wypisuje
ERROR gdy historia nie wystêpuje w drzewie acceptable_histories.*/
void showEnergy(Trie *acceptable_histories, char *quantum_states, int length) {
	bool check = moveToTheNode(&acceptable_histories, quantum_states, length);

	if (check && acceptable_histories -> is_energy_declared)
		printf("%" PRIu64 "\n", acceptable_histories -> energy);
	else
		fprintf(stderr, "ERROR\n");
}

/*Funckja zwraca energiê zapisan¹ w tablicy energy_sequence.*/
bool getValue(uint64_t *energy, char *energy_sequence, int length) {
	uint64_t multiplier = 1;

	*energy = 0;

	for (int i = length - 1; i >= 0; i--) {
        if (*energy < INT_LIMIT)
            *energy += multiplier * (energy_sequence[i] - '0');
        else
            return false;

		multiplier *= 10;
	}

	return true;
}

/*Funckja przydziela energiê dla historii zapisanej
w tablicy history i wszystkim historiom spl¹tanym.*/
void assignEnergy(Trie *histories, char *history, int length, uint64_t energy) {
	if (moveToTheNode(&histories, history, length)) {
		histories->is_energy_declared = true;
		histories->energy = energy;
		changeAllHistories(histories);
		puts("OK");
	}
	else {
		putError();
	}
}

/*Funkcja wykonuje polecenie ENERGY.*/
void energyOrder(Trie *acceptable_histories) {
	char *history, *energy_sequence;
	int tmp, tmp2;
	uint64_t energy;

	tmp = getChar();

	if (tmp == ' ') {
		history = getOrder(&tmp);
		tmp2 = getChar();

		if (tmp2 == '\n') {
			if (checkHistory(history, tmp))
				showEnergy(acceptable_histories, history, tmp);
			else
				fprintf(stderr, "ERROR\n");
		}
		else if (tmp2 == ' ' && checkHistory(history, tmp)) {
			energy_sequence = getOrder(&tmp2);

			if (getchar() == '\n') {
				if (checkEnergy(energy_sequence, tmp2)) {
					if (getValue(&energy, energy_sequence, tmp2))
					    assignEnergy(acceptable_histories, history,
					                 tmp, energy);
					else
                        fprintf(stderr, "ERROR\n");
				}
				else {
					fprintf(stderr, "ERROR\n");
				}
			}
			else {
				putError();
			}

			free(energy_sequence);
		}
		else {
			putError();
		}

		free(history);
	}
	else if (tmp == '\n') {
		fprintf(stderr, "ERROR\n");
	}
	else {
		putError();
	}
}

/*Funkcja domyka relacjê równowa¿noœci spl¹tania energii.*/
void addEqualEnergy(Trie *node, Trie *node2) {
	Trie *tmp_node = node2;

	while (node2->next_energy != tmp_node)
        node2 = node2->next_energy;

	node2->next_energy = node->next_energy;
	node->next_energy = tmp_node;
}

/*Funkcja zrównuje energie jeœli warunki s¹ spe³nione.*/
void equalHistories(Trie *histories, char *history,
                    char *history2, int length, int length2) {
	bool check, check2;
	Trie *tmp_node = histories;

	check = moveToTheNode(&histories, history, length);
	check2 = moveToTheNode(&tmp_node, history2, length2);

	if (check && check2) {
		if (histories->is_energy_declared && tmp_node->is_energy_declared) {
			addEqualEnergy(histories, tmp_node);

			if (histories->energy % 2 == 1 && tmp_node->energy % 2 == 1)
			    tmp_node->energy
			    = histories->energy / 2 + tmp_node->energy / 2 + 1;
			else
                tmp_node->energy = histories->energy / 2 + tmp_node->energy / 2;

			changeAllHistories(tmp_node);

			printf("OK\n");
		}
		else if (histories->is_energy_declared &&
		         !(tmp_node->is_energy_declared)) {
			addEqualEnergy(histories, tmp_node);
			tmp_node->is_energy_declared = true;
			tmp_node->energy = histories->energy;

			printf("OK\n");
		}
		else if (!(histories->is_energy_declared) &&
                 tmp_node->is_energy_declared) {
			addEqualEnergy(histories, tmp_node);
			histories->is_energy_declared = true;
			histories->energy = tmp_node->energy;

			printf("OK\n");
		}
		else {
			fprintf(stderr, "ERROR\n");
		}
	}
	else {
		fprintf(stderr, "ERROR\n");
	}
}

/* Sprawdza czy energie s¹ takie same. */
bool areEqual(char *history1, char *history2, int length1, int length2) {
    bool check = true;

    if (length1 != length2)
        check = false;

    if (check)
        for (int i = 0; i < length1; i++)
            if (history1[i] != history2[i])
                check = false;

    return check;
}

/*Funckja wykonuje polecenie EQUAL.*/
void equalEnergies(Trie *acceptable_histories) {
	char *history, *history2;
	int tmp, tmp2;
	Trie *tmp_node = acceptable_histories;

	tmp = getChar();

	if (tmp == ' ') {
		history = getOrder(&tmp);
		tmp2 = getChar();

		if (tmp2 == ' ' && checkHistory(history, tmp)) {
			history2 = getOrder(&tmp2);

			if (getChar() == '\n') {
                if (!checkHistory(history2, tmp2)) {
                    fprintf(stderr, "ERROR\n");
                }
                else {
                    if (!areEqual(history, history2, tmp, tmp2)) {
                        equalHistories(acceptable_histories, history,
                                       history2, tmp, tmp2);
                    }
                    else if (moveToTheNode(&tmp_node, history, tmp)) {
                        printf("OK\n");
                    }
                    else {
                        fprintf(stderr, "ERROR\n");
                    }
                }
            }
			else {
				putError();
			}

			free(history2);
		}
		else if (tmp2 == '\n') {
			fprintf(stderr, "ERROR\n");
		}
		else {
			putError();
		}

		free(history);
	}
	else if (tmp == '\n') {
		fprintf(stderr, "ERROR\n");
	}
	else {
		putError();
	}
}

/*Funckja wywo³uje odpowiedni¹ funkcjê dla danego polecenia.*/
void doOrder(Trie *acceptable_histories, char *order, int order_length) {
	if (checkOrder(order, "DECLARE", order_length, 7))
		declareHistories(acceptable_histories);
	else if (checkOrder(order, "REMOVE", order_length, 6))
		removeHistories(acceptable_histories);
	else if (checkOrder(order, "VALID", order_length, 5))
		checkIfHistoryExists(acceptable_histories);
	else if (checkOrder(order, "ENERGY", order_length, 6))
		energyOrder(acceptable_histories);
	else if (checkOrder(order, "EQUAL", order_length, 5))
		equalEnergies(acceptable_histories);
	else
		putError();
}
