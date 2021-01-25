#pragma once
#include "Rat.h"

using namespace sf;

int createBackground(VertexArray& rVA, IntRect arena);
Rat* createHorde(int numRats, IntRect arena);
