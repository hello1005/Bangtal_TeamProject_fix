/*
#include <bangtal.h>
#include <stdio.h>

SceneID testScene;
ObjectID test1, test2;

void mouseCallback_test(ObjectID obj, int x, int y, MouseAction action) {
	if (obj == test1) {
		printf("Pressed Price 1 \n");
	}
	else if (obj == test2) {
		printf("Pressed Price 2 \n");
	}
}

int main(void) {
	setMouseCallback(mouseCallback_test);

	testScene = createScene("testScene", "./Images/Backgrounds/Store_Inside.png");

	test1 = createObject("./Images/Items/Store/UI/Price1.png");
	locateObject(test1, testScene, 0, 0);
	showObject(test1);
	
	test2 = createObject("./Images/Items/Store/UI/Price2.png");
	locateObject(test2, testScene, 0, 0);
	showObject(test2);
	
	startGame(testScene);
}
*/
