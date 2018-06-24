#include "Field.h"
#include "Cell.h"
#include <vector>
#include <thread>
#include <assert.h>
#include <fstream>
#define _RENDER_

Field::Field(const std::string & configurationFile,
			 const uint32_t width,
			 const uint32_t height, 
			 const uint32_t threadCount) :
	_width(width),
	_height(height),
	_threadCount(threadCount),
	_prepareBarrier(_threadCount),
	_updateBarrier(_threadCount),
	_renderer(this, _width, _height) {


	// open file

	_frame = new Cell**[height];
	for (size_t i = 0; i < height; ++i) {
		_frame[i] = new Cell*[width];
		for (size_t j = 0; j < width; ++j) {
			_frame[i][j] = new Cell(CellState::dead, i, j);
		}
	}

	if (!readFieldFromFile(configurationFile)) {
		printf("Couldn't read from configuration file. All cells will be dead\n");
	}
}

bool Field::readFieldFromFile(const std::string &fileName) {
	std::ifstream fieldFileStream(fileName, std::ios::binary | std::ios::in);
	uint32_t readBytes = 0;
	uint32_t readPositions = 0;
	if (!fieldFileStream.is_open()) {
		return false;
	}
	
	struct Position {
		uint32_t posX;
		uint32_t posY;
	};

	Position positionsBuffer[512];

	while (true) {
		/*
			reading is not put in the condition because read operation returns
			false when it reaches EOF. This means that it would skip the content
			of the buffer that was filled when EOF was reached. Terminating logic
			is moved in the loop and checks if we have read 0 bytes.
		*/
		fieldFileStream.read((char*)&positionsBuffer, sizeof(positionsBuffer));

		// gcount returns long long, we don't need that in this case
		readBytes = (uint32_t) fieldFileStream.gcount();
		if (readBytes == 0) {
			break;
		}
		readPositions = readBytes / sizeof(Position);
		for (size_t i = 0; i < readPositions; ++i) {
			_frame[positionsBuffer[i].posX][positionsBuffer[i].posY]->setCurrentState(CellState::alive);
			printf("alive\n");
		}
	}

	fieldFileStream.close();
	return true;
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
	for (size_t gen = 0; gen < generations; ++gen) {
#ifdef _RENDER_
        system("cls");
		_renderer.showFrame();
#endif // _RENDER_
		generateNextGenerationSingleThread();


#ifdef _RENDER_
		_renderer.renderFrame();
#endif // _RENDER_
	}
}

uint32_t Field::countSurroundingLiveCells(Cell &cell) {

	uint32_t cellPosX = cell.getPosX();
	uint32_t cellPosY = cell.getPosY();
	uint32_t aliveSurroundingCount = 0;

	// put the possible positions in a flat array for performance
	uint32_t positions[6];

	positions[1] = cellPosX;
	positions[4] = cellPosY;
	// some wrapping logic is put in
	positions[0] = cellPosX == 0 ? _height - 1 : cellPosX - 1;
	positions[3] = cellPosY == 0 ? _width - 1 : cellPosY - 1;
	positions[2] = cellPosX == _height - 1 ? 0 : cellPosX + 1;
	positions[5] = cellPosY == _width - 1 ? 0 : cellPosY + 1;

	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 3; j < 6; j++) {
			// we count in the target cell too if it is alive. it is removed after the counting process
			if (_frame[positions[i]][positions[j]]->getCurrentState() == CellState::alive) {
				++aliveSurroundingCount;
			}
		}
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
