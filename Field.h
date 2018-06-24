#pragma once
#include <string>
#include "Cell.h"
#include "Renderer.h"
#include "Barrier.h"
#include "Error.h"
#include <stdint.h>

class Field {

public:

	Field(const uint32_t width,
		  const uint32_t height,
		  const uint32_t threadCount);

	~Field();

	CellState determineCellFate(Cell &cell);

	uint32_t getWidth() const {
		return _width;
	}

    uint32_t getHeight() const {
        return _height;
    }

    Error makeCellAlive(const uint32_t posX, const uint32_t posY);

	
	
	void show() {
		_renderer.renderFrame();
	}
	
	void startGame(const uint32_t generations);

	// only single thread enables rendering for now
	void startGameSingleThread(const uint32_t generations);

	void prepareFieldRow(const uint32_t row);
	void updateFieldRow(const uint32_t row);
private:

	void generateNextGeneration(const uint32_t startRow,const uint32_t generations);
	void generateNextGenerationSingleThread();

	friend void Renderer::renderFrame();

	uint32_t countSurroundingLiveCells(Cell &cell);

	Cell ***_frame;
	const uint32_t _width;
	const uint32_t _height;

	Renderer _renderer;

	const uint32_t _threadCount;
	ThreadBarrier _prepareBarrier;
	ThreadBarrier _updateBarrier;
};
