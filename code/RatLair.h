#pragma once
#include "Rat.h"

using namespace sf;

int const LAIR_BASE_SIZE = 100;

int getLairDimensions(int baseSize);

int createBackground(VertexArray& rVA, IntRect arena);
Rat* createHorde(int numRats, IntRect arena);