#include "stdafx.h"
#include "CppUnitTest.h"
#include "../GameOfLife/CellByCellGameConfigurator.h"
#include "../GameOfLife/Field.h"
#include "../GameOfLife/CellPosition.h"
#include "../GameOfLife/FieldBuilder.h"
#include "../GameOfLife/WrappingUint32.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests {

    TEST_CLASS(ConfigureFieldCellByCell) {

        TEST_METHOD(Should_BuildField_When_GivenConfigWithWidthAndHeightGreaterThan1) {
            ::CellByCellGameConfigurator config(2, 2);
            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            Assert::IsTrue(field != nullptr);
        }

        TEST_METHOD(Should_FailOnBuildField_When_GivenConfigWitHeightLessThan2) {
            ::CellByCellGameConfigurator config(2, 1);
            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            Assert::IsTrue(field == nullptr);
        }

        TEST_METHOD(Should_FailOnBuildField_When_GivenConfigWithWidthLessThan2) {
            ::CellByCellGameConfigurator config(1, 2);
            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            Assert::IsTrue(field == nullptr);
        }

        TEST_METHOD(Should_ReturnError_When_AddingCellOutOfFieldOnX) {
            ::CellByCellGameConfigurator config(5, 5);
            Error result = config.addAliveCell(Position(6, 2));
            Assert::IsTrue(result == Error::InvalidPositionForCell);
        }

        TEST_METHOD(Should_ReturnError_When_AddingCellOutOfFieldOnY) {
            ::CellByCellGameConfigurator config(5, 5);
            Error result = config.addAliveCell(Position(2, 6));
            Assert::IsTrue(result == Error::InvalidPositionForCell);
        }
    };


    TEST_CLASS(Field) {

        TEST_METHOD(Should_MakeCellAlive_When_AddingCellWithinField) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(2, 2));

            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            config.populateField(*field);

            Assert::IsTrue(field->getCellState(2, 2) == CellState::alive);
        }

        TEST_METHOD(Should_DetermineAliveCellToLive_When_ThereAre3SurroundingAliveCells) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(2, 2));
            config.addAliveCell(Position(3, 2));
            config.addAliveCell(Position(1, 2));
            config.addAliveCell(Position(2, 3));

            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(2, 2));

            Assert::IsTrue(cellState == CellState::alive);
        }

        TEST_METHOD(Should_DetermineAliveCellToLive_When_ThereAre2SurroundingAliveCells) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(2, 2));
            config.addAliveCell(Position(3, 2));
            config.addAliveCell(Position(1, 2));

            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(2, 2));

            Assert::IsTrue(cellState == CellState::alive);
        }



        TEST_METHOD(Should_DetermineCellToDie_When_ThereIs1SurroundingCells) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(2, 2));
            config.addAliveCell(Position(3, 2));

            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(2, 2));

            Assert::IsTrue(cellState == CellState::dead);
        }


        TEST_METHOD(Should_DetermineCellToDie_When_ThereAre4SurroundingCells) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(2, 2));
            config.addAliveCell(Position(3, 2));
            config.addAliveCell(Position(1, 2));
            config.addAliveCell(Position(2, 3));
            config.addAliveCell(Position(1, 3));

            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(2, 2));

            Assert::IsTrue(cellState == CellState::dead);
        }

        TEST_METHOD(Should_DetermineDeadCellToLive_When_ThereAre3SurroundingAliveCells) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(2, 2));
            config.addAliveCell(Position(3, 2));
            config.addAliveCell(Position(1, 2));

            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(2, 3));

            Assert::IsTrue(cellState == CellState::alive);
        }

        TEST_METHOD(Should_DetermineDeadCellToLive_When_ThereAre3SurroundingAliveCellsWithBottomWrapping) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(2, 0));
            config.addAliveCell(Position(3, 0));
            config.addAliveCell(Position(1, 0));


            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(2, 4));

            Assert::IsTrue(cellState == CellState::alive);
        }


        TEST_METHOD(Should_DetermineDeadCellToLive_When_ThereAre3SurroundingAliveCellsWithTopWrapping) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(2, 4));
            config.addAliveCell(Position(3, 4));
            config.addAliveCell(Position(1, 4));


            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(2, 0));

            Assert::IsTrue(cellState == CellState::alive);
        }

        TEST_METHOD(Should_DetermineDeadCellToLive_When_ThereAre3SurroundingAliveCellsWithRightWrapping) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(0, 1));
            config.addAliveCell(Position(0, 2));
            config.addAliveCell(Position(0, 3));


            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(4, 2));

            Assert::IsTrue(cellState == CellState::alive);
        }


        TEST_METHOD(Should_DetermineDeadCellToLive_When_ThereAre3SurroundingAliveCellsWithLeftWrapping) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(4, 1));
            config.addAliveCell(Position(4, 2));
            config.addAliveCell(Position(4, 3));


            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(0, 2));

            Assert::IsTrue(cellState == CellState::alive);
        }

        TEST_METHOD(Should_DetermineDeadCellToLive_When_ThereAre3SurroundingAliveCellsWithLeftAndTopWrapping) {
            ::CellByCellGameConfigurator config(5, 5);
            config.addAliveCell(Position(0, 4));
            config.addAliveCell(Position(4, 0));
            config.addAliveCell(Position(4, 1));


            ::FieldBuilder fieldBuilder(config, false);
            ::Field *field = fieldBuilder.getField();

            CellState cellState = field->determineCellFate(field->getCellAt(0, 0));

            Assert::IsTrue(cellState == CellState::alive);
        }


    };


    TEST_CLASS(WrappingUint32) {

        TEST_METHOD(Should_HaveValue3_When_InitializedWithValue3) {
            ::WrappingUint32 wrap(3, 10);
       
            Assert::IsTrue(wrap == 3);
        }

        TEST_METHOD(Should_WrapTo0_When_IncrementedAtUpperBoundary) {
            ::WrappingUint32 wrap(9, 10);
            ++wrap;
            ++wrap;

            Assert::IsTrue(wrap == 0);
        }

        TEST_METHOD(Should_WrapToUpperBoundary_When_DecrementedAt0) {
            ::WrappingUint32 wrap(1, 10);
            --wrap;
            --wrap;

            Assert::IsTrue(wrap == 10);
        }
    };
}
