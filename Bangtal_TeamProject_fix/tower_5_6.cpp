// Bangtal Library, Team Project
// 20200690 JinCheol Park, Code about Tower (5th, 6th floor)

// ====================================================================================

// Preprocessor
#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

// For debugging, set this variable true.
const bool DEBUGGING = true;

// Enemy's attack
class Attack1 {
public:
	ObjectID obj;
	int x, y, dy;

public:
	Attack1(ObjectID obj, int x, int y, int dy) : obj(obj), x(x), y(y), dy(dy) {};
};

class Attack2 {
public:
	ObjectID left, right;
	ObjectID rect;
	int left_x, right_x, rect_x, y;
	int time;

public:
	Attack2(ObjectID left, ObjectID right, ObjectID rect, int y, int time) : left(left), right(right), rect(rect), y(y), time(time) {
		left_x = 0;
		right_x = 983;
		rect_x = 333;
	};
};

// Animation for character's movement
TimerID moveTimer;
const Second MOVE_TICK = 0.025f;
const int MOVE_SPEED = 10;

// Const variable for turn
const int PLAYER = 0;
const int ENEMY = 1;

// Animation for turn
TimerID turnTimer;
int turn = PLAYER;
int turnCnt = 0;
const int TURN_TIME = 20;
const Second TURN_TICK = 0.05f;

// Animation for immune state
TimerID immuneTimer;
int immuneCnt = 0;
const Second IMMUNE_TICK = 0.05f;
const int IMMUNE_TIME = 20;
bool hitAlready = false;

// Animation for enemy's attack
TimerID enemyAtt1, enemyAtt2, enemyAtt3;
const Second ATTACK_TICK = 0.05f;
int GRAVITY = 2;

// ====================================================================================

// Scenes and Objects
// Scenes
SceneID tower5F_Scene, battle5F_Scene;
SceneID tower6F_Scene, game6F_Scene;
SceneID currentScene;

// Sounds
SoundID hitSound;      // Player was hit.
SoundID attackSound;   // Player attacked.

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
const int playerHpBarX_FIXED = 333, playerHpBarY_FIXED = 58;

int playerHp, playerMaxHp, playerAtk, playerDef;

ObjectID attackButton, itemButton, avoidButton;
const int buttonY_FIXED = 80;

// Enemy
ObjectID enemy;
ObjectID enemyHpBar;
int enemyX, enemyY;
int enemyHp, enemyMaxHp, enemyAtk, enemyDef;
const int enemyY_FIXED = 410;
const int enemyHpBarX_FIXED = 333, enemyHpBarY_FIXED = 660;

// Attack1
int enemyAtt1Cnt = 0, enemyAtt1MaxCnt = 40;
const int enemyAtt1StartCnt = 40;
const int enemyAtt1Y_FIXED = 440;
const int enemyAtt1X_SIZE = 34, enemyAtt1Y_SIZE = 30;

// Attack2
int enemyAtt2Cnt = 0, enemyAtt2MaxCnt = 20;

std::vector<Attack1> attacks1;

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
void enemyAttack(void);
void enemyAttack1(void);
void enemyAttack2(void);
void enemyAttack3(void);

bool checkCollision(ObjectID object, int xStart, int xEnd, int yStart, int yEnd);
void checkHp(int kind);

void showGold(void);

void timerCallback(TimerID timer);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void keyboardCallback(KeyCode code, KeyState state);

// ====================================================================================

// Implementations
void gameInit(void) {
	// Initialize a game with setting variable.
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	// Timers for animation
	moveTimer = createTimer(MOVE_TICK);
	startTimer(moveTimer);

	// Timers for turn, immune, and attacks
	turnTimer = createTimer(TURN_TICK);
	immuneTimer = createTimer(IMMUNE_TICK);
	enemyAtt1 = createTimer(ATTACK_TICK);
	enemyAtt2 = createTimer(ATTACK_TICK);
	enemyAtt3 = createTimer(ATTACK_TICK);

	// Sounds
	hitSound = createSound("./Sounds/HitSound.wav");
	attackSound = createSound("./Sounds/AttackSound.wav");

	// Scenes
	tower5F_Scene = createScene("tower5F_Scene", "./Images/Backgrounds/Tower_Inside.png");
	battle5F_Scene = createScene("battle5F_Scene", "./Images/Backgrounds/Battle.png");
	currentScene = tower5F_Scene;

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
	locateObject(playerIcon, battle5F_Scene, iconX, iconY);
	showObject(playerIcon);

	playerHp = 100;
	playerMaxHp = 100;
	playerAtk = 5;
	playerDef = 3;

	playerHpBar = createObject("./Images/UI/Battle/HP/Hp_100%.png");
	locateObject(playerHpBar, battle5F_Scene, playerHpBarX_FIXED, playerHpBarY_FIXED);
	showObject(playerHpBar);

	attackButton = createObject("./Images/UI/Battle/attack.png");
	locateObject(attackButton, battle5F_Scene, 310, buttonY_FIXED);
	scaleObject(attackButton, 0.65f);
	showObject(attackButton);

	itemButton = createObject("./Images/UI/Battle/item.png");
	locateObject(itemButton, battle5F_Scene, 610, buttonY_FIXED);
	scaleObject(itemButton, 0.65f);
	showObject(itemButton);

	avoidButton = createObject("./Images/UI/Battle/avoid.png");
	locateObject(avoidButton, battle5F_Scene, 890, buttonY_FIXED);
	scaleObject(avoidButton, 0.65f);
	showObject(avoidButton);

	// Enemy
	enemy = createObject("./Images/Enemies/BlackKnight.png");
	enemyX = 590;
	enemyY = enemyY_FIXED;
	enemyHp = 50;
	enemyMaxHp = 50;
	enemyAtk = 10;
	enemyDef = 3;
	locateObject(enemy, battle5F_Scene, enemyX, enemyY);
	scaleObject(enemy, 0.15f);
	showObject(enemy);

	enemyHpBar = createObject("./Images/UI/Battle/Hp/Hp_100%.png");
	locateObject(enemyHpBar, battle5F_Scene, enemyHpBarX_FIXED, enemyHpBarY_FIXED);
	showObject(enemyHpBar);

	// Gold
	goldList[0] = createObject("./Images/Numbers/0_R.png");
	goldList[1] = createObject("./Images/Numbers/1_R.png");
	goldList[2] = createObject("./Images/Numbers/2_R.png");

	for (int i = 0; i < 3; i++) {
		scaleObject(goldList[i], 0.8f);
		locateObject(goldList[i], battle5F_Scene, goldX[i], goldY);
	}

	showGold();
}

// ====================================================================================

void playerMove(void) {
	// Move a player. (tower5F_Scene)

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
	// Move a player-icon. (battle5F_Scene)

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
	if (turn == PLAYER) {
		showObject(attackButton);
		showObject(itemButton);
		showObject(avoidButton);
	}
	else if (turn == ENEMY) {
		enemyAttack();
	}
}

void enemyAttack(void) {
	srand((unsigned)time(NULL));
	
	int strategy = rand() % 1 + 1;
	switch (strategy) {
	case 1:
		enemyAttack1();

		turn = PLAYER;
		turnCnt = TURN_TIME * 5;
		setTimer(turnTimer, TURN_TICK);
		startTimer(turnTimer);
		
		break;
	case 2:
		// enemyAttack2();
		break;
	case 3:
		// dd
		break;
	default:
		break;
	}
}

void enemyAttack1(void) {
	if (0.5f < static_cast<float>(enemyHp) / static_cast<float>(enemyMaxHp)) {
		enemyAtt1MaxCnt = enemyAtt1StartCnt;
	}
	else if (0.25f < static_cast<float>(enemyHp) / static_cast<float>(enemyMaxHp)) {
		enemyAtt1MaxCnt = static_cast<int>(enemyAtt1StartCnt * 1.5f);
	}
	else {
		enemyAtt1MaxCnt = enemyAtt1StartCnt * 2;
	}

	enemyAtt1Cnt = enemyAtt1MaxCnt;

	setTimer(enemyAtt1, ATTACK_TICK);
	startTimer(enemyAtt1);
}

bool checkCollision(ObjectID object, int xStart, int xEnd, int yStart, int yEnd) {
	// Check a collsion, then return a result.
	
	// About Collision:
	// We can find 'collision' with position and size.
	// If enemy attack's X is about 400 ~ 600,
	// then X_MIN <= player's X <= 360 (360 can be replaced with 400 - SIZE) or 600 <= player's X <= X_MAX
	// If enemy attack's Y is about 230 ~ 300,
	// then Y_MIN <= player's Y <= 160 (160 can be replaced with 230 - SIZE) or 300 <= player's Y <= Y_MAX
	    
	// Conclusion:
	// If a <= attack's range <= b, then player will be hit when (a - SIZE) < player's position < b
	
	if (object == playerIcon) {
		bool xCollision = ((xStart - iconX_SIZE) < iconX) and (iconX < xEnd);
		bool yCollision = ((yStart - iconY_SIZE) < iconY) and (iconY < yEnd);

		return xCollision and yCollision;
	}
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
	
	if (0.75f < hpPercent and hpPercent <= 1.0f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_100%.png");
	}
	else if (0.5f < hpPercent and hpPercent <= 0.75f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_75%.png");
	}
	else if (0.25f < hpPercent and hpPercent <= 0.5f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_50%.png");
	}
	else if (0.1f < hpPercent and hpPercent <= 0.25f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_25%.png");
	}
	else if (0.0f < hpPercent and hpPercent <= 0.1f) {
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_10%.png");
	}
	else {
		// HP is 0, so we have to do something at here.
		setObjectImage(hpBar, "./Images/UI/Battle/Hp/Hp_0%.png");
	}
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
		if (currentScene == tower5F_Scene) {
			playerMove();
		}
		else if (currentScene == battle5F_Scene) {
			playerIconMove();
		}
	}
	else if (timer == turnTimer) {
		if (turnCnt > 0) {
			turnCnt -= 1;

			setTimer(turnTimer, TURN_TICK);
			startTimer(turnTimer);
		}
		else {
			battle();
		}
	}
	else if (timer == enemyAtt1) {
		if (enemyAtt1Cnt > 0) {
			srand((unsigned)time(NULL));
			attacks1.push_back(Attack1(createObject("./Images/Enemies/Att_Down.png"), iconX, enemyAtt1Y_FIXED, 0));

			enemyAtt1Cnt -= 1;
		}

		int erased = 0;
		for (int i = 0; i < (enemyAtt1MaxCnt - enemyAtt1Cnt); i++) {
			if (0.5f < static_cast<float>(enemyHp) / static_cast<float>(enemyMaxHp)) {
				attacks1[i].dy -= GRAVITY;
			}
			else if (0.25f < static_cast<float>(enemyHp) / static_cast<float>(enemyMaxHp)) {
				attacks1[i].dy -= static_cast<int>(GRAVITY * 1.5f);
			}
			else {
				attacks1[i].dy -= GRAVITY * 2;
			}
			
			attacks1[i].y += attacks1[i].dy;
			
			if (attacks1[i].y < 100) {
				hideObject(attacks1[i].obj);
				erased += 1;
			}
			else {
				locateObject(attacks1[i].obj, battle5F_Scene, attacks1[i].x, attacks1[i].y);
				showObject(attacks1[i].obj);

				if (checkCollision(playerIcon, attacks1[i].x + 3, attacks1[i].x + 3 + enemyAtt1X_SIZE, attacks1[i].y, attacks1[i].y + enemyAtt1Y_SIZE) and not hitAlready) {
					playerHp -= 25;
					playSound(hitSound);
					checkHp(PLAYER);

					hitAlready = true;
					immuneCnt = IMMUNE_TIME;
					setTimer(immuneTimer, IMMUNE_TICK);
					startTimer(immuneTimer);
				}
			}
		}

		if (enemyAtt1Cnt != 0) {
			setTimer(enemyAtt1, ATTACK_TICK);
			startTimer(enemyAtt1);
		}
		else {
			if (erased != enemyAtt1MaxCnt) {
				setTimer(enemyAtt1, ATTACK_TICK);
				startTimer(enemyAtt1);
			}
			else {
				attacks1.clear();
			}
		}
	}
	else if (timer == immuneTimer) {
		if (immuneCnt > 0) {
			immuneCnt -= 1;

			setTimer(immuneTimer, IMMUNE_TICK);
			startTimer(immuneTimer);
		}
		else {
			hitAlready = false;
		}
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == enemy) {
		enemyAttack1();
	}
	else if (object == attackButton) {
		enemyHp -= 15;
		playSound(attackSound);
		checkHp(ENEMY);

		hideObject(attackButton);
		hideObject(itemButton);
		hideObject(avoidButton);

		turn = ENEMY;
		turnCnt = TURN_TIME;
		setTimer(turnTimer, TURN_TICK);
		startTimer(turnTimer);
	}
	else if (object == itemButton) {
		// do something with item.
	}
	else if (object == avoidButton) {
		enemyHp = enemyMaxHp;
		enemyAtt1MaxCnt = 40;

		checkHp(ENEMY);

		hideObject(attackButton);
		hideObject(itemButton);
		hideObject(avoidButton);
		
		enterScene(tower5F_Scene);
		currentScene = tower5F_Scene;
	}
}

void keyboardCallback(KeyCode code, KeyState state) {
	// Processing a keyboard input.

	// 82: Left arrow-key
	if (code == 82) {
		if (currentScene == tower5F_Scene) {
			dx -= (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
		else if (currentScene == battle5F_Scene) {
			iconDx -= (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
	// 83: Right arrow-key
	else if (code == 83) {
		if (currentScene == tower5F_Scene) {
			dx += (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
		else if (currentScene == battle5F_Scene) {
			iconDx += (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
	// 84: Up arrow-key
	else if (code == 84) {
		// If current scene is tower and player can battle, then change a scene.
		if (currentScene == tower5F_Scene) {
			currentScene = battle5F_Scene;
			enterScene(currentScene);

			battle();
		}
		// Else if current scene is battle, then player move to upper direction.
		else if (currentScene == battle5F_Scene) {
			iconDy = MOVE_SPEED;
			iconDy += (state == KeyState::KEYBOARD_PRESSED ? MOVE_SPEED : -MOVE_SPEED);
		}
	}
	// 85: Down arrow-key
	else if (code == 85) {
		if (currentScene == battle5F_Scene) {
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