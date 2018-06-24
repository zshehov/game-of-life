#include "CellByCellGameConfigurator.h"

Error CellByCellGameConfigurator::addAliveCell(Position aliveCellPosition) {
    if (aliveCellPosition.posX >= _fieldWidth ||
        aliveCellPosition.posY >= _fieldHeight) {
        return Error::InvalidPositionForCell;
    }

    _positions.push_back(aliveCellPosition);
    return Error::None;
}

void CellByCellGameConfigurator::batchAddAliveCells(const std::vector<Position>& aliveCells) {
    for (const auto & aliveCell : aliveCells) {
        if (addAliveCell(aliveCell) == Error::InvalidPositionForCell) {
            std::cerr << "Alive cell with coordinates: " << aliveCell.posX << ", "
                << aliveCell.posY << " was not added: Out of field\n";
        }
    }
}

Error CellByCellGameConfigurator::populateField(Field & field) {
    for (const auto & cellPosition : _positions) {
        field.makeCellAlive(cellPosition.posX, cellPosition.posY);
    }
    return Error::None;
}
