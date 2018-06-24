#include "stdafx.h"
#include "CppUnitTest.h"
#include "../GameOfLife/CellByCellGameConfigurator.h"
#include "../GameOfLife/Field.h"
#include "../GameOfLife/CellPosition.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests {

    TEST_CLASS(ConfigureFieldCellByCell) {

        TEST_METHOD(Should_MakeCellAlive_When_AddingCellWithinField) {
            ::CellByCellGameConfigurator config(5, 5);

            ::Field field(config.getFieldWidth(), config.getFieldHeight(), 1);

            config.addAliveCell(Position(2, 2));

            config.populateField(field);

            Assert::IsTrue(field.getCellState(2, 2) == CellState::alive);
        }


        TEST_METHOD(Should_ReturnError_When_AddingCellOutOfFieldOnX) {
            ::CellByCellGameConfigurator config(5, 5);

            ::Field field(config.getFieldWidth(), config.getFieldHeight(), 1);

            Error result = config.addAliveCell(Position(6, 2));
            Assert::IsTrue(result == Error::InvalidPositionForCell);

        }

        TEST_METHOD(Should_ReturnError_When_AddingCellOutOfFieldOnY) {
            ::CellByCellGameConfigurator config(5, 5);

            ::Field field(config.getFieldWidth(), config.getFieldHeight(), 1);

            Error result = config.addAliveCell(Position(2, 6));
            Assert::IsTrue(result == Error::InvalidPositionForCell);
        }
    };


    TEST_CLASS(Field) {

        TEST_METHOD(Should_) {
            // TODO: Your test code here
        }

    };
}