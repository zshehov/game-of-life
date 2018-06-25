#include "Renderer.h"
#include "Field.h"

void Renderer::renderFrame() {

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