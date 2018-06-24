#pragma once
#include <stdint.h>
#include <vector>
#include <iostream>
#include "GameConfigurator.h"
#include "CellPosition.h"
#include "Error.h"
#include "Field.h"



class CellByCellGameConfigurator : public GameConfigurator {

public:

    CellByCellGameConfigurator(const uint32_t fieldWidth, const uint32_t fieldHeight) :
        GameConfigurator(fieldWidth, fieldHeight) {
    }

    // from GameConfigurator
    void populateField(Field &field) override;

    Error addAliveCell(Position aliveCellPosition);
    void batchAddAliveCells(const std::vector<Position> &aliveCells);

private:

    std::vector<Position> _positions;

};
