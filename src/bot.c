// bot.c - Crew bot brain

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Headers
#include "bot.h"

// This is Mark I of my Crew Bot: Niobium

// claim_task:
// This function returns the index of claimable_tasks that the bot wishes to claim
// Requirements: claim_task must return an integer t >= 0, t < task_len
int claim_task(int **claimable_tasks, int task_len, struct information *info) {
    if (task_len == 1) return 0; // In case the bot is the last to take a task
    int scores[HAND_SIZE] = {0};
    int handStrength = 0; // 12 is worst, 88 is best (trumpStrength = 10), 46.4 is average (7.2 5s and 0.4 Trump 2.5s + 0.4 captain strength)
        const int trumpStrength = 10; // Extra strengh given to trumps
        const int captainBonus = 2; // Extra strength given to Captain
    bool claimed[SUITS] = {0}; // Which suits are claimed
        const int claimedBonus = 5;
    int min[SUITS + 1] = {0}; int max[SUITS + 1] = {0};
    
        const int trumpTaskBonus = 3;
        const int appropriateBonus = 5;

    // Calculate HandStrength
    for (int c = 0; c < HAND_SIZE; c++) {
        if (info->hand[c]->deckid == (SUITS + 1) * 10 + TRUMPS) handStrength += captainBonus; // Bot is captain
        if (info->hand[c]->played) continue;
        if (info->hand[c]->suit < SUITS) {
            handStrength += info->hand[c]->number;
        } else {
            handStrength += info->hand[c]->number + trumpStrength; 
        }
    }
    
    if (TASK_COUNT > PLAYER_COUNT) { // Tasks cycle
        for (int t = 0; t < TASK_COUNT; t++) {
            if (info->tasks[t].owner == 0) continue; // Unclaimed
            claimed[info->tasks[t].cardDeckid / 10] = true; // Suit of the task is "claimed"
        }
    }

    // Calculate min and max card of each suit. 0 means no card of this suit
    for (int c = 0; c < HAND_SIZE; c++) {
            if (min[info->hand[c]->suit - 1] == 0) min[info->hand[c]->suit - 1] = info->hand[c]->number;
            if (info->hand[c]->number < min[info->hand[c]->suit - 1]) min[info->hand[c]->suit - 1] = info->hand[c]->number;
            if (info->hand[c]->number > max[info->hand[c]->suit - 1]) max[info->hand[c]->suit - 1] = info->hand[c]->number;
            assert(min != max);
            assert(min < max);
    }

    // Calculate score
    for (int t = 0; t < task_len; t++) {
        int TsuitIndex = (*claimable_tasks[t] / 10) - 1;
        // Bonuses
        //  -    High card of that suit
        scores[t] += max[t];
        //  -    Suit is unclaimed
        if (TASK_COUNT > PLAYER_COUNT) scores[t] += (!claimed[TsuitIndex]) * claimedBonus;
        //  -   Bot has trump, not Task suit
        scores[t] += (max[SUITS] != 0 && max[TsuitIndex] == 0) * trumpTaskBonus; // Has no cards of this suit and a trump
        //  -   Task difficulty is appropriate
        
    }

    return 0;
}

// communicate_card:
// This function returns the index of possible_coms that the bot wishes to communicate.
// Communication can only be done once per game. If the bot wishes to skip, then it should return -1
// Requirements: communicate_card must return an integer c >= 0, c < communicatable_len
int communicate_card(struct communication *possible_coms, int communicatable_len, struct information *info) {
    return -1;
}

// play_card:
// This function returns the index of playable_cards that the bot wishes to play
// Requirements: play_card must return an integer c >= 0, c < playable_len
int play_card(struct card **playable_cards, int playable_len, struct information *info) {
    return 0;
}

// Methods

// Taking Tasks
//  Give each task a score, and then pick the task with the highest score. 
//  The score is calculated with bonuses and detractors. A tasks score is the sum of bonuses and detractors.
//  Bonuses:
//  -   Bot has a high card of that suit (Hightened if task is high)
//  -   They may "claim suit" (Take a task in a suit of which there are other tasks of the same suit that will cycle to them)
//  -   Bot has a trump and does not have a card of the tasks' suit
//  -   Task is hard and Bot has a good hand / vice-versa
//  Detractors:
//  -   Bot has the task card (Hightened if low)
//  -   Another player has claimed the task's suit
//  -   Task is hard and Bot has a bad hand / vice-versa