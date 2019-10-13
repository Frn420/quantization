#include <stdio.h>
#include "ignore-comments.h"
#include "order-operations.h"

/*Funckja pomija linijki zaczynaj¹ce siê od #.*/
int ignoreCommentedLine() {
	int character;

	character = getChar();

	if (character == '#') {
        while (character != '\n' && character != EOF)
            character = getchar();

        if (character == EOF) {
            fprintf(stderr, "ERROR\n");
        }
    }

	return character;
}
/*Funckja pomija puste linie i komentarze.*/
void ignoreEmptyLines() {
	int character;

	do {
		character = ignoreCommentedLine();
	} while (character == '\n');

	ungetc(character, stdin);
}
