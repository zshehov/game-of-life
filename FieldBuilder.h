#pragma once
#include "GameConfigurator.h"
#include "Field.h"


class FieldBuilder {

public:
    FieldBuilder(GameConfigurator &gameConfigurator) : _field(nullptr) {
        if (gameConfigurator.getFieldHeight() > 1 &&
            gameConfigurator.getFieldWidth() > 1) {
            _field = new Field(gameConfigurator.getFieldWidth(), gameConfigurator.getFieldWidth());
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
