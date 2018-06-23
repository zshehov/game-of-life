#include "Renderer.h"
#include "Field.h"


void Renderer::renderFrame() {
	// this function can be mixed with actually updating the next frame since now there are 3 passes:
	// 1) determine next state
	// 2) update current state
	// 3) render                       -> it could be 2) render and update current state

	for (size_t i = 0; i < _fieldHeight; i++) {
		for (size_t j = 0; j < _fieldWidth; j++) {
			if (_field->_frame[i][j]->getCurrentState() == CellState::alive) {
				_buffer[(_fieldWidth + 1) * i + j] = '*';
			} else {
				_buffer[(_fieldWidth + 1) * i + j] = '-';
			}
		}
		_buffer[(_fieldWidth + 1) * i + _fieldWidth] = '\n';

	}

	_buffer[(_fieldWidth + 1) * _fieldHeight] = '\0';
}