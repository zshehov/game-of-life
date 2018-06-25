#pragma once
#include <stdint.h>


/*
    Utility class that abstracts out the wrapping logic
*/
class WrappingUint32 {
public:
    WrappingUint32(const uint32_t currentValue,
        const uint32_t upperBoundary) :
        _currentValue(currentValue),
        _upperBoundary(upperBoundary) {
    }

    uint32_t &operator--() {
        if (_currentValue == 0) {
            _currentValue = _upperBoundary;
        } else {
            --_currentValue;
        }
        return _currentValue;
    }

    uint32_t &operator++() {
        if (_currentValue >= _upperBoundary) {
            _currentValue = 0;
        } else {
            ++_currentValue;
        }
        return _currentValue;
    }

    uint32_t &operator=(const uint32_t &rhs) {
        _currentValue = rhs;
        return _currentValue;
    }

    operator uint32_t() {
        return _currentValue;
    }

private:
    uint32_t _currentValue;
    const uint32_t _upperBoundary;
};
