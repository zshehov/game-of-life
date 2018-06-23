#include "Cell.h"
#include "Field.h"

CellState Cell::getCurrentState() const
{
	return _currentState;
}

CellState Cell::getNextState() const
{
	return _nextState;
}

void Cell::exist() {
	_nextState =  _environment->determineCellFate(*this);
}