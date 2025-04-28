//bot.h

// Repetition Protection
#pragma once
#ifndef BOT_H
#define BOT_H

// Libraries

// Headers
#include "struct.h"

// Functions
// claim_task - returns the index of claimable_tasks that the bot wishes to claim
int claim_task(int **claimable_tasks, int task_len, struct information *info);

// communicate_card - returns the index of possible_coms that the bot wishes to communicate
int communicate_card(struct communication *possible_coms, int communicatable_len, struct information *info);

// play_card - returns the index of playable_cards that the bot wishes to play
int play_card(struct card **playable_cards, int playable_len, struct information *info);

#endif