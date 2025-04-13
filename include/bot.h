//bot.h

// Repetition Protection
#pragma once
#ifndef BOT_H
#define BOT_H

// Libraries

// Headers
#include "struct.h"

// Functions

// play_card - Returns index of playable_cards determining the card wished to be played
int play_card(struct card **playable_cards, int playable_len, struct card **played, int played_len, struct card **hand);

#endif