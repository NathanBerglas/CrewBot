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
    int task_scores[TASK_COUNT] = {0};
    int handStrength = 0; // 12 is worst, 88 is best (trumpStrength = 10), 46.4 is average (7.2 5s and 0.4 Trump 2.5s + 0.4 captain strength)
        const int trumpStrength = 10; // Extra strengh given to trumps
        const int difficultyCutoff = 7; // 7 and above considered "difficult"
        const int easeCutoff = 4; // 4 and below considered "easy"
        const int strengthCutoff = 55; // 55 and above considered "strong"
        const int weakCutoff = 35; // 35 and below considered "weak"
    int suitCount[SUITS + 1] = {0}; // How many cards does the Bot have in that suit
        const int suitCutoff = 3; // 3 suited cards is considered "well-suited"
        const int suitedBonus = 3;
    bool claimed[SUITS + 1] = {0}; // Which suits are claimed
        const int claimedBonus = 5; // Detractor
    bool hasTask[TASK_COUNT] = {0};
    int min[SUITS + 1] = {0}; int max[SUITS + 1] = {0};
    
        const int captainBonus = 2; // Extra strength given to Captain
        const int trumpTaskBonus = 3;
        const int appropriateBonus = 3; // Bonus & Detractor

    // Calculate HandStrength, if bot has that Task, and count cards per suit
    for (int c = 0; c < HAND_SIZE; c++) {
        if (info->hand[c]->deckid == (SUITS + 1) * 10 + TRUMPS) handStrength += captainBonus; // Bot is captain
        if (info->hand[c]->played) continue;
        for (int t = 0; t < task_len; t++) { // Check if this card is a task
            if (info->hand[c]->deckid == *claimable_tasks[t]) hasTask[t] = true;
        }
        if (info->hand[c]->suit < SUITS) {
            handStrength += info->hand[c]->number;
        } else {
            handStrength += info->hand[c]->number + trumpStrength; 
        }
        suitCount[info->hand[c]->suit - 1]++;
    }
    
    // Calculate if suit is taken
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
    }

    // Calculate score
    for (int t = 0; t < task_len; t++) {
        int TsuitIndex = (*claimable_tasks[t] / 10) - 1;
        // Bonuses
        //  -    High card of that suit
        task_scores[t] += max[TsuitIndex];
        //  -   Bot has trump, not Task suit
        if (suitCount[SUITS] > 0 && suitCount[TsuitIndex] == 0) task_scores[t] += trumpTaskBonus; // Has no cards of this suit and a trump, suitCount[SUITS] is max trump
        //  -   Task difficulty is appropriate
        if (*claimable_tasks[t] >= difficultyCutoff && handStrength >= strengthCutoff) task_scores[t] += appropriateBonus;
        //  -   Bot has many of that suit
        if (suitCount[TsuitIndex] > suitCutoff) task_scores[t] += suitedBonus;

        // Detractors
        //  -   Bot has task card
        if (hasTask[t]) task_scores[t] -= (CARDS_PER_SUIT - (*claimable_tasks[t] % 10)); // 10 - task, ie. no penalty if it's the 9, but a huge penalty if its the 1
        //  -    Suit is claimed
        if (task_len > PLAYER_COUNT) task_scores[t] -= (!claimed[TsuitIndex]) * claimedBonus;
        //  -   Task difficulty is appropriate
        if (*claimable_tasks[t] >= easeCutoff && handStrength <= weakCutoff) task_scores[t] -= appropriateBonus;
    }

    // Calculate maximum scored task
    int maxScore = task_scores[0];
    int taskIndex = 0;
    for (int t = 1; t < task_len; t++) {
        if (task_scores[t] > maxScore) {
            maxScore = task_scores[t];
            taskIndex = t;
        }
    }

    return taskIndex;
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
//  -   Bot has a trump and does not have a card of the tasks' suit
//  -   Task is hard and Bot has a good hand / vice-versa
//  -   Bot has many of that suit
//  Detractors:
//  -   Bot has the task card (Hightened if low)
//  -   Another player has claimed the task's suit (Take a task in a suit of which there are other tasks of the same suit that will cycle to them)