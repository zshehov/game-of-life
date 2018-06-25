#pragma once
#include "GameConfigurator.h"
#include "Field.h"
#include "Error.h"
#include "CellPosition.h"
#include <string>

/*
    Responsible for creating a configuration for the field from a file  
*/

class FromFileGameConfigurator : public GameConfigurator {

public:
    FromFileGameConfigurator(const std::string &configFileName,
                             const uint32_t fieldWidth,
                             const uint32_t fieldHeight) :
        GameConfigurator(fieldWidth, fieldHeight),
        _configFileName(configFileName) {
    }

    // from GameConfigurator abstract class
    Error populateField(Field& field) override;

private:
    std::string _configFileName;
};

