#include <vector>
#include <string>
#include <fstream>
class FieldCreator {


public:
    struct Position {
        uint32_t posX;
        uint32_t posY;
    };

    static std::vector<Position> positions;

    static bool writeToFile(const std::string &fileName) {

        std::ofstream ofs(fileName, std::ios::binary | std::ios::out | std::ios::trunc);

        if (!ofs.is_open()) {
            // caller is responsible for error messages
            return false;
        }

        ofs.write((char*)&FieldCreator::positions[0], positions.size() * sizeof(Position));
        ofs.close();

        return true;
    }


private:


};