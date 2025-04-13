//source.h

// Repetition Protection
#pragma once
#ifndef SOURCE_H
#define SOURCE_H

// Libraries

// Headers
#include "struct.h"

// Functions
void communicate(struct card ***players);
int trick(struct card ***players, int starting_player);
int playable(struct card **playable_cards, struct card *first, struct card **hand);
int winner(struct card **played, int starting_player);

#endif