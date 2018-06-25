#pragma once
#include "GameConfigurator.h"
#include "Field.h"

/*
    FuildBuilder is responsible for abstracting out the logic for
    creating a field. It guarantees that a created field will
    always be valid
*/
class FieldBuilder {

public:
    FieldBuilder(GameConfigurator &gameConfigurator) : _field(nullptr) {
        if (gameConfigurator.getFieldHeight() > 1 &&
            gameConfigurator.getFieldWidth() > 1) {
            _field = new Field(gameConfigurator.getFieldWidth(), gameConfigurator.getFieldHeight());
            gameConfigurator.populateField(*_field);
        }
    }

    ~FieldBuilder() {
        delete _field;
    }


    Field* getField() {
        return _field;
    }

private:
    Field *_field;
};
