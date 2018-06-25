#include <iostream>
#include "FromFileGameConfigurator.h"
#include "RAIIFileOpen.h"


/*
    Populates the field using the file given at the construction of the configurator
*/
Error FromFileGameConfigurator::populateField(Field & field) {

    RAIIFileOpen fileStream(_configFileName);
    std::ifstream &fieldFileStream = fileStream.getStream();

    uint32_t readBytes = 0;
    uint32_t readPositions = 0;
    if (!fieldFileStream.is_open()) {
        return Error::ConfigFileFail;
    }

    Position positionsBuffer[512];

    while (true) {
        /*
        reading is not put in the condition because read operation returns
        false when it reaches EOF. This means that it would skip the content
        of the buffer that was filled when EOF was reached. Terminating logic
        is moved in the loop and checks if we have read 0 bytes.
        */
        fieldFileStream.read((char*)&positionsBuffer, sizeof(positionsBuffer));

        // gcount returns long long, we don't need that in this case
        readBytes = (uint32_t)fieldFileStream.gcount();
        if (readBytes == 0) {
            break;
        }
        readPositions = readBytes / sizeof(Position);
        for (size_t i = 0; i < readPositions; ++i) {
            printf("adding\n");
            Error err = field.makeCellAlive(positionsBuffer[i].posX, positionsBuffer[i].posY);
            if (err == Error::InvalidPositionForCell) {
                std::cerr << "Alive cell with coordinates: " << positionsBuffer[i].posX << ", "
                    << positionsBuffer[i].posY << " was not added: Out of field\n";
            }
        }
    }
}
