// source.c - Main file that initalizes the game and houses the game loop

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

// Headers
#include "source.h"
#include "bot.h"
#include "debug.h"

// Global Variables

// main(void) - Initializes the game and runs the game loop
int main(void) {
    // Declare deck and players hands
    struct card *deck = malloc(sizeof(struct card) * (SUITS*CARDS_PER_SUIT + TRUMPS)); // An array of cards in deck order (len deck size)
    struct card ***players= malloc(PLAYER_COUNT * sizeof(struct card*) * HAND_SIZE); // An array of player's hands which are arrays of pointers to cards in the deck (len hand_size)
    for (int p = 0; p < PLAYER_COUNT; p++) {
        players[p] = malloc(sizeof(struct card*) * HAND_SIZE);
    }

    // Fill the deck with cards
    for (int i = 0; i < SUITS; i++) {
        for (int j = 0; j < CARDS_PER_SUIT; j++) {
            deck[CARDS_PER_SUIT * i + j].suit = i+1;
            deck[CARDS_PER_SUIT * i + j].number = j+1;
            deck[CARDS_PER_SUIT * i + j].deckid = deck[CARDS_PER_SUIT * i + j].suit * 10 + deck[CARDS_PER_SUIT * i + j].number;
            deck[CARDS_PER_SUIT * i + j].played = 0;
        }
    }
    // Add trumps
    for (int i = 0; i < TRUMPS; i++) {
        deck[SUITS*CARDS_PER_SUIT + i].suit = SUITS+1;
        deck[SUITS*CARDS_PER_SUIT + i].number = i+1;
        deck[SUITS*CARDS_PER_SUIT + i].deckid = deck[SUITS*CARDS_PER_SUIT + i].suit * 10 + deck[SUITS*CARDS_PER_SUIT + i].number;
        deck[SUITS*CARDS_PER_SUIT + i].played = 0;
    }

    // Shuffle deck
    shuffle_cards(deck, SUITS*CARDS_PER_SUIT + TRUMPS);

    // Round Starter
    int starting_player = -1;

    // Deal cards
    for (int p = 0; p < PLAYER_COUNT; p++) {
        for (int c = 0; c < HAND_SIZE; c++) {
            players[p][c] = &deck[p * HAND_SIZE + c]; // Player[p][c] is a pointer to the card they received in the deck
            if (players[p][c]->deckid == (SUITS + 1) * 10 + TRUMPS) { // Highest card in the game denotes who starts first (Captain)
                starting_player = p;
                DEBUG_PRINT("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nPlayer %d is Captain.\n", p+1);
            }
        }
        #ifndef SKIPSORT
            sort_hand(players[p]);
        #endif
    }

    // Print player's hands
    #ifdef VERBOSE
        DEBUG_PRINT("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        for(int p = 0; p < PLAYER_COUNT; p++) {
            printf("Player: %d\n - ", p+1);
            for(int c = 0; c < HAND_SIZE - 1; c++) {
                printf(" %d, ", players[p][c]->deckid);
            }
            printf(" %d\n", players[p][HAND_SIZE-1]->deckid);
        }
    #endif

    // Assign tasks
    struct task* tasks = malloc(sizeof(struct task) * TASK_COUNT);
    DEBUG_PRINT("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    {   int ids[CARDS_PER_SUIT*SUITS];
        int i = 0;
        int p = starting_player;
        for (int c = 11; c <= (SUITS * 10) + CARDS_PER_SUIT; c++) {
           if (c % 10 == 0) c++;
           ids[i] = c;
           i++;
        }
        shuffle_ints(ids, CARDS_PER_SUIT*SUITS);
        for(int t = 0; t < TASK_COUNT; t++) {
            srand(time(NULL));
            tasks[t].cardDeckid = ids[t];
            tasks[t].owner = p;
            tasks[t].complete = false;
            DEBUG_PRINT("Task %d: Card: %d, Player %d\n", t+1, tasks[t].cardDeckid, p+1);
            p = (p+1) % PLAYER_COUNT;
        }
    }
    DEBUG_PRINT("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    
    // Game loop
    for(int t = 0; t < HAND_SIZE; t++) {
        DEBUG_PRINT("ROUND %d BEGINS: Player %d starts.\n", t+1, starting_player+1);
        // Players decays from a 2d array of pointers to a pointer
        communicate(players); // Players are given a chance to communicate a card once per game
        starting_player = trick(players, starting_player, tasks); // The person who starts the game is the one who won the last
    }
    int tasks_complete = 0;
    bool win = false;
    for(int t = 0; t < TASK_COUNT; t++) {
        if (tasks[t].complete) tasks_complete++;
        else DEBUG_PRINT("Failed task! --> Task %d: Card: %d, Player %d\n", t+1, tasks[t].cardDeckid, tasks[t].owner + 1);
    }
    if (tasks_complete == TASK_COUNT) {
        win = true;
        DEBUG_PRINT("\nYou win!!\n");
    } else {
        DEBUG_PRINT("\nYou lose :(\n");        
    }
    // Free memory
    for (int p = 0; p < PLAYER_COUNT; p++) {
        free(players[p]);
    }
    free(players);
    free(deck);
    return win;
}

// Plays a trick
// players - An array of player's hands which are arrays of pointers to cards in the deck (len hand_size)
// starting_player - An int which decrees which player begins the trick
int trick(struct card ***players, int starting_player, struct task *tasks) {
    assert(players);
    assert(starting_player < PLAYER_COUNT && starting_player >= 0);
    struct card **played = malloc(sizeof(struct card*) * PLAYER_COUNT); // Cards played this trick - an array of pointers to cards in deck - in player order 
    int p = starting_player;
    
    for (int i = 0; i < PLAYER_COUNT; i++) { // Cycles through all the players starting with starting_player 
        struct card **playable_cards = malloc(sizeof(struct card*) * HAND_SIZE); // An array (len hand) of pointers to cards in the hand (that can be played) - in hand order
        struct card first = {.suit = 0, .number = 0, .deckid = 0}; // a blank card
        
        if (i != 0) copy_card(played[starting_player], &first); // If this is not the first card, then set first to be the first in this trick

        int playable_len = playable(playable_cards, &first, players[p]); // The len of array playable_cards
        assert(playable_cards[0] != NULL); // That at least one card is playable from playable_cards
        assert(playable_len <= HAND_SIZE && playable_len > 0);
        int played_index = play_card(playable_cards, playable_len, played, i, players[p]); // An int that is the index of playable_cards that the bot wishes to play
        assert(played_index < playable_len && played_index >= 0);
        played[p] = playable_cards[played_index]; // Player plays a card
        played[p]->played = true;
        DEBUG_PRINT("Player %d plays card %d.\n", p+1, played[p]->deckid);
        p = (p + 1) % PLAYER_COUNT;
        
        free(playable_cards);
    }

    int winner_index = winner(played, starting_player, tasks);
    assert(winner_index < PLAYER_COUNT && winner_index >= 0);
    free(played);
    return winner_index; // Frees played
}

// Player communicates a card
// r_playes - a pointer to a 2d array
void communicate(struct card ***players) {}

// Modifies playable_cards to include only playable cards and returns the number of playable cards
//      playable_cards - a blank array (len hand) of pointers to cards in the hand - in hand order
//      first - a card
//      played_len - the length of cards that have been played
//      hand - an array (len hand) of pointers to cards in deck - in hand order
int playable(struct card **playable_cards,  struct card *first, struct card **hand) {
    assert(playable_cards != NULL);
    assert(first != NULL);
    assert(hand != NULL);

    int playable_len = 0;
    
    for (int c = 0; c < HAND_SIZE; c++) {
        assert(hand[c]);
        if (hand[c]->played) { // If a card is played, it cannot be played again
        } else if (first->suit == 0) { // If this is the first card
            playable_cards[playable_len] = hand[c];
            playable_len++;
        } else if (hand[c]->suit == first->suit) { // The card is the correct suit
            playable_cards[playable_len] = hand[c];
            playable_len++;
        } else { // Check if you can discard this card by checking if player has other of this suit
            bool has_suit = false;
            for (int card = 0; card < HAND_SIZE; card++) {
                has_suit = has_suit || (hand[card]->suit == first->suit && !hand[card]->played);
            }
            if (!has_suit) {
                playable_cards[playable_len] = hand[c];
                playable_len++;
            }
        }
    }
    return playable_len;
}

// Returns which player won the trick
//      played - an array (len players) of pointers to cards in deck - in player order 
int winner(struct card **played, int starting_player, struct task *tasks) {
    assert(played != NULL);
    assert(starting_player < PLAYER_COUNT && starting_player >= 0);
    struct card max_card;
    copy_card(played[starting_player], &max_card);
    int winning_player = starting_player;
    for (int p = 0; p < PLAYER_COUNT; p++) {
        assert(played[p] != NULL);
        if ((played[p]->suit == max_card.suit && played[p]->number > max_card.number) || // If on-suit and better
            (max_card.suit != SUITS + 1 && played[p]->suit == SUITS + 1)) // If the first trump then auto-win
         {
            copy_card(played[p], &max_card);
            winning_player = p;
        }
    }
    // Check if a task was won
    for(int t = 0; t < TASK_COUNT; t++) {
        if(tasks[t].owner == winning_player) {
            for(int c = 0; c < PLAYER_COUNT; c++) {
                if (tasks[t].cardDeckid == played[c]->deckid) {
                    tasks[t].complete = true; // Set task as complete
                    DEBUG_PRINT("Task %d complete! - Card: %d, Player %d\n", t+1, tasks[t].cardDeckid, tasks[t].owner + 1);
                }
            }
        }
    }

    DEBUG_PRINT("Player %d wins the round with the card %d.\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", winning_player+1, max_card.deckid);
    return winning_player;
}