#pragma once
#include "Cell.h"
#include "Error.h"
#include <vector>
#include <stdint.h>
class Field; // forward declaration


/*
    Abstract class used for extracting away the logic for
    configuring the field
*/

class GameConfigurator {

public:
    GameConfigurator(const uint32_t fieldWidth = 2,
                     const uint32_t fieldHeight = 2) :
        _fieldWidth(fieldWidth),
        _fieldHeight(fieldHeight)  {    
    }

    uint32_t getFieldWidth() const { return _fieldWidth; }
    uint32_t getFieldHeight() const { return _fieldHeight; }

    void setFieldWidth(const uint32_t fieldWidth) { _fieldWidth = fieldWidth; }
    void setFieldHeigth(const uint32_t fieldHeight) { _fieldWidth = fieldHeight; }

    virtual Error populateField(Field&) = 0;

protected:
    uint32_t _fieldWidth;
    uint32_t _fieldHeight;
};
