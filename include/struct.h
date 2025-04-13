//struct.h

// Repetition Protection
#pragma once
#ifndef STRUCT_H
#define STRUCT_H

// Libraries
#include <stdbool.h>

// Headers

// Macros
#define SUITS 4 // Pink (1), Yellow (2), Green (3), Blue (4)
#define CARDS_PER_SUIT 9 // 1-9
#define TRUMPS 4 // 1-4
#define PLAYER_COUNT 5
#define HAND_SIZE 8 // Assuming 5 players

// Structures

//card - A playable card that makes up a deck
// Ex. 9, Blue
struct card {
    int suit;
    int number;
    int deckid;
    bool played;
};

//task - A card that is required to be won
struct task {
    int owner; // 1 to PLAYER_COUNT
    struct card target;
    bool complete;
};

// Functions
void copy_card(struct card *src, struct card *cp);

#endif