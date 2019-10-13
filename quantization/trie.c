#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "trie.h"

/*Funkcja alokuje pamiêæ na wêze³ drzewa.*/
Trie* createNode() {
	Trie *new_history = NULL;

	new_history = (Trie*)malloc(sizeof(Trie));

    if (new_history) {
        new_history->is_energy_declared = false;
        new_history->next_energy = new_history;

        for (int i = 0; i < NUMBER_OF_STATES; i++)
            new_history->children[i] = NULL;
    }
    else {
        exit(1);
    }

    return new_history;
}

/*Funkcja usuwa ca³e drzewo z pamiêci.*/
void deleteTrie(Trie *current_node) {
	if (current_node) {
		for (int i = 0; i < NUMBER_OF_STATES; i++)
			deleteTrie(current_node -> children[i]);

		free(current_node);
	}
}

/*Funkcja sprawdza do którego syna przejœæ.*/
void moveNode(Trie **current_node, char quantum_state) {
	switch (quantum_state) {
		case '0':
			(*current_node) = (*current_node)->children[0];
			break;
		case '1':
			(*current_node) = (*current_node)->children[1];
			break;
		case '2':
			(*current_node) = (*current_node)->children[2];
			break;
		case '3':
			(*current_node) = (*current_node)->children[3];
			break;
	}
}

/*Funkcja przechodzi do odpowiedniego wêz³a.*/
bool moveToTheNode(Trie **current_node, char *path, int length) {
	bool result = true;

	for (int i = 0; i < length; i++) {
		if ((*current_node)->children[path[i] - '0'])
			moveNode(current_node, path[i]);
		else
			result = false;
	}

	return result;
}

/*Funkcja usuwa podan¹ historiê ze spl¹tanych z ni¹ energii.*/
void removeNode(Trie *histories) {
    Trie *tmp_node;

    if (histories) {
        for (int i = 0; i < NUMBER_OF_STATES; i++)
            removeNode(histories -> children[i]);

        if (histories->next_energy != histories) {
            tmp_node = histories;

            do {
                tmp_node = tmp_node->next_energy;
            } while (tmp_node->next_energy != histories);

            tmp_node->next_energy = histories->next_energy;
        }
    }
}

/*Funkcja usuwa z drzewa wszystkie historie
maj¹ce prefix zapisany w tablicy quantum_states.*/
void removeNodes(Trie *acceptable_histories, char *quantum_states, int length) {
    for (int i = 0; i < length - 1; i++) {
		if (acceptable_histories->children[quantum_states[i] - '0'])
			moveNode(&acceptable_histories, quantum_states[i]);
	}

	removeNode(acceptable_histories->children[quantum_states[length - 1] - '0']);
	deleteTrie(acceptable_histories->children[quantum_states[length - 1] - '0']);
	acceptable_histories->children[quantum_states[length - 1] - '0'] = NULL;
}

/*Funkcja zmienia wszystkie spl¹tane energie.*/
void changeAllHistories(Trie *first_node) {
	Trie *temporary_node = first_node;

	do {
		temporary_node = temporary_node->next_energy;
		temporary_node->is_energy_declared = true;
		temporary_node->energy = first_node->energy;
	} while (temporary_node != first_node);
}
