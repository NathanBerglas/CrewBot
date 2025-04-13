// bot.c - Crew bot brain

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Headers
#include "bot.h"

// Functions
int play_card(struct card **playable_cards, int playable_len, struct card **played, int played_len, struct card **hand) {
    srand(time(NULL)); // Generates Random Seed
    if(playable_len == 1) {
        return 0;
    }
    int c = rand() % (playable_len - 1); // Pick a random index from 0 to playable_len - 1
    return c;
}