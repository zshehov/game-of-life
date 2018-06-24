#include <time.h>
#include <string>
#include <stdio.h>
#include "Field.h"
#include "FieldCreator.h"
#include "CellByCellGameConfigurator.h"

#define _THREADS_ 4

int main(int argc, char *argv[]) {


    CellByCellGameConfigurator config(128, 65);
    config.addAliveCell({ 4, 8 });
    config.addAliveCell({ 5, 8 });
    config.addAliveCell({ 8, 8 });
    config.addAliveCell({ 9, 8 });
    config.addAliveCell({ 10, 8 });
    config.addAliveCell({ 5, 6 });
    config.addAliveCell({ 7, 7 });


    Field field(config.getFieldWidth() , config.getFieldHeight(), _THREADS_);

    config.populateField(field);


	clock_t start = clock();
//	field.startGame(100);
	field.startGameSingleThread(1);
	printf("time passed: %f\n", (float)(clock() - start) / CLOCKS_PER_SEC);
//	field.show();
}

