#pragma once
#include "GameConfigurator.h"
#include "Field.h"
#include "Error.h"
#include "CellPosition.h"
#include <string>
#include <fstream>
#include <iostream>



class FromFileGameConfigurator : public GameConfigurator {
public:

    FromFileGameConfigurator(const std::string &configFileName,
                             const uint32_t fieldWidth,
                             const uint32_t fieldHeight) :
        GameConfigurator(fieldWidth, fieldHeight),
        _configFileName(configFileName) {

    }


    Error populateField(Field& field) override;


private:

    std::string _configFileName;
};

