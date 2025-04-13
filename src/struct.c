// bot.c - Structure related functions

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

// Headers
#include "struct.h"

// Functions

// copy_card(struct card *src, card cp) - Copies values of src to cp
void copy_card(struct card *src, struct card *cp) {
    cp->suit = src->suit;
    cp->number = src->number;
    cp->deckid = src->deckid;
    cp->played = src->played;
}

// shuffle(struct card[], int len) - Shuffles an array of cards
//  arr - An array of len len 
void shuffle(struct card *arr, int len) {
    assert(arr != NULL);
    srand(time(NULL)); // Generates Random Seed
    for (int i = 0; i < len; i++) {
        int j = rand() % (i + 1); // Pick a random index from 0 to i
        // Swap arr[i] and arr[j]
        struct card temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// compare_hand
static int compare_card(const void *a, const void *b) {
    const struct card *cA = *(const struct card**)a;
    const struct card *cB = *(const struct card**)b;
    if(cA->deckid > cB->deckid) return 1;
    if(cA->deckid < cB->deckid) return -1;
    return 0;
}
// Sorts a player's hand ascending by suit then number
// hand - An array of len HAND_LENGTH of pointers to cards in deck - deck order -> hand order 
void sort_hand(struct card** hand) {
    assert(hand != NULL);
    qsort(hand, HAND_SIZE, sizeof(struct card*), compare_card);
}