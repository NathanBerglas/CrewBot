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
#define TASK_COUNT 10 // 1-36 tasks

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
    int cardDeckid; // deckid 
    bool complete;
};

//information - Information necessary to make gameplay decisions that is passed to the player
struct information {
    struct card **played; // Array of cards that have been played this trick
    int played_len;
    struct card **hand; // Array of cards len HAND_CARDS in your hand
    struct task *tasks; // Array of tasks
    struct card **commed; // Array of cards len PLAYER_COUNT that have been communicated
};

// Functions
void copy_card(struct card *src, struct card *cp);
void shuffle_cards(struct card *arr, int len);
void shuffle_ints(int *arr, int len);
void sort_hand(struct card** hand);

#endif