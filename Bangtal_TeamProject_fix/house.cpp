// Bangtal Library, Team Project
// 20200690 JinCheol Park, Code about House 

// ====================================================================================

// Information about images:
// Backgrounds
// Characters
// Enemies
// Items
// Messages
// Numbers
// Structures

// Information about points at village:
// x of Structures: 150, 750
// y of Strucutres: 195

// ====================================================================================

// Preprocessor
#include <bangtal.h>
#include <stdio.h>

// For debugging, set this variable true.
const bool DEBUGGING = true;

// Timer for animation
// Seconds message last: messageCnt x messageTime
// Ex: Showing message with messageCnt = 1, messageTime = 1 -> Show message 1 seconds then hide.
TimerID timer;
ObjectID messageObj;
const Second messageTime = 1.0f;
int messageCnt = 0;

// Scenes and Objects.
SceneID villageScene;
ObjectID house, casino;
/*
SceneID towerScene, battleScene, gameOverScene;
ObjectID tower, store;
ObjectID items[20];
ObjectID slotMachine;
ObjectID fruits[9];
*/

/*
// Characters (Not Implemented)
ObjectID player;
int playerX, playerY;
int playerHp, playerMaxHp, playerAtk, playerDef;
*/

// Start at Day1, End at Day30
int day = 1;
const int DAY_LIMIT = 30;
bool isNight = false;

// ====================================================================================

// Functions
void changeToNight(void);
void rest(void);
void mouseCallback(ObjectID obj, int x, int y, MouseAction action);
void timerCallback(TimerID timer);
void keyboardCallback(KeyCode code, KeyState state);

// ====================================================================================

void changeToNight(void) {
	// Change a time to night.

	if (not isNight) {
		isNight = true;
		setSceneImage(villageScene, "./Images/Backgrounds/NightTime.png");
	}
}

void rest(void) {
	// Change a day to tomorrow.

	if (isNight) {
		// If time is a night, set player's hp max and change a day.

		if (day < DAY_LIMIT) {
			// Set player's hp max. (Not Implemented)
			// playerHp = playerMaxHp;

			// Change a day.
			day += 1;

			setSceneImage(villageScene, "./Images/Backgrounds/DayTime.png");
			isNight = false;

			if (DEBUGGING) {
				// For debugging, print a result at console.
				printf("Day: %d -> %d \n", day - 1, day);
			}

		}
		else {
			// At DAY_LIMIT, enter scene to game over.
			// enterScene(gameOverScene);
		}
	}
	else {
		// If time is not a night, show message about using house.
		ObjectID canNotRest = createObject("./Images/Messages/CanNotRest.png");
		locateObject(canNotRest, villageScene, 0, 0);
		showObject(canNotRest);

		// Using animations.
		messageCnt = 1;
		messageObj = canNotRest;
		startTimer(timer);
		setTimer(timer, messageTime);
	}
}

void mouseCallback(ObjectID obj, int x, int y, MouseAction action) {
	// Processing a mouse action.
	// If player clicked a structures, then player go to there.

	if (obj == casino) {
		// If player clicked a casino, then time changes to night.
		changeToNight();
	}
	else if (obj == house) {
		// If player clicked a house, then player rests at house.
		rest();
	}
}

void timerCallback(TimerID timer) {
	// Processing an animation.

	if (messageCnt > 0) {
		// Processing a message.

		messageCnt -= 1;
		if (messageCnt == 0) {
			hideObject(messageObj);
		}

		startTimer(timer);
		setTimer(timer, messageTime);
	}
}

void keyboardCallback(KeyCode code, KeyState state) {
	// Processing a keyboard input.
	// If player pressed an Up arrow at structures, then player go there. (Not Implemented) 
}

// ====================================================================================

int main(void) {
	// Callbacks
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	// setKeyboardCallback(keyboardCallback);

	// Timer for animation
	timer = createTimer();

	// Scenes
	villageScene = createScene("Village", "./Images/Backgrounds/DayTime.png");

	// Casino
	casino = createObject("./Images/Structures/Casino.png");
	locateObject(casino, villageScene, 150, 195);
	showObject(casino);

	// House
	house = createObject("./Images/Structures/House.png");
	locateObject(house, villageScene, 750, 195);
	showObject(house);

	// Start a game
	startGame(villageScene);
}


