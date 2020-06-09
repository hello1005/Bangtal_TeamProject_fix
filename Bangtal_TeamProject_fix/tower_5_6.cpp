// Bangtal Library, Team Project
// 20200690 JinCheol Park, Code about Tower (5th, 6th floor)

// ====================================================================================

// Preprocessor
#include <bangtal.h>
#include <stdio.h>

// For debugging, set this variable true.
const bool DEBUGGING = true;

// Animation for character's movement
TimerID moveTimer;
const Second MOVE_TICK = 0.01f;
const int MOVE_SPEED = 10;

// Animation for immune state
TimerID immuneTimer;
const Second IMMUNE_TICK = 0.05f;
const int IMMUNE_TIME = 20;

// Animation for attack (Test)
TimerID testAttackTimer;
const Second TEST_TICK = 0.01f;
const int GRAVITY = 1;

// Const variable for turn
const int PLAYER = 0;
const int ENEMY = 1;

// ====================================================================================

// Scenes and Objects
// Scenes
SceneID towerScene, battleScene;
SceneID currentScene;

// Player
ObjectID player;
int playerX, playerY;
int dx, dy;
const int playerX_MIN = 50, playerX_MAX = 1150;
const int playerY_FIXED = 175;

ObjectID playerIcon;
ObjectID playerHpBar;
int iconX, iconY;
int iconDx, iconDy;
const int iconX_MIN = 333, iconX_MAX = 943, iconX_SIZE = 40;
const int iconY_MIN = 150, iconY_MAX = 320, iconY_SIZE = 70;
const int playerHpBarX_FIXED = 333, playerHpBarY_FIXED = 100;

int playerHp, playerMaxHp, playerAtk, playerDef;

// Enemy
// enemyX: depending on size (500 ~ 600)
ObjectID enemy;
ObjectID enemyHpBar;
int enemyX, enemyY;
int enemyHp, enemyMaxHp, enemyAtk, enemyDef;
const int enemyY_FIXED = 410;
const int enemyHpBarX_FIXED = 333, enemyHpBarY_FIXED = 660;

ObjectID enemyTestAttack;
int testAttackX = 550, testAttackY = 500;
int dyTest = 0;

// Turn
int turn = PLAYER;

// Gold
int gold = 359;
ObjectID goldList[3];
int goldX[3] = { 1040, 1067, 1094 };
int goldY = 595;

// ====================================================================================

// Functions
void gameInit(void);

void playerMove(void);
void playerIconMove(void);

void battle(void);
bool checkCollision(int xStart, int xEnd, int yStart, int yEnd);
void checkHp(int kind);

void testAttack(void);

void showGold(void);

void timerCallback(TimerID timer);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void keyboardCallback(KeyCode code, KeyState state);

// ====================================================================================

// Implementations
void gameInit(void) {
	// Initialize a game with setting variable.

	// Timers for animation
	moveTimer = createTimer(MOVE_TICK);
	startTimer(moveTimer);

	// Timers for attack (Test)
	testAttackTimer = createTimer(TEST_TICK);

	// Scenes
	towerScene = createScene("towerScene", "./Images/Backgrounds/Tower_Inside.png");
	battleScene = createScene("battleScene", "./Images/Backgrounds/Battle.png");
	currentScene = towerScene;

	// Player
	player = createObject("./Images/Characters/Warrior_R.png");
	scaleObject(player, 0.5f);
	playerX = 1150;
	playerY = playerY_FIXED;
	locateObject(player, currentScene, playerX, playerY);
	showObject(player);

	playerIcon = createObject("./Images/Characters/Warrior_I.png");
	iconX = 943;
	iconY = 320;
	locateObject(playerIcon, battleScene, iconX, iconY);
	showObject(playerIcon);

	playerHp = 100;
	playerMaxHp = 100;
	playerAtk = 5;
	playerDef = 3;

	playerHpBar = createObject("./Images/UI/Battle/HP/Hp_100%.png");
	locateObject(playerHpBar, battleScene, playerHpBarX_FIXED, playerHpBarY_FIXED);
	showObject(playerHpBar);

	// Enemy
	enemy = createObject("./Images/Enemies/TestEnemy.png");
	enemyX = 530;
	enemyY = enemyY_FIXED;
	enemyHp = 50;
	enemyMaxHp = 50;
	enemyAtk = 10;
	enemyDef = 3;

	enemyHpBar = createObject("./Images/UI/Battle/Hp/Hp_10%.png");
	locateObject(enemyHpBar, battleScene, enemyHpBarX_FIXED, enemyHpBarY_FIXED);
	showObject(enemyHpBar);

	locateObject(enemy, battleScene, enemyX, enemyY);
	showObject(enemy);

	// Gold
	goldList[0] = createObject("./Images/Numbers/0_R.png");
	goldList[1] = createObject("./Images/Numbers/1_R.png");
	goldList[2] = createObject("./Images/Numbers/2_R.png");

	for (int i = 0; i < 3; i++) {
		scaleObject(goldList[i], 0.8f);
		locateObject(goldList[i], battleScene, goldX[i], goldY);
	}

	showGold();
}

// ====================================================================================

void playerMove(void) {
	// Move a player. (towerScene)

	playerX += dx;

	// Bug fix:
	// Fixed a bug - Difference between player's direction and image.
	// Change an image with player's direction.
	if (dx < 0) {
		setObjectImage(player, "./Images/Characters/Warrior_L.png");
	}
	else if (dx > 0) {
		setObjectImage(player, "./Images/Characters/Warrior_R.png");
	}

	if (dx < -MOVE_SPEED) {
		dx = -MOVE_SPEED;
	}
	else if (dx > MOVE_SPEED) {
		dx = MOVE_SPEED;
	}
	if (dy < -MOVE_SPEED) {
		dy = -MOVE_SPEED;
	}
	else if (dy > MOVE_SPEED) {
		dy = MOVE_SPEED;
	}
	
	// Set a restriction.
	if (playerX < playerX_MIN) {
		playerX = playerX_MIN;
	}
	else if (playerX > playerX_MAX) {
		playerX = playerX_MAX;
	}
	
	locateObject(player, currentScene, playerX, playerY);

	setTimer(moveTimer, MOVE_TICK);
	startTimer(moveTimer);
}

void playerIconMove(void) {
	// Move a player-icon. (battleScene)

	// Bug fix:
	// Fixed a bug.
	if (iconDx < -MOVE_SPEED) {
		iconDx = -MOVE_SPEED;
	}
	else if (iconDx > MOVE_SPEED) {
		iconDx = MOVE_SPEED;
	}
	if (iconDy < -MOVE_SPEED) {
		iconDy = -MOVE_SPEED;
	}
	else if (iconDy > MOVE_SPEED) {
		iconDy = MOVE_SPEED;
	}
	
	
	// Set a restriction.
	iconX += iconDx;
	if (iconX < iconX_MIN) {
		iconX = iconX_MIN;
	}
	else if (iconX > iconX_MAX) {
		iconX = iconX_MAX;
	}

	iconY += iconDy;
	if (iconY < iconY_MIN) {
		iconY = iconY_MIN;
	}
	else if (iconY > iconY_MAX) {
		iconY = iconY_MAX;
	}
	
	locateObject(playerIcon, currentScene, iconX, iconY);

	setTimer(moveTimer, MOVE_TICK);
	startTimer(moveTimer);
}

// ====================================================================================

void battle(void) {
	// Start a battle.
	// When battle ends: Player selects avoid, Player died, Enemy died.
}

bool checkCollision(int xStart, int xEnd, int yStart, int yEnd) {
	// Check a collsion, then return a result.
	
	// About Collision:
	// We can find 'collision' with position and size.
	// If enemy attack's X is about 400 ~ 600,
	// then X_MIN <= player's X <= 360 (360 can be replaced with 400 - SIZE) or 600 <= player's X <= X_MAX
	// If enemy attack's Y is about 230 ~ 300,
	// then Y_MIN <= player's Y <= 160 (160 can be replaced with 230 - SIZE) or 300 <= player's Y <= Y_MAX
	    
	// Conclusion:
	// If a <= attack's range <= b, then player will be hit when (a - SIZE) < player's position < b
	
	bool xCollision = ((xStart - iconX_SIZE) < iconX) and (iconX < xEnd);
	bool yCollision = ((yStart - iconY_SIZE) < iconY) and (iconY < yEnd);

	return xCollision and yCollision;
}

void checkHp(int kind) {
	// Check object's Hp and change Hp bar with their Hp percent.

	float hpPercent;
	ObjectID hpBar;
	if (kind == PLAYER) {
		hpPercent = static_cast<float>(playerHp) / static_cast<float>(playerMaxHp);
		hpBar = playerHpBar;
	}
	else if (kind == ENEMY) {
		hpPercent = static_cast<float>(enemyHp) / static_cast<float>(enemyMaxHp);
		hpBar = enemyHpBar;
	}
		
	if (0.75f < hpPercent <= 1.0f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_100%");
	}
	else if (0.5f < hpPercent <= 0.75f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_75%");
	}
	else if (0.25f < hpPercent <= 0.5f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_50%");
	}
	else if (0.1f < hpPercent <= 0.25f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_25%");
	}
	else if (0.0f < hpPercent <= 0.1f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_10%");
	}
	else {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_0%");
	}
}

void testAttack(void) {
	// Attack (test)

	testAttackX = 550;
	testAttackY = 500;
	dyTest = 0;

	enemyTestAttack = createObject("./Images/Enemies/TestAttack.png");
	locateObject(enemyTestAttack, battleScene, testAttackX, testAttackY);
	showObject(enemyTestAttack);

	printf("testAttack() \n");

	setTimer(testAttackTimer, TEST_TICK);
	startTimer(testAttackTimer);
}

void showGold(void) {
	// Shows a gold.

	int temp = gold;
	if (0 <= gold and gold < 10) {
		for (int i = 0; i <= 0; i++) {
			int num = temp % 10;
			temp /= 10;

			char imageName[30];
			sprintf_s(imageName, sizeof(imageName), "./Images/Numbers/%d_R.png", num);
			setObjectImage(goldList[i], imageName);
			showObject(goldList[i]);
		}

		for (int i = 1; i <= 2; i++) {
			hideObject(goldList[i]);
		}
	}
	else if (10 <= gold and gold < 100) {
		for (int i = 0; i <= 1; i++) {
			int num = temp % 10;
			temp /= 10;

			char imageName[30];
			sprintf_s(imageName, sizeof(imageName), "./Images/Numbers/%d_R.png", num);
			setObjectImage(goldList[1-i], imageName);
			showObject(goldList[1-i]);
		}

		for (int i = 2; i <= 2; i++) {
			hideObject(goldList[i]);
		}
	}
	else if (100 <= gold and gold < 1000) {
		for (int i = 0; i <= 2; i++) {
			int num = temp % 10;
			temp /= 10;

			char imageName[30];
			sprintf_s(imageName, sizeof(imageName), "./Images/Numbers/%d_R.png", num);
			setObjectImage(goldList[2-i], imageName);
			showObject(goldList[2-i]);
		}
	}
	else {
		// Not implemented when gold >= 1000.
		printf("showGold: Gold is over 999 \n");

		for (int i = 0; i < 3; i++) {
			setObjectImage(goldList[i], "./Images/Numbers/0_R.png");
			showObject(goldList[i]);
		}
	}
}

// ====================================================================================

void timerCallback(TimerID timer) {
	// Processing an animation.

	// If keyboard input, then player will move with moveTimer.
	if (timer == moveTimer) {
		if (currentScene == towerScene) {
			playerMove();
		}
		else if (currentScene == battleScene) {
			playerIconMove();
		}
	}
	else if (timer == testAttackTimer) {
		dyTest -= GRAVITY;
		testAttackY += dyTest;

		locateObject(enemyTestAttack, battleScene, testAttackX, testAttackY);

		if (testAttackY < 50) {
			hideObject(enemyTestAttack);
		}
		else {
			setTimer(testAttackTimer, TEST_TICK);
			startTimer(testAttackTimer);
		}
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == enemy) {
		testAttack();
	}
}

void keyboardCallback(KeyCode code, KeyState state) {
	// Processing a keyboard input.

	// 82: Left arrow-key
	if (code == 82) {
		if (currentScene == towerScene) {
			dx -= (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
		else if (currentScene == battleScene) {
			iconDx -= (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
	// 83: Right arrow-key
	else if (code == 83) {
		if (currentScene == towerScene) {
			dx += (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
		else if (currentScene == battleScene) {
			iconDx += (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
	// 84: Up arrow-key
	else if (code == 84) {
		// If current scene is tower and player can battle, then change a scene.
		if (currentScene == towerScene) {
			currentScene = battleScene;
			enterScene(currentScene);
		}
		// Else if current scene is battle, then player move to upper direction.
		else if (currentScene == battleScene) {
			iconDy = MOVE_SPEED;
			iconDy += (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
	// 85: Down arrow-key
	else if (code == 85) {
		if (currentScene == battleScene) {
			iconDy -= (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
}

// ====================================================================================

int main(void) {
	// Set callback functions.
	setTimerCallback(timerCallback);
	setMouseCallback(mouseCallback);
	setKeyboardCallback(keyboardCallback);

	// Start a game.
	gameInit();
	startGame(currentScene);
}