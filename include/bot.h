//bot.h

// Repetition Protection
#pragma once
#ifndef BOT_H
#define BOT_H

// Libraries

// Headers
#include "struct.h"

// Functions
// claim_task - Returns index of claimable_tasks determining the task wished to be claimed
int claim_task(int **claimable_tasks, int task_len, struct information *info);

// communicate_card - Returns index of playable_cards determining the card wished to be played, -1 means no communication
int communicate_card(int *communicatable_ids, int communicatable_len, struct information *info);

// play_card - Returns index of playable_cards determining the card wished to be played
int play_card(struct card **playable_cards, int playable_len, struct information *info);

#endif