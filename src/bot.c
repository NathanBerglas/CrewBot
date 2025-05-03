// bot.c - Crew bot brain

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Headers
#include "bot.h"

// claim_task:
// This function returns the index of claimable_tasks that the bot wishes to claim
// Requirements: claim_task must return an integer t >= 0, t < task_len
int claim_task(int *claimable_task_ids, int task_len, struct information *info) {
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