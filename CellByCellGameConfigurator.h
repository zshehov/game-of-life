#pragma once
#include <stdint.h>
#include <vector>
#include "GameConfigurator.h"
#include "CellPosition.h"
#include "Error.h"
#include "Field.h"

/*
    Responsible for creating a configuration of the field by
    manually inserting positions of alive cells
*/

class CellByCellGameConfigurator : public GameConfigurator {

public:
    CellByCellGameConfigurator(const uint32_t fieldWidth, const uint32_t fieldHeight) :
        GameConfigurator(fieldWidth, fieldHeight) {
    }

    // from GameConfigurator abstract class
    Error populateField(Field &field) override;

    // own methods
    Error addAliveCell(Position aliveCellPosition);
    void batchAddAliveCells(const std::vector<Position> &aliveCells);

private:
    std::vector<Position> _positions;
};
