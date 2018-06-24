#include "Field.h"
#include "Cell.h"
#include "WrappingUint32.h"
#include <vector>
#include <thread>
#include <assert.h>
#define _RENDER_

Field::Field(const uint32_t width,
			 const uint32_t height, 
			 const uint32_t threadCount) :
	_width(width),
	_height(height),
	_threadCount(threadCount),
	_prepareBarrier(_threadCount),
	_updateBarrier(_threadCount),
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

void Field::generateNextGeneration(const uint32_t startRow, const uint32_t generations) {	
	// going with jumps of _threadCount, so that workload is more spread out between the threads
	for (size_t gen = 0; gen < generations; ++gen) {

		for (size_t i = startRow; i < _height; i += _threadCount) {
			prepareFieldRow(i);
		}
		// no thread goes past this point until all threads have reached the barrier
		_prepareBarrier.wait(); 

		for (size_t i = startRow; i < _height; i += _threadCount) {
			updateFieldRow(i);
		}

		// no thread goes past this point until all threads have reached the barrier
		_updateBarrier.wait();
	}
}

void Field::generateNextGenerationSingleThread() {
	for (size_t row = 0; row < _height; ++row) {
		prepareFieldRow(row);
	}

	for (size_t row = 0; row < _height; ++row) {
		updateFieldRow(row);
	}
}

void Field::startGame(const uint32_t generations) {
	std::vector<std::thread> threads;
	for (size_t i = 0; i < _threadCount; ++i) {
		threads.push_back(std::thread([this, i, generations]() { this->generateNextGeneration(i, generations); }));
	}

	for (size_t i = 0; i < _threadCount; ++i) {
		threads[i].join();
	}
}

void Field::startGameSingleThread(const uint32_t generations) {
#ifdef _RENDER_
    _renderer.renderFrame();

#endif // _RENDER_

	for (size_t gen = 0; gen < generations; ++gen) {
#ifdef _RENDER_
		_renderer.showFrame();
#endif // _RENDER_
		generateNextGenerationSingleThread();


#ifdef _RENDER_
		_renderer.renderFrame();
#endif // _RENDER_
	}
}

uint32_t Field::countSurroundingLiveCells(Cell &cell) {

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

CellState Field::determineCellFate(Cell &cell) {

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
