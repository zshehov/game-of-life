#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "CellPosition.h"

class FieldCreator {


public:
    void addPosition(const Position &position) {
        positions.push_back(position);
    }

    bool writeToFile(const std::string &fileName) {

        std::ofstream ofs(fileName, std::ios::binary | std::ios::out | std::ios::trunc);

        if (!ofs.is_open()) {
            // caller is responsible for error messages
            return false;
        }

        ofs.write((char*)&positions[0], positions.size() * sizeof(Position));
        ofs.close();

        return true;
    }

private:
    std::vector<Position> positions;
};
