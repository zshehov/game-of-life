#include "Field.h"
#include "Cell.h"
#include "WrappingUint32.h"
#include <vector>
#include <thread>
#include <assert.h>
#define _RENDER_

/*
    Constructs the field, by allocating the needed memory for the _frame and
    initializes it with dead cells
*/
Field::Field(const uint32_t width,
			 const uint32_t height,
             const bool shouldRenderEveryGeneration) :
	_width(width),
	_height(height),
    _shouldRenderEveryGeneration(shouldRenderEveryGeneration),
	_renderer(this, _width, _height) {

	_frame = new Cell**[height];
	for (size_t i = 0; i < height; ++i) {
		_frame[i] = new Cell*[width];
		for (size_t j = 0; j < width; ++j) {
			_frame[i][j] = new Cell(CellState::dead, j, i);
		}
	}
}


Field::~Field() {
	for (size_t i = 0; i < _height; ++i) {
		for (size_t j = 0; j < _width; ++j) {
			delete _frame[i][j];
		}
		delete[] _frame[i];
	}

	delete[] _frame;
}

/*
    Sets the next state for every cell in the given row
*/
void Field::prepareFieldRow(const uint32_t row) {
	for (size_t i = 0; i < _width; ++i) {
        CellState cellNextState = determineCellFate(*_frame[row][i]);
        _frame[row][i]->setNextState(cellNextState);
	}
}


void Field::updateFieldRow(const uint32_t row) {
	for (size_t j = 0; j < _width; ++j) {
		_frame[row][j]->commitFate();
	}
}

/*
    Implements the cycle of life
*/

void Field::generateNextGeneration() {
	for (size_t row = 0; row < _height; ++row) {
		prepareFieldRow(row);
	}

	for (size_t row = 0; row < _height; ++row) {
		updateFieldRow(row);
	}
}

void Field::startGame(const uint32_t generations) {

	for (size_t gen = 0; gen < generations; ++gen) {
        if (_shouldRenderEveryGeneration) {
		    _renderer.renderFrame();
		    _renderer.showFrame();
        }
		generateNextGeneration();
	}

    _renderer.renderFrame();
    _renderer.showFrame();
}

/*
    Calculates how many alive cells are around the given cell.
    Counting vertically, horizontally and diagonally with wrapping logic.
*/
uint32_t Field::countSurroundingLiveCells(const Cell &cell) const{

    WrappingUint32 cellPosX(cell.getPosX(), _width - 1);
    WrappingUint32 cellPosY(cell.getPosY(), _height - 1);
	uint32_t aliveSurroundingCount = 0;

    // anchor to upper left corner;
    uint32_t startingX = --cellPosX;
    --cellPosY;

	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			// we count in the target cell too if it is alive. it is removed after the counting process
      
			if (_frame[cellPosY][cellPosX]->getCurrentState() == CellState::alive) {
				++aliveSurroundingCount;
			}
            ++cellPosX;
		}
            
        cellPosX = startingX;
        ++cellPosY;
	}

	if (CellState::alive == cell.getCurrentState()) {
		// if the target cell is alive we have counted it too
		--aliveSurroundingCount;
	}
	return aliveSurroundingCount;
}

/*
    Determines what state the cell will be in the next generation
    by Conway's rules
*/
CellState Field::determineCellFate(const Cell &cell) {

	uint32_t aliveSurrounding = countSurroundingLiveCells(cell);

	// Conway's logic

	if (cell.getCurrentState() == CellState::dead) { // more likely to be dead
		if (aliveSurrounding == 3) {
			return CellState::alive;
		}
		return CellState::dead;
	} else {
		if (aliveSurrounding < 2) { // most likely to have less alive surrounding
			return CellState::dead;
		} else if (aliveSurrounding < 4) {
			return CellState::alive;
		} else {
			return CellState::dead;
		}
	}

	// this should N E V E R be reached
	assert(false);
	return CellState::dead;
}


Error Field::makeCellAlive(const uint32_t posX, const uint32_t posY) {
    if (posX >= _width || posY >= _height) {
        return Error::InvalidPositionForCell;
    }

    _frame[posY][posX]->setCurrentState(CellState::alive);
    return Error::None;
}
