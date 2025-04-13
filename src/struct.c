// bot.c - Structure related functions

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Headers
#include "struct.h"

// Functions
void copy_card(struct card *src, struct card *cp) {
    cp->suit = src->suit;
    cp->number = src->number;
    cp->deckid = src->deckid;
    cp->played = src->played;
}