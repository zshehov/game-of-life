#pragma once
#include <stdint.h>

struct Position {
    Position(const uint32_t posX = 0, const uint32_t posY = 0) :
        posX(posX),
        posY(posY) {}


    const uint32_t posX;
    const uint32_t posY;
};