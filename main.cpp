#include <time.h>
#include <string>
#include <stdio.h>
#include "Field.h"
#include "FieldCreator.h"
#include "CellByCellGameConfigurator.h"
#include "FromFileGameConfigurator.h"
#include "FieldBuilder.h"

#define _THREADS_ 4

int main(int argc, char *argv[]) {


    CellByCellGameConfigurator config(128, 65);

    config.addAliveCell(Position( 4, 8 ));
    config.addAliveCell(Position( 5, 8 ));
    config.addAliveCell(Position( 8, 8 ));
    config.addAliveCell(Position( 9, 8 ));
    config.addAliveCell(Position( 10, 8 ));
    config.addAliveCell(Position( 5, 6 ));
    config.addAliveCell(Position( 7, 7 ));


    FieldBuilder fieldBuilder(config);

    Field *field = fieldBuilder.getField();
    if (field == nullptr) {
        printf("Couldn't create field\n");
        return 1;
    }


	clock_t start = clock();
//	field.startGame(100);
	field->startGameSingleThread(1000);
	printf("time passed: %f\n", (float)(clock() - start) / CLOCKS_PER_SEC);
//	field.show();
}

