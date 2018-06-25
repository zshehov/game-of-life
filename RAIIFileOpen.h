#pragma once
#include <string>
#include <fstream>

/*
    Utility class that wraps the opening of a stream so that
    it is certain it will be closed in all cases
*/
class RAIIFileOpen {

public:
    RAIIFileOpen(const std::string &fileName) :
        _openedStream(fileName, std::ios::binary | std::ios::in) {
    }

    ~RAIIFileOpen() {
        if (_openedStream.is_open()) {
            _openedStream.close();
        }
    }

    std::ifstream& getStream() {
        return _openedStream;
    }
private:
    std::ifstream _openedStream;
};
