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
int dxIcon, dyIcon;
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

// Turn
int turn = PLAYER;

// ====================================================================================

// Functions
void gameInit(void);

void playerMove(void);
void playerIconMove(void);

void battle(void);
bool checkCollision(int xStart, int xEnd, int yStart, int yEnd);
void checkHp(int kind);

void timerCallback(TimerID timer);
void keyboardCallback(KeyCode code, KeyState state);

// ====================================================================================

// Implementations
void gameInit(void) {
	// Initialize a game with setting variable.

	// Timers for animation
	moveTimer = createTimer(MOVE_TICK);
	startTimer(moveTimer);

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
	// Fixed a bug - Icon is going down automatically.
	if (dyIcon > MOVE_SPEED) {
		dyIcon = MOVE_SPEED;
	}
	
	// Set a restriction.
	iconX += dxIcon;
	if (iconX < iconX_MIN) {
		iconX = iconX_MIN;
	}
	else if (iconX > iconX_MAX) {
		iconX = iconX_MAX;
	}

	iconY += dyIcon;
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

// ====================================================================================

void timerCallback(TimerID timer) {
	// Processing an animation.

	// If keyboard input, then player will move.
	if (timer == moveTimer) {
		if (currentScene == towerScene) {
			playerMove();
		}
		else if (currentScene == battleScene) {
			playerIconMove();
		}
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
			dxIcon -= (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
	// 83: Right arrow-key
	else if (code == 83) {
		if (currentScene == towerScene) {
			dx += (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
		else if (currentScene == battleScene) {
			dxIcon += (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
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
			dyIcon = MOVE_SPEED;
			dyIcon += (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
	// 85: Down arrow-key
	else if (code == 85) {
		if (currentScene == battleScene) {
			dyIcon -= (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
}

// ====================================================================================

int main(void) {
	// Set callback functions.
	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);

	// Start a game.
	gameInit();
	startGame(currentScene);
}