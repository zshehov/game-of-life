#pragma once
#include <stdint.h>
#include "CellPosition.h"
enum class CellState {
	alive,
	dead,
};

/**
Lifecycle of a cell:
	1) exist - determine the status of the cell on the next cycle
	2) commitFate - fulfill its destiny. a.k.a _currentState becomes what it was fated to become
*/

class Cell {

public:

	Cell(const CellState &state, uint32_t posX, uint32_t posY) :
		_currentState(state),
		_nextState(CellState::dead),
		_position(posX, posY) {}


    CellState getCurrentState() const {
        return _currentState;
    }
    CellState getNextState() const {
        return _nextState;
    }

	void commitFate() {
		_currentState = _nextState;
	}

	uint32_t getPosX() const {
		return _position.posX;
	}

	uint32_t getPosY() const {
		return _position.posY;
	}

	void setCurrentState(const CellState &state) {
		_currentState = state;
	}

    void setNextState(const CellState &state) {
        _nextState = state;
    }

private:

	CellState _currentState;
	CellState _nextState;

	Position _position;


};
