#include <stdio.h>
#include <stdlib.h>
#include "ignore-comments.h"
#include "order-operations.h"
#include "orders.h"
#include "trie.h"

int main() {
	char *word;
	int character, order_length;
	Trie *acceptable_histories = createNode();

	character = getChar();

	while (character != EOF) {
	    ungetc(character, stdin);

	    ignoreEmptyLines();
	    word = getOrder(&order_length);
	    doOrder(acceptable_histories, word, order_length);

	    if (word)
	        free(word);

	    character = getChar();
	}

    deleteTrie(acceptable_histories);

	return 0;
}
