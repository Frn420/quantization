#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "order-operations.h"

/*Funkcja wywo³uje funkcjê getchar.*/
int getChar() {
    return getchar();
}

/*Funkcja zwraca teblicê znaków z wejœcia
do miejsca gdy napotka spacjê lub koniec linii.*/
char* getOrder(int *order_length) {
	char *word = NULL;
	int i = 0;
	int character = getchar();

	while (character != ' ' && character != '\n' && character != EOF) {
		word = realloc(word, (i + 1) * sizeof(char));

		ungetc(character, stdin);
		scanf("%c", &word[i]);

		i++;
		character = getchar();
    }

	ungetc(character, stdin);
	*order_length = i;

	return word;
}

/*Funkcja sprawdza jakie polecenie zosta³o podane.*/
bool checkOrder(char *order, char *correct_order,
                int order_length, int correct_length) {
	int counter = 0;

	if (order_length == correct_length)
		for(int i = 0; i < order_length; i++)
			counter += order[i] == correct_order[i];

	return counter == correct_length;
}

/*Funckja wypisuje b³¹d na wyjœcie
diagnostyczne i przechodzi do nastêpnej linii.*/
void putError() {
	fprintf(stderr, "ERROR\n");
	while(getchar() != '\n');
}

/*Funkcja sprawdza czy w stringu s¹ cyfry od 0 do 3.*/
bool checkHistory(char *quantum_states, int length) {
	bool result = true;
	int i;

	for (i = 0; i < length; i++)
		switch (quantum_states[i]) {
			case '0': break;
			case '1': break;
			case '2': break;
			case '3': break;
			default:
				result = false;
				break;
		}

	return result;
}

/*Funkcja sprawdza czy w stringu jest energia.*/
bool checkEnergy(char *energy_sequence, int length) {
	bool check = true;

	for(int i = 0; i < length; i++) {
		switch (energy_sequence[i]) {
			case '0':
				if (i == 0)
					check = false;
				break;
			case '1': break;
			case '2': break;
			case '3': break;
			case '4': break;
			case '5': break;
			case '6': break;
			case '7': break;
			case '8': break;
			case '9': break;
			default:
				check = false;
				break;
		}
	}

	if (length > 20)
        check = false;

	return check;
}
