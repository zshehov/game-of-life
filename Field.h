#pragma once
#include <string>
#include "Cell.h"
#include "Renderer.h"
#include "Barrier.h"
#include "Error.h"
#include <stdint.h>

class Field {

public:

    ~Field();

	CellState determineCellFate(Cell &cell);

	uint32_t getWidth() const {
		return _width;
	}

    uint32_t getHeight() const {
        return _height;
    }

    // only for tests
    CellState getCellState(const uint32_t posX, const uint32_t posY) const {
        return _frame[posY][posX]->getCurrentState();
    }

    Error makeCellAlive(const uint32_t posX, const uint32_t posY);


	// only single thread enables rendering for now
	void startGame(const uint32_t generations);

	void prepareFieldRow(const uint32_t row);
	void updateFieldRow(const uint32_t row);

    // since we can't create Field with its constructor
    friend class FieldBuilder;
private:
    Field(const uint32_t width,
          const uint32_t height);

	void generateNextGeneration();

	friend void Renderer::renderFrame();

	uint32_t countSurroundingLiveCells(Cell &cell) const;

	Cell ***_frame;
	const uint32_t _width;
	const uint32_t _height;

	Renderer _renderer;
};
