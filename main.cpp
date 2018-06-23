#include <time.h>
#include <string>
#include <stdio.h>
#include "Field.h"
#include "FieldCreator.h"

#define _THREADS_ 4

//#define _FIELD_CREATION_
const std::string fieldFileName = "field.map";

int main(int argc, char *argv[]) {
#ifdef _FIELD_CREATION_
	FieldCreator fieldCreator;
	//fieldCreator.addPosition({ 10,2 });
	fieldCreator.addPosition({ 8, 4 });
	fieldCreator.addPosition({ 8,5 });
	fieldCreator.addPosition({ 8,8 });
	fieldCreator.addPosition({ 8,9 });
	fieldCreator.addPosition({ 8,10 });
	fieldCreator.addPosition({ 6,5 });
	fieldCreator.addPosition({ 7,7 });
	fieldCreator.writeToFile(fieldFileName);
#else 

	Field field(fieldFileName, 128, 128, _THREADS_);

	clock_t start = clock();
//	field.startGame(100);
	field.startGameSingleThread(10000);
	printf("time passed: %f\n", (float)(clock() - start) / CLOCKS_PER_SEC);
//	field.show();
#endif
}

