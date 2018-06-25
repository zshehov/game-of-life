#pragma once
#include <stdint.h>
#include <stdio.h>
class Field; // forward declaration


/*
    Renderer is responsible for rendering the field in a
    simple printf fashion
*/
class Renderer {
public:

    Renderer(const Field * field, const uint32_t fieldWidth, const uint32_t fieldHeight) :
        _field(field),
        _fieldWidth(fieldWidth),
        _fieldHeight(fieldHeight),
        // +1 is for the \n symbols
        _buffer(new char[(_fieldWidth + 1)* _fieldHeight + 1]) {
    }

    ~Renderer() {
        delete[] _buffer;
    }

    // possibly implement buffering

    void renderFrame();
    void showFrame() {
        printf(_buffer);
    }

private:
    const Field *_field;
    const uint32_t _fieldWidth;
    const uint32_t _fieldHeight;

    char *_buffer;
};