#include <time.h>
#include <string>
#include <stdio.h>
#include "Field.h"
#include "FieldCreator.h"
#include "CellByCellGameConfigurator.h"
#include "FromFileGameConfigurator.h"

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

    FromFileGameConfigurator config2("field.map", 128, 65);




    Field field(config2.getFieldWidth() , config2.getFieldHeight(), _THREADS_);

    if (config2.populateField(field) == Error::ConfigFileFail) {

        printf("failed\n");
        return 1;

    }


	clock_t start = clock();
//	field.startGame(100);
	field.startGameSingleThread(1000);
	printf("time passed: %f\n", (float)(clock() - start) / CLOCKS_PER_SEC);
//	field.show();
}

