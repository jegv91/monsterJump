/*
  Monster Jump
  Final Project for computer graphics class

  Created by Manuel Spitzl - A01298284 & Joel Garcia - A00806027
  Copyright (c) 2014 Tecnológico de Monterrey. All rights reserved.

*/


#include <windows.h>
#include <iostream>
#include <fstream>
#include <audiere.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <cstdio>
#include <SOIL.h>
#include <time.h>
#include <sstream>

using namespace std;
using namespace audiere;

// Buttons, help us to check if the user pressed, selected or clicked them
const int NO_BUTTON = 0;

//Main Menu
const int PLAY_BUTTON = 11;	
const int CONFIG_BUTTON = 10;	
const int HIGH_S_BUTTON = 12; 
const int CREDITS_BUTTON = 13;

//Config
const int C_BACK_BUTTON = 20;	
const int ENGLISH_BUTTON = 21;	
const int SPANISH_BUTTON = 22;	
const int GERMAN_BUTTON = 23;
const int MUSIC_ON_BUTTON = 24;	
const int MUSIC_OFF_BUTTON = 25;	
const int SOUND_ON_BUTTON = 26;	
const int SOUND_OFF_BUTTON = 27;

//High scores
const int H_BACK_BUTTON = 30;

//Pause
const int RESUME_BUTTON = 40;	
const int RESTART_BUTTON = 41;	
const int RESUME_MENU_BUTTON = 42;	
const int INSTRUCTIONS_BUTTON = 43;

//Credits
const int CR_BACK_BUTTON = 50;

// Variables for screen borders
int borderRight = 1366;
int borderLeft = 0;
int borderBottom = 0;
int borderTop = 700;
int borderNear = -200;
int borderFar = 200;

// Variables for platforms  
const float platformSizeUnit = 20;  
int showPlatform[24];
int platformWithCoin[24];
int coinShown[24];
GLint coinType[24];
int difficultyPlatforms = 10;
int numberOfPlatforms[2] = { 0, 0 };
bool haveToPut = false;
float rotateX = 20.0;
float rotateY = 20.0;
float posY[24];
float posX[24];
int _hidden[24];

// Buttons sizes; changed when pressed 
float playBtnPressed = 1.5;
float configBtnPressed = 1.5;
float highscoreBtnPressed = 1.5;
float cBackBtnPressed = 1.5; //Config back
float hBackBtnPressed = 1.5; //High score back
float crBackBtnPressed = 1.5;//Credits back
float resumeBtnPressed = 1.5;
float restartBtnPressed = 1.5;
float menuBtnPressed = 1.5;
float instructionsBtnPressed = 1.5;

//Variables to move the background 
float offset = 0.0;
static float MENU_OFFSET = 4600.0;
bool anim = false;

//Variables helping us to know the currently selected button if there is more than one button shown
int selectedBtn;
int selectedBtnConfig;
int selectedBtnPause;
int selectedBtnGameOver;

// Possible states of the game
static int GAME_STATE; 
const int STATE_MENU = 1;
const int STATE_GAME = 2;
const int STATE_PAUSE = 3;
const int STATE_WON = 4;
const int STATE_LOST = 5;

// Possible languages
static int LANG;
const int LANG_ENG = 0;
const int LANG_ESP = 1;	
const int LANG_DEU = 2;

// Possible music states
static float VOL_MUSIC = 0.5;
const int MUSIC_ON = 10;
const int MUSIC_OFF = 11;
static float VOL_SFX = 1.0;
const int SOUND_ON = 20;
const int SOUND_OFF = 21;

// Possible main menu states
static int MENU_STATE;
const int MS_MENU = 0;
const int MS_CONFIG = 1;
const int MS_HIGH = 2;
const int MS_CREDITS = 3;

// Indicates if the button is pressed
bool playPressed = false;
bool configPressed = false;
bool highscorePressed = false;
bool hBackPressed = false;
bool cBackPressed = false;
bool crBackPressed = false;
bool resumePressed = false;
bool restartPressed = false;
bool creditsPressed = false;
bool menuPressed = false;
bool instructionsPressed = false;

// Manages the transformation of menu icons
float rotateAngleConf = 0.0;
float rotateAngleHigh = 0.0;
float yRedCoin = 140.0;

// Manages the count down at the start of the game
bool counting = false;

// Manages the possible movements of Hopster
bool jumping = false;
bool falling = false;
bool moveLeft = false;
bool moveRight = false;
int jumpDuration = 5;

// Position and size of Hopster
float hopsterX = 0;
float hopsterY = 0;
float hopsterWidth = 0;
float hopsterHeight = 0;
int onPlatform = -1;

// Manages the animations 
int animIntroCurrent;
int animJumpCurrent;
int currentCountDownNumber;


// Textures 
GLuint interTextures[3][20];
GLuint animationIntro[48];
GLuint animationJump[48];
GLuint countDown[3];
GLuint tex, platformSide, platformTop, platformBottom, background,
	platformVenomSide, platformVenomTop, platformVenomBottom,
	buttonSide, buttonPlay, buttonJugar, buttonSpielen, buttonSettings,
	buttonConfig, buttonEinstellungen, buttonHighscore,
	configIcon, highscoreIcon, buttonBack, buttonSalir, buttonZurueck,
	musicOnIcon, musicOffIcon, soundOnIcon, soundOffIcon, englishSelected,
	englishUnselected, espanolSelected, espanolUnselected, deutschSelected,
	deutschUnselected, offSelected, offUnselected, onSelected, onUnselected,
	ausSelected, ausUnselected, anSelected, anUnselected, apagarSelected,
	apagarUnselected, encenderSelected, encenderUnselected,
	buttonResume, buttonSeguir, buttonFortsetzen, buttonRestart, buttonReiniciar,
	buttonNeustart, buttonMenuEng, buttonMenuEsp, buttonMenuDeu, buttonInstEng,
	buttonInstEsp, buttonAnleitung, coinRed, coinGold, coinBlue, magnet, star,
	monsterJumpLogo, credits[3], gameover[3];

// Loading the music streams
AudioDevicePtr device(OpenDevice());
	OutputStreamPtr gameMusic = OutputStreamPtr(OpenSound(device, "../data/music/MJMainMusic_Game.wav", false));
AudioDevicePtr device1(OpenDevice());
	OutputStreamPtr menuMusic = OutputStreamPtr(OpenSound(device1, "../data/music/MJMainMusic_Menu.wav", false));
AudioDevicePtr device2(OpenDevice());
	OutputStreamPtr gameOverMusic = OutputStreamPtr(OpenSound(device2, "../data/music/MJMainMusic_GameOver.wav", false));
AudioDevicePtr deviceX3(OpenDevice());
	OutputStreamPtr pauseMusic = OutputStreamPtr(OpenSound(deviceX3, "../data/music/MJMainMusic_Pause.wav", false));


// Loading the sound effect streams 
AudioDevicePtr device3(OpenDevice());
	OutputStreamPtr sfxJump = OutputStreamPtr(OpenSound(device3, "../data/music/MJSoundFX_Jump.wav", false));
AudioDevicePtr device4(OpenDevice());
	OutputStreamPtr sfxFallDown = OutputStreamPtr(OpenSound(device4, "../data/music/MJSoundFX_FallDown.wav", false));
AudioDevicePtr device5(OpenDevice());
	OutputStreamPtr sfxMagnet = OutputStreamPtr(OpenSound(device5, "../data/music/MJSoundFX_PowerUpMagnet.wav", false));
AudioDevicePtr device6(OpenDevice());
	OutputStreamPtr sfxStar = OutputStreamPtr(OpenSound(device6, "../data/music/MJSoundFX_PowerUpStar.wav", false));
AudioDevicePtr device7(OpenDevice());
	OutputStreamPtr sfxCoin = OutputStreamPtr(OpenSound(device7, "../data/music/MJSoundFX_CoinCollected.wav", false));
AudioDevicePtr device8(OpenDevice());
	OutputStreamPtr sfxBigCoin = OutputStreamPtr(OpenSound(device8, "../data/music/MJSoundFX_BigCoinCollected.wav", false));
AudioDevicePtr device9(OpenDevice());
	OutputStreamPtr sfxButton = OutputStreamPtr(OpenSound(device9, "../data/music/MJSoundFX_ButtonPressed.wav", false));

void updateVolume()
{
	gameMusic->setVolume(VOL_MUSIC);
	menuMusic->setVolume(VOL_MUSIC);
	gameOverMusic->setVolume(VOL_MUSIC);
	pauseMusic->setVolume(VOL_MUSIC);
	sfxJump->setVolume(VOL_SFX);
	sfxFallDown->setVolume(VOL_SFX);
	sfxMagnet->setVolume(VOL_SFX);
	sfxStar->setVolume(VOL_SFX);
	sfxCoin->setVolume(VOL_SFX);
	sfxBigCoin->setVolume(VOL_SFX);
	sfxButton->setVolume(VOL_SFX);
}

GLint LoadGLTexture(const char *filename, int width, int height)
{
	GLuint texture;

	//CODE SEGMENT 2
	texture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture
		(
		filename,
		SOIL_LOAD_RGBA,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
		);


	// check for an error during the load process
	if (texture == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	glGenTextures(1, &tex); // allocate a texture name
	glBindTexture(GL_TEXTURE_2D, texture); // select our current texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);  // when texture area is small, bilinear filter the closest mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when texture area is large, bilinear filter the first mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // texture should tile
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//j gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	return texture;
}

//Initializes variables and music for menu state
void initMenu(void)
{
	GAME_STATE = STATE_MENU;

    //Change from game to menu music
	gameMusic->stop();
	menuMusic->stop();
	gameOverMusic->stop();
	pauseMusic->stop();
	menuMusic->setPosition(0);
	menuMusic->play();
	menuMusic->setRepeat(true);

	animIntroCurrent = 0;
	offset = MENU_OFFSET;
	MENU_STATE = MS_MENU;
	currentCountDownNumber = 3;
	anim = false;
	playPressed = false;
	configPressed = false;
	highscorePressed = false;
	hBackPressed = false;
	cBackPressed = false;

	selectedBtn = PLAY_BUTTON;

	yRedCoin = 140.0;
	rotateAngleConf = 0.0;
	rotateAngleHigh = 0.0;
}

//Moves the platforms & background
void gameTimer(int v)
{
    //Update the positions of the platforms
	for (int i = 0; i < 24; i++)
	{
		posY[i] -= 5;
		if (posY[i]<(-65 + borderBottom - platformSizeUnit * 2))
		{
			posY[i] = borderTop + platformSizeUnit * 2;
			//changeHiddenInRow(i);
		}
	}

    //Update the offset of the background
	offset += 0.5;

	glutPostRedisplay();
	if (GAME_STATE == STATE_GAME)
		glutTimerFunc(30, gameTimer, 1);

}

//Initializes variables and music for game state
void initGame()
{
    //change from pause/menu to game music
	gameMusic->stop();
	gameOverMusic->stop();
	pauseMusic->stop();
	gameMusic->setPosition(0);
	gameMusic->play();
	gameMusic->setRepeat(true);

	animJumpCurrent = 0;

	int _posY = 0;
	int _posX = 160;

    //Get initial positions for the platforms
	for (int i = 0; i < 4; i++)
	{
		posX[i * 6] = _posX;
		posY[i * 6] = _posY;

		posX[i * 6 + 1] = _posX + 200;
		posY[i * 6 + 1] = _posY;

		posX[i * 6 + 2] = _posX + 400;
		posY[i * 6 + 2] = _posY;

		posX[i * 6 + 3] = _posX + 600;
		posY[i * 6 + 3] = _posY;

		posX[i * 6 + 4] = _posX + 800;
		posY[i * 6 + 4] = _posY;

		posX[i * 6 + 5] = _posX + 1000;
		posY[i * 6 + 5] = _posY;

		_posY += borderTop / 3 - platformSizeUnit / 2;
	}

    glClearColor(0.0, 0.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);//sombreado plano
	//glShadeModel (GL_FLAT );

	offset = 0.0;
	anim = false;
	jumping = false;
	falling = false;
	moveLeft = false;
	moveRight = false;
	jumpDuration = 5;

    //Initialize hidden as false for all platforms
	for (int i = 0; i<24; i++) {
		if(i<18) _hidden[i] = 1;
		else _hidden[i] = 0;
		coinShown[i] = 0;
	}
	
	// Initialize hopster size & position
	float percentSize = 0.35;
	hopsterWidth = 640 * percentSize;
	hopsterHeight = 950 * percentSize;
	hopsterX = posX[19] - (hopsterWidth/2);
	hopsterY = posY[19];
	onPlatform = 19;

	glutTimerFunc(5, gameTimer, 1);
	glutPostRedisplay();
}


//Initializes variables for game over state
void initGameOverMenu()
{
	selectedBtnPause = RESTART_BUTTON;
	restartBtnPressed = 1.5;
	menuBtnPressed = 1.5;
	restartPressed = false;
	menuPressed = false;
}


//Initializes variables for pause state
void initPauseMenu()
{
	selectedBtnPause = RESUME_BUTTON;
	resumeBtnPressed = 1.5;
	restartBtnPressed = 1.5;
	menuBtnPressed = 1.5;
	instructionsBtnPressed = 1.5;
	resumePressed = false;
	restartPressed = false;
	menuPressed = false;
	instructionsPressed = false;
}


//Is called when the game is started, executes the start animation & the countdown
void startTimer(int v)
{
	if (GAME_STATE == STATE_MENU && animIntroCurrent < 47)
	{
		animIntroCurrent++;
		glutPostRedisplay();
		glutTimerFunc(30, startTimer, 1);
	}
	else if (offset > 0)
	{
		if (offset >= 20) offset -= 20;
		else offset -= 2;
		glutPostRedisplay();
		glutTimerFunc(5, startTimer, 1);
	}
	else if (v == 1 && currentCountDownNumber > 0)
	{
		sfxBigCoin->setPosition(0);
		sfxBigCoin->play();
		counting = true;
		currentCountDownNumber--;
		glutPostRedisplay();
		glutTimerFunc(500, startTimer, 2);
	}
	else if (v == 2 && currentCountDownNumber >= 0)
	{
		counting = false;
		glutPostRedisplay();
		glutTimerFunc(500, startTimer, 1);
	}

	//update the variables for state game
	else
	{
		GAME_STATE = STATE_GAME;
		counting = false;
		currentCountDownNumber = 3;
		initGame();
	}

}


//Called when configuration button in menu is pressed, animates to config view
void configTimer(int v)
{
	if (rotateAngleConf < 360)
	{
		rotateAngleConf += 5;
		glutPostRedisplay();
		glutTimerFunc(20, configTimer, 1);
	}
	else if (offset > 3800)
	{
		offset -= 5;
		glutPostRedisplay();
		glutTimerFunc(10, configTimer, 1);
	}
	else
	{
		anim = false;
		rotateAngleConf = 0;
		MENU_STATE = MS_CONFIG;
		glutPostRedisplay();
	}
}

void collisionPowerUp()
{
    float hopsterOffset = hopsterX + (hopsterWidth / 2);
    float hopsterTop = hopsterY + (2*hopsterHeight/3);
    float leftSideOfPowerUp;
    float rightSideOfPowerUp;
    float tolerance = hopsterWidth/6;

    for(int i = 0; i < 24; i ++)
    {
        if(_hidden[i]==0 && coinShown[i] == 1)

        {
            leftSideOfPowerUp = posX[i]-25;
            rightSideOfPowerUp = posX[i]+25;
            //offset is in range of left and right size + tolerance
            if(leftSideOfPowerUp - tolerance <= hopsterOffset &&
                rightSideOfPowerUp + tolerance >= hopsterOffset &&
                hopsterTop >= posY[i] + 4*platformSizeUnit && hopsterY - 10 <= posY[i] + 4*platformSizeUnit)
            {
				sfxCoin->setPosition(0);
				sfxCoin->play();
				platformWithCoin[i]=0;
				glutPostRedisplay();
            }

        }

    }
}

int collisionPlatform()
{
	float hopsterOffset = hopsterX + (hopsterWidth / 2);
	float leftSideOfPlatform;
	float rightSideOfPlatform;
	float tolerance = 25.0;

	for (int i = 0; i < 24; i++)
	{
		if(_hidden[i] == 0 && posY[i] < borderTop - platformSizeUnit)
        {
            leftSideOfPlatform = posX[i] - 4 * platformSizeUnit;
            rightSideOfPlatform = leftSideOfPlatform +  8 * platformSizeUnit;

            if ((hopsterOffset+tolerance >= leftSideOfPlatform) && (hopsterOffset-tolerance <= rightSideOfPlatform)

                && (hopsterY >= posY[i]) && (hopsterY <= (posY[i] + 25)))
            {
                return i;
            }
        }
/*else{

			printf("NO Collision at x: %f  y: %f  Hopster x: %f y: %f  plat: %f \n", posX[i], posY[i], hopsterOffset, hopsterY, rightSideOfPlatform);
		}*/


	}


	return -1;
}


//Updates the x, y position of Hopster
void moveHopsterInY() {
    collisionPowerUp();

    //Movement depending on current state
    if(animJumpCurrent <= 23) {
        hopsterY += 28;
    } else {
        onPlatform = collisionPlatform();
        if(onPlatform == -1 || hopsterY > posY[onPlatform]+28)
            hopsterY -=28;
        else {
			hopsterY = posY[onPlatform];
		}
    }
}

//Updates the horizontal position of Hopster
void moveHopsterInX() {
	collisionPowerUp();

	if(moveLeft) {
		if (hopsterX + hopsterWidth>20) hopsterX -= 20;
        else hopsterX = borderRight - hopsterWidth;
    } else if(moveRight) {
		if (hopsterX - hopsterWidth<borderRight - 20) hopsterX += 20;
        else hopsterX = 0;
    }

	if(!jumping) 	{
		onPlatform = collisionPlatform();
		cout << "Collision platform: " << onPlatform << endl;

		if(onPlatform == -1 || hopsterY > posY[onPlatform]+28) {
            hopsterY -=28;
			falling = true;
		} else {
			hopsterY = posY[onPlatform];
			falling = false;
		}
	}
}

void walkTimer(int v) {
	if(GAME_STATE == STATE_GAME && (moveLeft || moveRight || falling)) {
		moveHopsterInX();
		glutPostRedisplay();
		if(falling) glutTimerFunc(15, walkTimer, 1);
		else glutTimerFunc(50, walkTimer, 1);
	}
}

void jumpTimer(int v) {
	moveHopsterInX();

	if(GAME_STATE == STATE_PAUSE) 
	{
		glutTimerFunc(120, jumpTimer, 1);
		return;
	}

	if(animJumpCurrent == 16) {
		sfxJump->setPosition(0);
		sfxJump->play();
	}

    //Changes the current image of the animation if it has not yet finished
	if (animJumpCurrent < 47) {
	    //Wait for hopster starting to jump until movement in x & y starts
        if (animJumpCurrent > 15) {
            moveHopsterInY();
        }

		if ((animJumpCurrent != 23 || jumpDuration == 0) && animJumpCurrent < 47) animJumpCurrent++;
		else if (jumpDuration > 0) jumpDuration--;

		glutPostRedisplay();

		//Only keep executing the timer if game not lost
		if(GAME_STATE == STATE_GAME && animJumpCurrent < 15) glutTimerFunc(10, jumpTimer, 1);

		else if(GAME_STATE == STATE_GAME && onPlatform==-1) glutTimerFunc(30, jumpTimer, 1);
		else if(GAME_STATE == STATE_GAME) glutTimerFunc(10, jumpTimer, 1);
	}
	//if animation has finished, but hopster is still falling
	else if(onPlatform == -1) {
        moveHopsterInY();
		glutPostRedisplay();
        if(GAME_STATE == STATE_GAME) glutTimerFunc(15, jumpTimer, 1);
    }

	//animation has finished and hopster reached a platform, reset variables, stop calling glutTimerFunc(30, jumpTi
	else {
	    hopsterY = posY[onPlatform];
		jumping = false;
		jumpDuration = 5;
		glutPostRedisplay();
		animJumpCurrent = 0;
	}
}

void highscoreTimer(int v)
{
	if (rotateAngleHigh < 360)
	{
		rotateAngleHigh += 5;
		(rotateAngleHigh < 180) ? yRedCoin += 1 : yRedCoin -= 1;
		glutPostRedisplay();
		glutTimerFunc(20, highscoreTimer, 1);
	}
	else if (offset < 5400)
	{
		offset += 5;
		glutPostRedisplay();
		glutTimerFunc(10, highscoreTimer, 1);
	}
	else
	{
		rotateAngleHigh = 0.0;
		yRedCoin = 140.0;
		anim = false;
		MENU_STATE = MS_HIGH;
	}

}

/*Credits timer*/
void creditsTimer(int time){

	if (offset <= 7500-borderTop) {
		offset += 15;
		glutPostRedisplay();
		glutTimerFunc(10, creditsTimer, 1);
	} else {
		anim = false;
		MENU_STATE = MS_CREDITS;
	}
}

void backToMainTimer(int v)
{
	if (offset <= 4600 - 5)
	{
		offset += 5;
		glutPostRedisplay();
		glutTimerFunc(15, backToMainTimer, 1);
	}
	else if (offset >= 4600 + 5)
	{
		offset -= 5;
		glutPostRedisplay();
		glutTimerFunc(15, backToMainTimer, 1);
	}
	else
	{
		anim = false;
		MENU_STATE = MS_MENU;
		glutPostRedisplay();
	}
}

void drawPlatform(GLuint top, GLuint bottom, GLuint side, GLuint frontTxt)
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, frontTxt);
	glBegin(GL_QUADS);
	//Frente
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-4 * platformSizeUnit, -platformSizeUnit, 4 * platformSizeUnit);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(4 * platformSizeUnit, -platformSizeUnit, 4 * platformSizeUnit);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(4 * platformSizeUnit, platformSizeUnit, 4 * platformSizeUnit);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-4 * platformSizeUnit, platformSizeUnit, 4 * platformSizeUnit);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	//Arriba
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-4 * platformSizeUnit, platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-4 * platformSizeUnit, platformSizeUnit, 4 * platformSizeUnit);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(4 * platformSizeUnit, platformSizeUnit, 4 * platformSizeUnit);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(4 * platformSizeUnit, platformSizeUnit, -4 * platformSizeUnit);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, bottom);
	glBegin(GL_QUADS);
	//Abajo
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-4 * platformSizeUnit, -platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(4 * platformSizeUnit, -platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(4 * platformSizeUnit, -platformSizeUnit, 4 * platformSizeUnit);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-4 * platformSizeUnit, -platformSizeUnit, 4 * platformSizeUnit);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, side);
	glBegin(GL_QUADS);
	//Izquierda
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-4 * platformSizeUnit, platformSizeUnit, 4 * platformSizeUnit);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(-4 * platformSizeUnit, platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(-4 * platformSizeUnit, -platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-4 * platformSizeUnit, -platformSizeUnit, 4 * platformSizeUnit);

	//Derecha
	glTexCoord2d(0.0, 0.0);
	glVertex3f(4 * platformSizeUnit, -platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(4 * platformSizeUnit, platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(4 * platformSizeUnit, platformSizeUnit, 4 * platformSizeUnit);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(4 * platformSizeUnit, -platformSizeUnit, 4 * platformSizeUnit);

	//Atrás
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-4 * platformSizeUnit, -platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-4 * platformSizeUnit, platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(4 * platformSizeUnit, platformSizeUnit, -4 * platformSizeUnit);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(4 * platformSizeUnit, -platformSizeUnit, -4 * platformSizeUnit);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawBackground()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background);

	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(0.0, 0 - offset, borderNear + 1);
	glTexCoord2d(1, 0.0);
	glVertex3d(1366, 0 - offset, borderNear + 1);
	glTexCoord2d(1, 1);
	glVertex3d(1366, 8000 - offset, borderNear + 1);
	glTexCoord2d(0.0, 1);
	glVertex3d(0.0, 8000 - offset, borderNear + 1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawIcon(GLuint icon, int width, int height, float texMapLeft, float texMapRight, int angle)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, icon);

	//Rotate the texture
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(0.5, 0.5, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glTranslatef(-0.5, -0.5, 0.0);
	glMatrixMode(GL_MODELVIEW);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2d(texMapLeft, texMapLeft);
	glVertex3d(0, 0, borderNear + 2);
	glTexCoord2d(texMapRight, texMapLeft);
	glVertex3d(width, 0, borderNear + 2);
	glTexCoord2d(texMapRight, texMapRight);
	glVertex3d(width, height, borderNear + 2);
	glTexCoord2d(texMapLeft, texMapRight);
	glVertex3d(0, height, borderNear + 2);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

int BTN_PLAY = 0;
int BTN_SETTINGS = 1;
int BTN_BACK = 2;
int BTN_HIGH = 3;
int ENG_SEL = 4;
int ESP_SEL = 5;
int DEU_SEL = 6;
int OFF_SEL = 7;
int OFF_UNS = 8;
int ON_SEL = 9;
int ON_UNS = 10;
int BTN_RESUME = 11;
int BTN_RESTART = 12;
int BTN_MENU = 13;
int BTN_INSTR = 14;

void initInterTextures()
{
	//english
	interTextures[LANG_ENG][BTN_PLAY] = buttonPlay;
	interTextures[LANG_ENG][BTN_SETTINGS] = buttonSettings;
	interTextures[LANG_ENG][BTN_BACK] = buttonBack;
	interTextures[LANG_ENG][BTN_HIGH] = buttonHighscore;
	interTextures[LANG_ENG][ENG_SEL] = englishSelected;
	interTextures[LANG_ENG][ESP_SEL] = espanolUnselected;
	interTextures[LANG_ENG][DEU_SEL] = deutschUnselected;
	interTextures[LANG_ENG][OFF_SEL] = offSelected;
	interTextures[LANG_ENG][OFF_UNS] = offUnselected;
	interTextures[LANG_ENG][ON_SEL] = onSelected;
	interTextures[LANG_ENG][ON_UNS] = onUnselected;
	interTextures[LANG_ENG][BTN_RESUME] = buttonResume;
	interTextures[LANG_ENG][BTN_RESTART] = buttonRestart;
	interTextures[LANG_ENG][BTN_MENU] = buttonMenuEng;
	interTextures[LANG_ENG][BTN_INSTR] = buttonInstEng;

	//espanol
	interTextures[LANG_ESP][BTN_PLAY] = buttonJugar;
	interTextures[LANG_ESP][BTN_SETTINGS] = buttonConfig;
	interTextures[LANG_ESP][BTN_BACK] = buttonSalir;
	interTextures[LANG_ESP][BTN_HIGH] = buttonHighscore;
	interTextures[LANG_ESP][ENG_SEL] = englishUnselected;
	interTextures[LANG_ESP][ESP_SEL] = espanolSelected;
	interTextures[LANG_ESP][DEU_SEL] = deutschUnselected;
	interTextures[LANG_ESP][OFF_SEL] = apagarSelected;
	interTextures[LANG_ESP][OFF_UNS] = apagarUnselected;
	interTextures[LANG_ESP][ON_SEL] = encenderSelected;
	interTextures[LANG_ESP][ON_UNS] = encenderUnselected;
	interTextures[LANG_ESP][BTN_RESUME] = buttonSeguir;
	interTextures[LANG_ESP][BTN_RESTART] = buttonReiniciar;
	interTextures[LANG_ESP][BTN_MENU] = buttonMenuEsp;
	interTextures[LANG_ESP][BTN_INSTR] = buttonInstEsp;

	//german
	interTextures[LANG_DEU][BTN_PLAY] = buttonSpielen;
	interTextures[LANG_DEU][BTN_SETTINGS] = buttonEinstellungen;
	interTextures[LANG_DEU][BTN_BACK] = buttonZurueck;
	interTextures[LANG_DEU][BTN_HIGH] = buttonHighscore;
	interTextures[LANG_DEU][ENG_SEL] = englishUnselected;
	interTextures[LANG_DEU][ESP_SEL] = espanolUnselected;
	interTextures[LANG_DEU][DEU_SEL] = deutschSelected;
	interTextures[LANG_DEU][OFF_SEL] = ausSelected;
	interTextures[LANG_DEU][OFF_UNS] = ausUnselected;
	interTextures[LANG_DEU][ON_SEL] = anSelected;
	interTextures[LANG_DEU][ON_UNS] = anUnselected;
	interTextures[LANG_DEU][BTN_RESUME] = buttonFortsetzen;
	interTextures[LANG_DEU][BTN_RESTART] = buttonNeustart;
	interTextures[LANG_DEU][BTN_MENU] = buttonMenuDeu;
	interTextures[LANG_DEU][BTN_INSTR] = buttonAnleitung;
}





void drawMenu()
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	drawBackground();

	glPushMatrix();
	glTranslatef(0, (MENU_OFFSET - offset), 0);

	//Monster Jump logo
	glPushMatrix();
	glTranslatef(borderRight / 2 - 179, borderTop - 150, 0);
	drawIcon(monsterJumpLogo, 400, 160, 0, 1, 0);
	glPopMatrix();

	//Draw hopster play pressed animation
	glPushMatrix();
	glTranslatef((borderRight / 2) - 350 * 0.6, (borderTop / 2) - 170, 0);
	drawIcon(animationIntro[animIntroCurrent], 700 * 0.6, 880 * 0.6, 0, 1, 0);
	glPopMatrix();

	//Draw configurations icon
	glPushMatrix();
	glTranslatef(100, 150, 0);
	drawIcon(configIcon, 200, 200, 0.25, 0.75, rotateAngleConf);
	glPopMatrix();

	//Draw highscore icon
	glPushMatrix();
	glTranslatef(borderRight - 300, 150, 0);
	drawIcon(highscoreIcon, 200, 200, 0, 1, 0);
	glPushMatrix();
	glTranslatef(17, 85, 100);
	drawIcon(coinBlue, 30, 30, 0.25, 0.75, rotateAngleHigh);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(157, 85, 100);
	drawIcon(coinGold, 30, 30, 0.25, 0.75, rotateAngleHigh);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(76, yRedCoin, 100);
	drawIcon(coinRed, 50, 50, 0.25, 0.75, rotateAngleHigh);
	glPopMatrix();
	glPopMatrix();

	GLuint musIcon, musOn, musOff, sndIcon, sndOn, sndOff;
	if (VOL_MUSIC == 0.0)
	{
		musIcon = musicOffIcon;
		musOn = interTextures[LANG][ON_UNS];
		musOff = interTextures[LANG][OFF_SEL];
	}
	else
	{
		musIcon = musicOnIcon;
		musOn = interTextures[LANG][ON_SEL];
		musOff = interTextures[LANG][OFF_UNS];
	}

	if (VOL_SFX == 0.0)
	{
		sndIcon = soundOffIcon;
		sndOn = interTextures[LANG][ON_UNS];
		sndOff = interTextures[LANG][OFF_SEL];
	}
	else
	{
		sndIcon = soundOnIcon;
		sndOn = interTextures[LANG][ON_SEL];
		sndOff = interTextures[LANG][OFF_UNS];
	}

	
	//Draw music & sound icons in configuration view
	glPushMatrix();
	glTranslatef((borderRight / 3), -550, 0);

	//Draw "on" and "off" icons for music
	glPushMatrix();

	//On
	if (selectedBtnConfig == MUSIC_ON_BUTTON) glColor4f(1, 1, 1, 1);
	else glColor4f(0.0, 0.0, 0.75, 1);
	glTranslatef(250, 50, 0);
	drawIcon(musOn, 200, 50, 0, 1, 0);

	//Off
	if (selectedBtnConfig == MUSIC_OFF_BUTTON) glColor4f(1, 1, 1, 1);
	else glColor4f(0.0, 0.0, 0.75, 1);
	glTranslatef(250, 0, 0);
	drawIcon(musOff, 200, 50, 0, 1, 0);
	glPopMatrix();

	glColor4f(1, 1, 1, 1);
	drawIcon(musIcon, 150, 150, 0, 1, 0);
	glTranslatef(0, -200, 0);
	drawIcon(sndIcon, 150, 150, 0, 1, 0);

	//Draw "on" and "off" icons for sound
	glPushMatrix();

	//On
	if (selectedBtnConfig == SOUND_ON_BUTTON) glColor4f(1, 1, 1, 1);
	else glColor4f(0.0, 0.0, 0.75, 1);
	glTranslatef(250, 50, 0);
	drawIcon(sndOn, 200, 50, 0, 1, 0);

	//Off
	if (selectedBtnConfig == SOUND_OFF_BUTTON) glColor4f(1, 1, 1, 1);
	else glColor4f(0.0, 0.0, 0.75, 1);
	glTranslatef(250, 0, 0);
	drawIcon(sndOff, 200, 50, 0, 1, 0);
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);


	//Draw configurations icon in configurations view
	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	glTranslatef(20, -800, 0);
	drawIcon(configIcon, 400, 400, 0.25, 0.75, 0);
	glPopMatrix();

	//Highscore view
	glColor4f(1, 1, 1, 1);
	glPushMatrix();

		// Moves the icons of the highscore view +20 in x and +800 in y to be drawn above the main menu
		glTranslatef(20, 800, 0);
		drawIcon(highscoreIcon, 400, 400, 0, 1, 0);
		glPushMatrix();
			glTranslatef(24, 170, 100);
			drawIcon(coinBlue, 60, 60, 0.25, 0.75, rotateAngleHigh);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(314, 170, 100);
			drawIcon(coinGold, 60, 60, 0.25, 0.75, rotateAngleHigh);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(152, 280, 100);
			drawIcon(coinRed, 100, 100, 0.25, 0.75, rotateAngleHigh);
		glPopMatrix();
	glPopMatrix();

	//Credits view
	GLint currentCredits = credits[LANG];
	glColor4f(1, 1, 1, 1);
	glPushMatrix();
		// Move the icon 
		glTranslatef(90, 6825 - MENU_OFFSET, 0);
		drawIcon(currentCredits, 1024, 690, 0, 1, 0);
	glPopMatrix();

	//Idiomas en pantalla config
	glPushMatrix();
		//Ingles
		(selectedBtnConfig == ENGLISH_BUTTON)? glColor4f(1, 1, 1, 1):glColor4f(0.0, 0.0, 0.75, 1);
		glTranslatef((borderRight / 3), -350, 0);
		drawIcon(interTextures[LANG][ENG_SEL], 200, 50, 0, 1, 0);

		//Espanol

		(selectedBtnConfig == SPANISH_BUTTON)? glColor4f(1, 1, 1, 1):glColor4f(0.0, 0.0, 0.75, 1);

		glTranslatef(250, 0, 0);
		drawIcon(interTextures[LANG][ESP_SEL], 200, 50, 0, 1, 0);

		//Aleman
		(selectedBtnConfig == GERMAN_BUTTON)? glColor4f(1, 1, 1, 1):glColor4f(0.0, 0.0, 0.75, 1);

		glTranslatef(250, 0, 0);
		drawIcon(interTextures[LANG][DEU_SEL], 200, 50, 0, 1, 0);
	glPopMatrix();

	//Boton de jugar
	(selectedBtn == PLAY_BUTTON)? glColor4f(1, 1, 1, 1):glColor4f(0.5, 0.5, 0.5, 1);

	glPushMatrix();
		glTranslatef(((borderRight / 2) - (platformSizeUnit / 2)), 160, 0);

		glScalef(playBtnPressed, playBtnPressed, 0);
		glRotatef(2, 1, 0, 0);


		drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_PLAY]);
	glPopMatrix();

	//Boton de configuración
	(selectedBtn == CONFIG_BUTTON)?glColor4f(1, 1, 1, 1): glColor4f(0.5, 0.5, 0.5, 1);

	glPushMatrix();
		glTranslatef(200, 100, 0);

		glScalef(configBtnPressed, configBtnPressed, 0);
		glRotatef(-4, 0, 1, 0);
		glRotatef(2, 1, 0, 0);

		drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_SETTINGS]);
	glPopMatrix();

	//Boton de highscore
	(selectedBtn == HIGH_S_BUTTON)? glColor4f(1, 1, 1, 1):glColor4f(0.5, 0.5, 0.5, 1);

	glPushMatrix();
		glTranslatef(borderRight - 200, 100, 0); 

		glScalef(highscoreBtnPressed, highscoreBtnPressed, 0);
		glRotatef(4, 0, 1, 0);
		glRotatef(2, 1, 0, 0);


		drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_HIGH]);
	glPopMatrix();

	glColor4f(1, 1, 1, 1);

	//Boton de regresar en pantalla highscore
	glPushMatrix();
		glTranslatef(150, 1450, 0);

		glScalef(hBackBtnPressed, hBackBtnPressed, 0);
		glRotatef(-5, 0, 1, 0);
		glRotatef(-3, 1, 0, 0);


		drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_BACK]);
	glPopMatrix();

	//Boton de regresar en pantalla config
	(selectedBtnConfig == C_BACK_BUTTON)?glColor4f(1, 1, 1, 1):glColor4f(0.5, 0.5, 0.5, 1);

	glPushMatrix();
		glTranslatef(150, -150, 0);
		glScalef(cBackBtnPressed, cBackBtnPressed, 0);
		glRotatef(-5, 0, 1, 0);
		glRotatef(-3, 1, 0, 0);
		drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_BACK]);
	glPopMatrix();

	//Boton de regresar en pantalla credits
	//(selectedBtnConfig == CR_BACK_BUTTON)?glColor4f(1, 1, 1, 1):glColor4f(0.5, 0.5, 0.5, 1);
	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	glTranslatef(150, 7450 - MENU_OFFSET, 0);
		glScalef(crBackBtnPressed, crBackBtnPressed, 0);

		glRotatef(-5, 0, 1, 0);
		glRotatef(-3, 1, 0, 0);


		drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_BACK]);
	glPopMatrix();

	glPopMatrix();
}

void drawGameOverMenu()
{
	glColor4f(0.0, 0.0, 0.0, 0.5);
	glBegin(GL_QUADS);
	glVertex3d(0.0, 0 - offset, borderNear + 1);
	glVertex3d(1366, 0 - offset, borderNear + 1);
	glVertex3d(1366, 8000 - offset, borderNear + 1);
	glVertex3d(0.0, 8000 - offset, borderNear + 1);
	glEnd();

	glPushMatrix();

		// Draw the game over view 
		glColor4f(1, 1, 1, 1);
		glPushMatrix();
			glTranslatef(164, 0, 0);
			drawIcon(gameover[LANG], 1038.1, borderTop, 0, 1, 0);
		glPopMatrix();

		float posX = 170;
		glTranslatef(0, 0, 180);

		//Boton de restart
		glPushMatrix();
			if (selectedBtnGameOver == RESTART_BUTTON) glColor4f(1, 1, 1, 1);
			else glColor4f(0.5, 0.5, 0.5, 1);
			posX = (borderRight / 4) + 170;
			glTranslatef(posX, (borderTop / 2 - (platformSizeUnit / 2))-200, 0);

			glScalef(restartBtnPressed, restartBtnPressed, 0);
			glRotatef(-1.5, 0.0, 1.0, 0.0);
			//////////////////////////////////

			drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_RESTART]);
		glPopMatrix();


		//Boton de menu
		glPushMatrix();
			if (selectedBtnGameOver == RESUME_MENU_BUTTON) glColor4f(1, 1, 1, 1);
			else glColor4f(0.5, 0.5, 0.5, 1);
			posX = (2 * borderRight / 4) + 170;
			glTranslatef(posX, (borderTop / 2 - (platformSizeUnit / 2))-200, 0);

			glScalef(menuBtnPressed, menuBtnPressed, 0);
			glRotatef(1.5, 0.0, 1.0, 0.0);
			//////////////////////////////////

			drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_MENU]);
		glPopMatrix();

	glPopMatrix();

}

void drawPauseMenu()
{
	glColor4f(0.0, 0.0, 0.0, 0.5);
	glBegin(GL_QUADS);
	glVertex3d(0.0, 0 - offset, borderNear + 1);
	glVertex3d(1366, 0 - offset, borderNear + 1);
	glVertex3d(1366, 8000 - offset, borderNear + 1);
	glVertex3d(0.0, 8000 - offset, borderNear + 1);
	glEnd();

	glPushMatrix();
		glTranslatef(0, 0, 180);

		//Boton de resume
		glPushMatrix();
			glColor4f(1, 1, 1, 1);
			glTranslatef(500, (borderTop / 2 - (platformSizeUnit / 2)), 0);
			glScalef(5, 5, 0);
			//////////////////////////////////

			drawPlatform(-1, -1, -1, monsterJumpLogo);
			//drawIcon(monsterJumpLogo, 1058, 447,0, 1, 0);
		glPopMatrix();
			

		//Boton de resume
		glPushMatrix();
			if (selectedBtnPause == RESUME_BUTTON) glColor4f(1, 1, 1, 1);
			else glColor4f(0.5, 0.5, 0.5, 1);
			float posX = 170;
			glTranslatef(posX, (borderTop / 2 - (platformSizeUnit / 2))-260, 0);

			glScalef(resumeBtnPressed, resumeBtnPressed, 0);
			glRotatef(-3, 0.0, 1.0, 0.0);
			//////////////////////////////////

			drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_RESUME]);
		glPopMatrix();


		//Boton de restart
		glPushMatrix();
			if (selectedBtnPause == RESTART_BUTTON) glColor4f(1, 1, 1, 1);
			else glColor4f(0.5, 0.5, 0.5, 1);
			posX = (borderRight / 4) + 170;
			glTranslatef(posX, (borderTop / 2 - (platformSizeUnit / 2))-260, 0);

			glScalef(restartBtnPressed, restartBtnPressed, 0);
			glRotatef(-1.5, 0.0, 1.0, 0.0);
			//////////////////////////////////

			drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_RESTART]);
		glPopMatrix();

		//Boton de menu
		glPushMatrix();
			if (selectedBtnPause == RESUME_MENU_BUTTON) glColor4f(1, 1, 1, 1);
			else glColor4f(0.5, 0.5, 0.5, 1);
			posX = (2 * borderRight / 4) + 170;
			glTranslatef(posX, (borderTop / 2 - (platformSizeUnit / 2))-260, 0);

			glScalef(menuBtnPressed, menuBtnPressed, 0);
			glRotatef(1.5, 0.0, 1.0, 0.0);
			//////////////////////////////////

			drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_MENU]);
		glPopMatrix();

		//Boton de instrucciones
		glPushMatrix();
			if (selectedBtnPause == INSTRUCTIONS_BUTTON) glColor4f(1, 1, 1, 1);
			else glColor4f(0.5, 0.5, 0.5, 1);
			posX = (3 * borderRight / 4) + 170;
			glTranslatef(posX, (borderTop / 2 - (platformSizeUnit / 2))-260, 0);

			glScalef(instructionsBtnPressed, instructionsBtnPressed, 0);
			glRotatef(3, 0.0, 1.0, 0.0);
			//////////////////////////////////

			drawPlatform(buttonSide, buttonSide, buttonSide, interTextures[LANG][BTN_INSTR]);
		glPopMatrix();

	glPopMatrix();

}

GLint getRandomCoin(){
	int numberOfCoin = rand() % 6;
	GLint typeOfCoin;
	if (numberOfCoin == 0){
		typeOfCoin = coinRed;
	}
	else if (numberOfCoin == 1 || numberOfCoin == 2){
		typeOfCoin = coinBlue;
	}
	else {
		typeOfCoin = coinGold;
	}

	return typeOfCoin;
}

void drawPowerUp(GLuint icon, int width, int height, float limitLow, float limitHigh)
{
	glPushMatrix();
	glTranslatef(-25, 4*platformSizeUnit, -0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, icon);
	glBegin(GL_QUADS);

	glTexCoord2d(limitLow, limitLow);
	glVertex3d(0, 0, 1);
	glTexCoord2d(limitHigh, limitLow);
	glVertex3d(width, 0, 1);
	glTexCoord2d(limitHigh, limitHigh);
	glVertex3d(width, height, 1);
	glTexCoord2d(limitLow, limitHigh);
	glVertex3d(0, height, 1);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawMovingPlatforms(){

	// 8000 = size of the background
	if (offset >= 2001 && offset <= 3500){
		difficultyPlatforms = 11;
	}
	else if (offset >= 3501 && offset <= 5000) {
		difficultyPlatforms = 12;
	}
	else if (offset >= 5001 && offset <= 6500) {
		difficultyPlatforms = 13;
	}
	else if (offset >= 6501) {
		difficultyPlatforms = 14;
	}

	for (int cnt = 0; cnt < 24; cnt++)
	{
		glPushMatrix();
		glTranslatef(posX[cnt], posY[cnt], 0);

		//Calcula el por ciento de la rotacición en x & y.
		//Posición en x más la mitad del ancho (platformSizeUnit*2) menos la mitad del ancho la pantalla
		glRotatef((((posX[cnt] + (platformSizeUnit * 2)) - (borderRight / 2)) / 683) * rotateX, 0.0, 1.0, 0.0);

		//Posición en y más la mitad del alto (platformSizeUnit) menos la mitad de la altura de la pantalla
		glRotatef((-(((posY[cnt] + (platformSizeUnit / 2)) - (borderTop / 2)) / 350) * rotateY), 1.0, 0.0, 0.0);

		// get the postion and take it away the offset to renew the platform
		if (posY[cnt] <= -99){
			platformWithCoin[cnt] = rand() % 10;
			showPlatform[cnt] = rand() % difficultyPlatforms;
			(showPlatform[cnt] <= 5) ? _hidden[cnt] = 0 : _hidden[cnt] = 1;
			coinType[cnt] = getRandomCoin();
		}

		if ((cnt >= 0 && cnt <= 5) || (cnt >= 12 && cnt <= 17)){
			if ((_hidden[cnt]==0) && (cnt % 2 == 0)){
				if (platformWithCoin[cnt] >= 6){
					drawPowerUp(coinType[cnt], 50, 50, 0.25, 0.75);
					coinShown[cnt] = 1;
				}
				else
				{
					coinShown[cnt] = 0;
				}
				drawPlatform(platformTop, platformBottom, platformSide, platformSide);
				numberOfPlatforms[0]++;
			}
			else
            {
                _hidden[cnt] = 1;
            }
		}
		else if ((cnt >= 6 && cnt <= 11) || (cnt >= 18 && cnt <= 23)) {
			if ((_hidden[cnt]==0) && (cnt % 2 == 1)){
				if (platformWithCoin[cnt] >= 6){
					drawPowerUp(coinType[cnt], 50, 50, 0.25, 0.75);
					coinShown[cnt] = 1;
				}
				else
				{
					coinShown[cnt] = 0;
				}
				drawPlatform(platformTop, platformBottom, platformSide, platformSide);
				numberOfPlatforms[1]++;
			}
			else
            {
                _hidden[cnt] = 1;
            }
		}


		//set Position -10000 for not counting it in collision check
        glPopMatrix();
	}
	numberOfPlatforms[0] = 0;
	numberOfPlatforms[1] = 0;
}

void drawGame()
{
	if (GAME_STATE == STATE_PAUSE || GAME_STATE == STATE_LOST)
		glColor4f(0.4, 0.4, 0.4, 1.0);
	else
		glColor4f(1.0, 1.0, 1.0, 1.0);

	drawBackground();
	glPushMatrix();
		if (!anim && GAME_STATE != STATE_LOST)
		{ 
			drawMovingPlatforms();

			if(!jumping && !falling) hopsterY = posY[onPlatform];


			if(hopsterY > borderBottom - 0.2*hopsterHeight){
				//Draw current image of Hopster animation
				glPushMatrix();
				glTranslatef(hopsterX, 0, 0);
				glTranslatef(0, 0, 300);
				if(hopsterY > borderTop/2) hopsterY += 0.004 * hopsterY;
				glTranslatef(0, hopsterY, 0);
				drawIcon(animationJump[animJumpCurrent], hopsterWidth, hopsterHeight, 0, 1, 0);
				glPopMatrix();

				//Draw second half of Hopster, when he's passing the horizontal borders
				if (hopsterX < 0 || hopsterX + hopsterWidth > borderRight)
				{
					//Calculate where the second half has to be
					int x;
					if (hopsterX < 0) x = borderRight + hopsterX;
					else x = hopsterX - borderRight;


					//Draw current image of Hopster animation
					glPushMatrix();
					glTranslatef(x, 0, 0);
					glTranslatef(0, 0, 300);
					glTranslatef(0, hopsterY, 0);
					drawIcon(animationJump[animJumpCurrent], hopsterWidth, hopsterHeight, 0, 1, 0);
					glPopMatrix();
				}
			}
			else if(GAME_STATE != STATE_LOST)
			{
				gameMusic->stop();
				pauseMusic->stop();
				menuMusic->stop();
				gameOverMusic->setPosition(0);
				gameOverMusic->play();
				gameOverMusic->setRepeat(true);
			
				initGameOverMenu();
				GAME_STATE = STATE_LOST;
			}

		}

	glPopMatrix();
}

void dibuja()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (GAME_STATE == STATE_MENU)
	{
		drawMenu();
	}
	else if (GAME_STATE == STATE_GAME || GAME_STATE == STATE_PAUSE || GAME_STATE == STATE_LOST)
	{
		drawGame();
	}

	glColor4f(1.0, 1.0, 1.0, 1.0);
	if (counting && currentCountDownNumber >= 0)
	{
		glPushMatrix();
		glTranslatef((borderRight / 2) - 200, (borderTop / 2) - 200, 0);
		drawIcon(countDown[currentCountDownNumber], 400, 400, 0, 1, 0);
		glPopMatrix();
	}
	else if (GAME_STATE == STATE_PAUSE)
	{
		drawPauseMenu();
	}
	else if (GAME_STATE == STATE_LOST)
	{
		drawGameOverMenu();
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(borderLeft, borderRight, borderBottom, borderTop, borderNear, borderFar);
	// glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void key(unsigned char key, int x, int y)
{
	if (!anim)
		switch (key)
	{
		case 32:
		case 13:

			if (key != 13 && GAME_STATE == STATE_GAME && !jumping && !falling)
			{
				jumping = true;
				onPlatform = -1;
				glutTimerFunc(30, jumpTimer, 1);
			}
			else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_MENU && !playPressed && !configPressed && !highscorePressed)
			{

				sfxButton->setPosition(0);
				sfxButton->play();

				if (selectedBtn == PLAY_BUTTON)
				{
					playPressed = true;
					playBtnPressed = 1.4;
				}
				else if (selectedBtn == CONFIG_BUTTON)
				{
					configPressed = true;
					configBtnPressed = 1.4;
				}
				else if (selectedBtn == HIGH_S_BUTTON)
				{
					highscorePressed = true;
					highscoreBtnPressed = 1.4;


				}else if (selectedBtn == CREDITS_BUTTON){
					creditsPressed = true;
				}
			}
			else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_HIGH)
			{
				sfxButton->setPosition(0);
				sfxButton->play();

				hBackPressed = true;
				hBackBtnPressed = 1.4;
			}else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CREDITS && !crBackPressed) {
				sfxButton->setPosition(0);
				sfxButton->play();
				crBackPressed = true;
				crBackBtnPressed = 1.4;
			}else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CONFIG && !cBackPressed)
			{
				sfxButton->setPosition(0);
				sfxButton->play();

				if (selectedBtnConfig == C_BACK_BUTTON)
				{
					cBackBtnPressed = 1.4;
					cBackPressed = true;
				}


				else if (selectedBtnConfig == ENGLISH_BUTTON)
				{
					LANG = LANG_ENG;
				}
				else if (selectedBtnConfig == SPANISH_BUTTON)
				{
					LANG = LANG_ESP;
				}
				else if (selectedBtnConfig == GERMAN_BUTTON)
				{
					LANG = LANG_DEU;
				}
				else if (selectedBtnConfig == MUSIC_ON_BUTTON)
				{
					VOL_MUSIC = 0.5;
					updateVolume();
				}
				else if (selectedBtnConfig == MUSIC_OFF_BUTTON)
				{
					VOL_MUSIC = 0.0;
					updateVolume();
				}
				else if (selectedBtnConfig == SOUND_ON_BUTTON)
				{
					VOL_SFX = 1.0;
					updateVolume();
				}
				else if (selectedBtnConfig == SOUND_OFF_BUTTON)
				{
					VOL_SFX = 0.0;
					updateVolume();
				}
			}
			else if (GAME_STATE == STATE_PAUSE && !resumePressed && !restartPressed && !menuPressed && !instructionsPressed)
			{
				sfxButton->setPosition(0);
				sfxButton->play();


				if (selectedBtnPause == RESUME_BUTTON)
				{
					resumeBtnPressed = 1.4;
					resumePressed = true;
				}
				if (selectedBtnPause == RESTART_BUTTON)
				{
					restartBtnPressed = 1.4;
					restartPressed = true;
				}
				if (selectedBtnPause == RESUME_MENU_BUTTON)
				{
					menuBtnPressed = 1.4;
					menuPressed = true;
				}
				if (selectedBtnPause == INSTRUCTIONS_BUTTON)
				{
					instructionsBtnPressed = 1.4;
					instructionsPressed = true;
				}
			}
			else if (GAME_STATE == STATE_LOST && !restartPressed && !menuPressed)
			{
				sfxButton->setPosition(0);
				sfxButton->play();

				if (selectedBtnGameOver == RESTART_BUTTON)
				{
					restartBtnPressed = 1.4;
					restartPressed = true;
				}
				if (selectedBtnGameOver == RESUME_MENU_BUTTON)
				{
					menuBtnPressed = 1.4;
					menuPressed = true;
				}
			}
			break;
		case 'p':
		case 'P':
			if (GAME_STATE == STATE_GAME) {
				gameMusic->stop();
				pauseMusic->setPosition(0);
				pauseMusic->play();
				pauseMusic->setRepeat(true);

				GAME_STATE = STATE_PAUSE;
				initPauseMenu();
			}
			else if (GAME_STATE == STATE_PAUSE)
			{
				pauseMusic->stop();
				gameMusic->play();

				GAME_STATE = STATE_GAME;
				glutTimerFunc(5, gameTimer, 1);
			}
			break;
	}

	glutPostRedisplay();
}

/*Return the button that is touched*/
int overButton(int x, int y) {
	if(GAME_STATE == STATE_MENU && MENU_STATE == MS_MENU 
		&& x>960 && x<1125 && y>645 && y<800){
		return CREDITS_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_MENU 
		&& x > 546 && x < 784 && y > 224 && y < 294) {
		return PLAY_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_MENU 
		&& x > 70 && x < 324 && y > 166 && y < 234) {
		return CONFIG_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_MENU 
		&& x > 1026 && x < 1279 && y > 166 && y < 234) {
		return HIGH_S_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CREDITS
		&& x > 34 && x<276 && y>716 && y<777){
			return CR_BACK_BUTTON;
	} else if (GAME_STATE == STATE_MENU &&  MENU_STATE == MS_HIGH 
		&& x > 20 && x < 277 && y > 714 && y < 786) {
		return H_BACK_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CONFIG 
		&& x > 20 && x < 277 && y > 714 && y < 786) {
		return C_BACK_BUTTON;
	}else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CONFIG 
		&& x > 472 && x < 630 && y > 556 && y < 594) {
		return ENGLISH_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CONFIG 
		&& x > 708 && x < 884 && y > 556 && y < 596) {
		return SPANISH_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CONFIG 
		&& x > 952 && x < 1134 && y > 556 && y < 594) {
		return GERMAN_BUTTON;
	}else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CONFIG &&
		((LANG == LANG_ENG && x > 716 && x < 790 && y > 404 && y < 444) //boton en ingles
		|| (LANG == LANG_ESP && x > 710 && x < 896 && y > 408 && y < 442) //boton en espanol
		|| (LANG == LANG_DEU && x > 720 && x < 784 && y > 406 && y < 444))) { //boton en aleman
		return MUSIC_ON_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CONFIG &&
		((LANG == LANG_ENG && x > 958 && x < 1048 && y > 404 && y < 444) //boton en ingles
		|| (LANG == LANG_ESP && x > 962 && x < 1124 && y > 408 && y < 442) //boton en espanol
		|| (LANG == LANG_DEU && x > 960 && x < 1042 && y > 406 && y < 444))) { //boton en aleman
		return MUSIC_OFF_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CONFIG &&
		((LANG == LANG_ENG && x > 716 && x < 790 && y > 204 && y < 244) //boton en ingles
		|| (LANG == LANG_ESP && x > 710 && x < 896 && y > 208 && y < 242) //boton en espanol
		|| (LANG == LANG_DEU && x > 720 && x < 784 && y > 206 && y < 244))) { //boton en aleman
		return SOUND_ON_BUTTON;
	} else if (GAME_STATE == STATE_MENU && MENU_STATE == MS_CONFIG &&
		((LANG == LANG_ENG && x > 958 && x < 1048 && y > 204 && y < 244) //boton en ingles
		|| (LANG == LANG_ESP && x > 962 && x < 1124 && y > 208 && y < 242) //boton en espanol
		|| (LANG == LANG_DEU && x > 960 && x < 1042 && y > 206 && y < 244))) { //boton en aleman
		return SOUND_OFF_BUTTON;
	} else if (GAME_STATE == STATE_PAUSE 
		&& x > 42 && x < 296 && y > 150 && y < 210) {
		return RESUME_BUTTON;
	} else if (GAME_STATE == STATE_PAUSE 
		&& x > 386 && x < 643 && y > 150 && y < 210) {
		return RESTART_BUTTON;
	} else if (GAME_STATE == STATE_PAUSE 
		&& x > 728 && x < 974 && y > 150 && y < 210) {
		return RESUME_MENU_BUTTON;
	} else if (GAME_STATE == STATE_PAUSE 
		&& x > 1064 && x < 1316 && y > 150 && y < 210) {
		return INSTRUCTIONS_BUTTON;
	} else if (GAME_STATE == STATE_LOST 
		&& x > 386 && x < 643 && y > 210 && y < 270) {
		return RESTART_BUTTON;
	} else if (GAME_STATE == STATE_LOST 
		&& x > 728 && x < 974 && y > 210 && y < 270) {
		return RESUME_MENU_BUTTON;
	} else {
		return NO_BUTTON;
	} 
}


void performButtonPress(bool keyUp, int x, int y)
{
	if (GAME_STATE == STATE_MENU)
	{
		if (playPressed)
		{
			playBtnPressed = 1.5;
			glutPostRedisplay();

			if (keyUp || overButton(x, y) == PLAY_BUTTON)
			{
				anim = true;
				glutTimerFunc(10, startTimer, 1);
				menuMusic->stop();
				pauseMusic->setPosition(0);
				pauseMusic->play();
			}

			playPressed = false;
		}

		else if (configPressed)
		{
			configBtnPressed = 1.5;
			glutPostRedisplay();

			if (keyUp || overButton(x, y) == CONFIG_BUTTON)
			{
				anim = true;
				selectedBtnConfig = ENGLISH_BUTTON;
				glutTimerFunc(10, configTimer, 1);
			}

			configPressed = false;
		}

		else if (highscorePressed)
		{
			highscoreBtnPressed = 1.5;
			glutPostRedisplay();

			if (keyUp || overButton(x, y) == HIGH_S_BUTTON)
			{
				anim = true;
				glutTimerFunc(10, highscoreTimer, 1);
			}

			highscorePressed = false;
		}else if (creditsPressed) {
			glutPostRedisplay();

			if (keyUp || overButton(x, y) == CREDITS_BUTTON) {
				anim = true;
				glutTimerFunc(10, creditsTimer, 1);
			}

			creditsPressed = false;
		}else if (crBackPressed)
		{
			crBackPressed = 1.5;

			glutPostRedisplay();

			if (keyUp || overButton(x, y) == CR_BACK_BUTTON)
			{
				anim = true;
				glutTimerFunc(10, backToMainTimer, 1);
			}

			crBackPressed = false;
		}else if (cBackPressed)
		{
			cBackPressed = 1.5;

			glutPostRedisplay();

			if (keyUp || overButton(x, y) == C_BACK_BUTTON)
			{
				anim = true;
				glutTimerFunc(10, backToMainTimer, 1);
			}

			cBackPressed = false;
		}else if (hBackPressed) {
			hBackPressed = 1.5;
			glutPostRedisplay();

			if (keyUp || overButton(x, y) == H_BACK_BUTTON) {
				anim = true;
				glutTimerFunc(10, backToMainTimer, 1);
			}

			hBackPressed = false;
		}
	}
	
	else if (GAME_STATE == STATE_PAUSE || GAME_STATE == STATE_LOST)
	{
		if (resumePressed)
		{
			resumeBtnPressed = 1.5;
			glutPostRedisplay();

			if (keyUp || overButton(x, y) == RESUME_BUTTON)
			{
				pauseMusic->stop();
				gameMusic->play();

				GAME_STATE = STATE_GAME;
				glutTimerFunc(5, gameTimer, 1);
			}

			resumePressed = false;
		}
		else if (restartPressed)
		{
			restartBtnPressed = 1.5;
			glutPostRedisplay();

			if (keyUp || overButton(x, y) == RESTART_BUTTON)
			{
				anim = true;
				GAME_STATE = STATE_GAME;
				glutPostRedisplay();
				glutTimerFunc(10, startTimer, 1);
			}

			restartPressed = false;
		}
		else if (menuPressed)
		{
			menuBtnPressed = 1.5;
			glutPostRedisplay();

			
			if (keyUp || overButton(x, y) == RESUME_MENU_BUTTON)
			{
				GAME_STATE = STATE_MENU;
				pauseMusic->stop();
				initMenu();
			}

			menuPressed = false;
		}
		else if (instructionsPressed)
		{
			instructionsBtnPressed = 1.5;
			glutPostRedisplay();

			if (keyUp || overButton(x, y) == INSTRUCTIONS_BUTTON)
			{
				//Show instructions
			}

			instructionsPressed = false;
		}
	}
}

static void keyUp(unsigned char key, int x, int y)
{
	if (GAME_STATE != STATE_GAME)
		switch (key)
	{
		case 13:
		case 32:
			performButtonPress(true, x, y);
	}
}

static void specialKeyUp(int key, int x, int y)
{
	if (GAME_STATE == STATE_GAME)
		switch (key)
	{
		case GLUT_KEY_LEFT:
			moveLeft = false;
			break;
		case GLUT_KEY_RIGHT:
			moveRight = false;
			break;
	}
}

static void specialKey(int key, int x, int y)
{
	if (GAME_STATE == STATE_MENU && !anim 
			&& (MENU_STATE == MS_MENU || MENU_STATE == MS_CONFIG) 
			&& !playPressed && !configPressed && !highscorePressed && !creditsPressed) {

		switch (key) {

		case GLUT_KEY_LEFT:
			//Actual state is config state and moving to the next 
			if (MENU_STATE == MS_CONFIG && selectedBtnConfig > C_BACK_BUTTON) selectedBtnConfig--;
			else if (MENU_STATE == MS_CONFIG) selectedBtnConfig = GERMAN_BUTTON;
			if (MENU_STATE == MS_CONFIG) break;

		case GLUT_KEY_DOWN:
			if (MENU_STATE == MS_MENU && selectedBtn > CONFIG_BUTTON) selectedBtn--;
			else if (MENU_STATE == MS_MENU) selectedBtn = HIGH_S_BUTTON;
			else if (MENU_STATE == MS_CONFIG && selectedBtnConfig == C_BACK_BUTTON) selectedBtnConfig = ENGLISH_BUTTON;
			else if (MENU_STATE == MS_CONFIG && selectedBtnConfig == ENGLISH_BUTTON) selectedBtnConfig = MUSIC_ON_BUTTON;
			else if (MENU_STATE == MS_CONFIG && selectedBtnConfig >= SPANISH_BUTTON && selectedBtnConfig <= MUSIC_OFF_BUTTON) selectedBtnConfig += 2;
			else if (MENU_STATE == MS_CONFIG) selectedBtnConfig = C_BACK_BUTTON;
			break;
		case GLUT_KEY_RIGHT:
			if (MENU_STATE == MS_CONFIG && selectedBtnConfig < SOUND_OFF_BUTTON) selectedBtnConfig++;
			else if (MENU_STATE == MS_CONFIG) selectedBtnConfig = C_BACK_BUTTON;
			if (MENU_STATE == MS_CONFIG) break;
		case GLUT_KEY_UP:
			if (MENU_STATE == MS_MENU && selectedBtn < HIGH_S_BUTTON) selectedBtn++;
			else if (MENU_STATE == MS_MENU) selectedBtn = CONFIG_BUTTON;
			else if (MENU_STATE == MS_CONFIG && selectedBtnConfig == C_BACK_BUTTON) selectedBtnConfig = SOUND_ON_BUTTON;
			else if (MENU_STATE == MS_CONFIG && selectedBtnConfig >= MUSIC_ON_BUTTON) selectedBtnConfig -= 2;
			else if (MENU_STATE == MS_CONFIG) selectedBtnConfig = CONFIG_BUTTON;
			break;

		}
	}
	else if (GAME_STATE == STATE_PAUSE && !resumePressed && !restartPressed && !menuPressed && !instructionsPressed)
		switch (key)
	{
		case GLUT_KEY_LEFT:
		case GLUT_KEY_DOWN:
			if (selectedBtnPause == RESUME_BUTTON) selectedBtnPause = INSTRUCTIONS_BUTTON;
			else selectedBtnPause--;
			break;
		case GLUT_KEY_RIGHT:
		case GLUT_KEY_UP:
			if (selectedBtnPause == INSTRUCTIONS_BUTTON) selectedBtnPause = RESUME_BUTTON;
			else selectedBtnPause++;
			break;
	}
	else if(GAME_STATE == STATE_LOST && !restartPressed && !menuPressed)
		switch(key)
	{
		case GLUT_KEY_LEFT:
		case GLUT_KEY_DOWN:
		case GLUT_KEY_RIGHT:
		case GLUT_KEY_UP:
			if (selectedBtnGameOver == RESTART_BUTTON) selectedBtnGameOver = RESUME_MENU_BUTTON;
			else selectedBtnGameOver = RESTART_BUTTON;
			break;
	}

	if (GAME_STATE == STATE_GAME)
		switch (key)
	{
		case GLUT_KEY_LEFT:
            if(!moveLeft)
			{
				moveLeft = true;
				moveRight = false;


				if(!jumping && !falling) glutTimerFunc(0, walkTimer, 1);
			}
			break;
		case GLUT_KEY_RIGHT:
           if(!moveRight)
			{
				moveRight = true;
				moveLeft = false;


				if(!jumping && !falling) glutTimerFunc(0, walkTimer, 1);
			}
			break;
	}

	glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
	y = 800 - y;

	if (GAME_STATE == STATE_MENU && (MENU_STATE == MS_MENU || MENU_STATE == MS_CONFIG)
		&& !configPressed && !highscorePressed && !playPressed && !cBackPressed && !hBackPressed && !creditsPressed)
	{
		int btn = overButton(x, y);


		if (btn == NO_BUTTON) {
			return;
		} else if (btn == PLAY_BUTTON) {

			selectedBtn = PLAY_BUTTON;

		} else if (btn == CONFIG_BUTTON) {
			selectedBtn = CONFIG_BUTTON;

		} else if (btn == HIGH_S_BUTTON) {

			selectedBtn = HIGH_S_BUTTON;

		} else if (btn == CREDITS_BUTTON) {
			selectedBtn = CREDITS_BUTTON;

		} else if (btn == C_BACK_BUTTON) {
			selectedBtnConfig = C_BACK_BUTTON;

		} else if (btn == CR_BACK_BUTTON) {
			selectedBtnConfig = CR_BACK_BUTTON;

		} else if (btn == H_BACK_BUTTON) {

			selectedBtn = H_BACK_BUTTON;

		} else if (btn == ENGLISH_BUTTON) {

			selectedBtnConfig = ENGLISH_BUTTON;

		} else if (btn == SPANISH_BUTTON) {

			selectedBtnConfig = SPANISH_BUTTON;

		} else if (btn == GERMAN_BUTTON) {

			selectedBtnConfig = GERMAN_BUTTON;

		} else if (btn == MUSIC_ON_BUTTON) {

			selectedBtnConfig = MUSIC_ON_BUTTON;

		} else if (btn == MUSIC_OFF_BUTTON) {

			selectedBtnConfig = MUSIC_OFF_BUTTON;

		} else if (btn == SOUND_ON_BUTTON) {
			selectedBtnConfig = SOUND_ON_BUTTON;


		} else if (btn == SOUND_OFF_BUTTON) {
			selectedBtnConfig = SOUND_OFF_BUTTON;
		}

		glutPostRedisplay();
	}
	else if (GAME_STATE == STATE_PAUSE && !resumePressed && !restartPressed && !menuPressed && !instructionsPressed)
	{
		int btn = overButton(x, y);

		if (btn == 0) return;
		else if (btn == RESUME_BUTTON)
		{
			selectedBtnPause = RESUME_BUTTON;
		}
		else if (btn == RESTART_BUTTON)
		{
		 	selectedBtnPause = RESTART_BUTTON;
		}
		else if (btn == RESUME_MENU_BUTTON)
		{
			selectedBtnPause = RESUME_MENU_BUTTON;
		}
		else if (btn == INSTRUCTIONS_BUTTON)
		{
			selectedBtnPause = INSTRUCTIONS_BUTTON;
		}
		glutPostRedisplay();
	}
	else if (GAME_STATE == STATE_LOST && !restartPressed && !menuPressed)
	{
		int btn = overButton(x, y);

		if (btn == 0) return;
		else if (btn == RESTART_BUTTON)
		{
			selectedBtnGameOver = RESTART_BUTTON;
		}
		else if (btn == RESUME_MENU_BUTTON)
		{
		 	selectedBtnGameOver = RESUME_MENU_BUTTON;
		}
		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y) {

	y = 800 - y;

	cout << "Current x: " << x;
	cout << " - current y: " << y << endl;

	if (!anim)
	{
		if (GAME_STATE == STATE_MENU && state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		{
			int btn = overButton(x, y);
			if (btn != NO_BUTTON) {

				sfxButton->setPosition(0);
				sfxButton->play();
			}
			if (btn == PLAY_BUTTON) {

				playPressed = true;
				playBtnPressed = 1.4;
			

			} else if (btn == CONFIG_BUTTON) {

				configPressed = true;
				configBtnPressed = 1.4;
			

			} else if (btn == HIGH_S_BUTTON) {

				highscorePressed = true;
				highscoreBtnPressed = 1.4;

			}else if (btn == CREDITS_BUTTON) {

				creditsPressed = true;
			
			} else if (btn == CR_BACK_BUTTON) {
				crBackPressed = true;
				crBackBtnPressed = 1.4;
			

			} else if (btn == C_BACK_BUTTON) {

				cBackPressed = true;
				cBackBtnPressed = 1.4;
			
			} else if (btn == H_BACK_BUTTON) {
				hBackPressed = true;
				hBackBtnPressed = 1.4;



			} else if (btn == ENGLISH_BUTTON) {

				LANG = LANG_ENG;
			

			} else if (btn == SPANISH_BUTTON) {

				LANG = LANG_ESP;
			

			} else if (btn == GERMAN_BUTTON) {

				LANG = LANG_DEU;
			

			} else if (btn == MUSIC_ON_BUTTON) {

				VOL_MUSIC = 0.5;
				updateVolume();


			} else if (btn == MUSIC_OFF_BUTTON) {

				VOL_MUSIC = 0.0;
				updateVolume();
			

			} else if (btn == SOUND_ON_BUTTON) {

				VOL_SFX = 1.0;
				updateVolume();
			

			} else if (btn == SOUND_OFF_BUTTON) {

				VOL_SFX = 0.0;
				updateVolume();
			
			} 

			glutPostRedisplay();
		}
		if (GAME_STATE == STATE_PAUSE && state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		{
			int btn = overButton(x, y);
			if (btn != NO_BUTTON)
			{
				sfxButton->setPosition(0);
				sfxButton->play();
			}
			if (btn == RESUME_BUTTON)
			{
				resumePressed = true;
				resumeBtnPressed = 1.4;
			}
			else if (btn == RESTART_BUTTON)
			{
				restartPressed = true;
				restartBtnPressed = 1.4;
			}
			else if (btn == RESUME_MENU_BUTTON)
			{
				menuPressed = true;
				menuBtnPressed = 1.4;
			}
			else if (btn == INSTRUCTIONS_BUTTON)
			{
				instructionsPressed = true;
				instructionsBtnPressed = 1.4;
			}

			glutPostRedisplay();
		}
		if (GAME_STATE == STATE_LOST && state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		{
			int btn = overButton(x, y);
			if (btn != NO_BUTTON)
			{
				sfxButton->setPosition(0);
				sfxButton->play();
			}
			if (btn == RESTART_BUTTON)
			{
				restartPressed = true;
				restartBtnPressed = 1.4;
			}
			else if (btn == RESUME_MENU_BUTTON)
			{
				menuPressed = true;
				menuBtnPressed = 1.4;
			}

			glutPostRedisplay();
		}
		else if (GAME_STATE != STATE_GAME && state == GLUT_UP && button == GLUT_LEFT_BUTTON)
		{
			performButtonPress(false, x, y);
		}
	}
}

void onMenu(int cmd)
{
	if (cmd == 100) exit(0);
	else if (cmd == SOUND_ON)
		VOL_SFX = 1.0;
	else if (cmd == SOUND_OFF)
		VOL_SFX = 0.0;
	else if (cmd == MUSIC_ON)
		VOL_MUSIC = 0.5;
	else if (cmd == MUSIC_OFF)
		VOL_MUSIC = 0.0;
	else
	{
		LANG = cmd;
	}
	updateVolume();
	glutPostRedisplay();

}

void addMenues(){
	char* lang;
	char* exit;
	char* music;
	char* sound;
	char* on;
	char* off;

	if (LANG == LANG_ESP)
	{
		lang = "Idioma";
		exit = "Salir";
		music = "Música";
		sound = "Sonido";
		on = "Encender";
		off = "Apagar";
	}
	else if (LANG == LANG_DEU)
	{
		lang = "Sprache";
		exit = "Beenden";
		music = "Musik";
		sound = "Effekte";
		on = "An";
		off = "Aus";
	}
	else
	{
		lang = "Language";
		exit = "Exit";
		music = "Music";
		sound = "Sound";
		on = "On";
		off = "Off";
	}

	int menuLanguages = glutCreateMenu(onMenu);
	glutAddMenuEntry("English", LANG_ENG);
	glutAddMenuEntry("Espanol", LANG_ESP);
	glutAddMenuEntry("Deutsch", LANG_DEU);

	int menuSound = glutCreateMenu(onMenu);
	glutAddMenuEntry(on, SOUND_ON);
	glutAddMenuEntry(off, SOUND_OFF);

	int _menuMusic = glutCreateMenu(onMenu);
	glutAddMenuEntry(on, MUSIC_ON);
	glutAddMenuEntry(off, MUSIC_OFF);

	int menuPrincipal = glutCreateMenu(onMenu);

	glutAddSubMenu(lang, menuLanguages);
	glutAddSubMenu(music, _menuMusic);
	glutAddSubMenu(sound, menuSound);
	glutAddMenuEntry(exit, 100);

	glutSetMenu(menuPrincipal);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Saves configuration properties to an .ini file when execution is exited
void writeIniFile()
{
	ofstream iniFile;
	iniFile.open("C:/configMonsterJumpB.txt");
	iniFile << "# Config file for Monster Jump" << endl;
	iniFile << "language:" << LANG << endl;
	iniFile << "sound:" << VOL_SFX << endl;
	iniFile << "music:" << VOL_MUSIC << endl;
	iniFile.close();
}

// Reads in configurations properties from an .ini file at the beginning of execution
void readIniFile()
{
	// Read the .ini file
	string line;
	ifstream iniFile("C:/configMonsterJump.txt");
	if (iniFile.is_open())
	{
		while (getline(iniFile, line))
		{
			// Ignore comments
			if (!(line.find("#") == 0))
			{
				// Split line at the position of colon and get key and value
				unsigned pos = line.find(":");  //Posicion de dos puntos
				string key = line.substr(0, pos);
				string value = line.substr(pos + 1);

				//Set the values for the distinct keys
				if (key.compare("language") == 0)
				{
					LANG = atoi(value.c_str());
				}
				else if (key.compare("sound") == 0)
				{
					VOL_SFX = atof(value.c_str());
				}
				else if (key.compare("music") == 0)
				{
					VOL_MUSIC = atof(value.c_str());
				}
			}
		}
		iniFile.close();
	}
}




// Main method running the game
int main(int argc, char** argv)
{
// Initializes the creation of random numbers
	srand(time(NULL));
	//readIniFile();

	//Sets up the window in glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1366, 700);
	glutInitWindowPosition(0, 0);

	glutCreateWindow(argv[0]);

	//Loads configuration data, initializes the setup and creates glut menus
	updateVolume();
	initMenu();
	addMenues();

	// Loads the textures and initializes the texture variables
	background = LoadGLTexture("../data/texturas/fondo.jpg", 1366, 8000);
	platformSide = LoadGLTexture("../data/texturas/platformSide.png", 256, 64);
	platformTop = LoadGLTexture("../data/texturas/platformTop.png", 256, 256);
	platformBottom = LoadGLTexture("../data/texturas/platformBottom.png", 256, 256);
	platformVenomSide = LoadGLTexture("../data/texturas/platformVenomSide.png", 256, 64);
	platformVenomTop = LoadGLTexture("../data/texturas/platformVenomTop.png", 256, 256);
	platformVenomBottom = LoadGLTexture("../data/texturas/platformVenomBottom.png", 256, 256);
	buttonSide = LoadGLTexture("../data/texturas/buttonSide.png", 240, 80);
	buttonPlay = LoadGLTexture("../data/texturas/buttonPlay.png", 240, 80);
	buttonJugar = LoadGLTexture("../data/texturas/buttonJugar.png", 240, 80);
	buttonSpielen = LoadGLTexture("../data/texturas/buttonSpielen.png", 240, 80);
	buttonSettings = LoadGLTexture("../data/texturas/buttonSettings.png", 240, 80);
	buttonConfig = LoadGLTexture("../data/texturas/buttonConfiguracion.png", 240, 80);
	buttonEinstellungen = LoadGLTexture("../data/texturas/buttonEinstellungen.png", 240, 80);
	buttonHighscore = LoadGLTexture("../data/texturas/buttonHighScore.png", 240, 80);
	buttonBack = LoadGLTexture("../data/texturas/buttonBack.png", 400, 400);
	buttonSalir = LoadGLTexture("../data/texturas/buttonSalir.png", 400, 400);
	buttonZurueck = LoadGLTexture("../data/texturas/buttonZurueck.png", 400, 400);
	buttonResume = LoadGLTexture("../data/texturas/buttonResume.png", 240, 80);
	buttonSeguir = LoadGLTexture("../data/texturas/buttonSeguir.png", 240, 80);
	buttonFortsetzen = LoadGLTexture("../data/texturas/buttonFortsetzen.png", 240, 80);
	buttonRestart = LoadGLTexture("../data/texturas/buttonRestart.png", 240, 80);
	buttonReiniciar = LoadGLTexture("../data/texturas/buttonReiniciar.png", 240, 80);
	buttonNeustart = LoadGLTexture("../data/texturas/buttonNeustart.png", 240, 80);
	buttonMenuEng = LoadGLTexture("../data/texturas/buttonMenu.png", 240, 80);
	buttonMenuEsp = LoadGLTexture("../data/texturas/buttonMenuEsp.png", 240, 80);
	buttonMenuDeu = LoadGLTexture("../data/texturas/buttonMenü.png", 240, 80);
	buttonInstEng = LoadGLTexture("../data/texturas/buttonInstructions.png", 240, 80);
	buttonInstEsp = LoadGLTexture("../data/texturas/buttonInstrucciones.png", 240, 80);
	buttonAnleitung = LoadGLTexture("../data/texturas/buttonAnleitung.png", 240, 80);
	configIcon = LoadGLTexture("../data/texturas/gear.png", 400, 400);
	highscoreIcon = LoadGLTexture("../data/texturas/HighscoreIcon.png", 400, 400);
	musicOnIcon = LoadGLTexture("../data/texturas/MusicOn.png", 400, 400);
	musicOffIcon = LoadGLTexture("../data/texturas/MusicOff.png", 400, 400);
	soundOnIcon = LoadGLTexture("../data/texturas/SoundOn.png", 400, 400);
	soundOffIcon = LoadGLTexture("../data/texturas/SoundOff.png", 400, 400);
	englishSelected = LoadGLTexture("../data/texturas/EnglishSelected.png", 400, 100);
	englishUnselected = LoadGLTexture("../data/texturas/EnglishUnselected.png", 400, 100);
	espanolSelected = LoadGLTexture("../data/texturas/EspanolSelected.png", 400, 100);
	espanolUnselected = LoadGLTexture("../data/texturas/EspanolUnselected.png", 400, 100);
	deutschSelected = LoadGLTexture("../data/texturas/DeutschSelected.png", 400, 100);
	deutschUnselected = LoadGLTexture("../data/texturas/DeutschUnselected.png", 400, 100);
	offSelected = LoadGLTexture("../data/texturas/OffSelected.png", 200, 100);
	offUnselected = LoadGLTexture("../data/texturas/OffUnselected.png", 200, 100);
	onSelected = LoadGLTexture("../data/texturas/OnSelected.png", 200, 100);
	onUnselected = LoadGLTexture("../data/texturas/OnUnselected.png", 200, 100);
	apagarSelected = LoadGLTexture("../data/texturas/ApagarSelected.png", 200, 100);
	apagarUnselected = LoadGLTexture("../data/texturas/ApagarUnselected.png", 200, 100);
	encenderSelected = LoadGLTexture("../data/texturas/EncenderSelected.png", 200, 100);
	encenderUnselected = LoadGLTexture("../data/texturas/EncenderUnselected.png", 200, 100);
	ausSelected = LoadGLTexture("../data/texturas/AusSelected.png", 200, 100);
	ausUnselected = LoadGLTexture("../data/texturas/AusUnselected.png", 200, 100);
	anSelected = LoadGLTexture("../data/texturas/AnSelected.png", 200, 100);
	anUnselected = LoadGLTexture("../data/texturas/AnUnselected.png", 200, 100);
	countDown[0] = LoadGLTexture("../data/texturas/count1.png", 200, 100);
	countDown[1] = LoadGLTexture("../data/texturas/count2.png", 200, 100);
	countDown[2] = LoadGLTexture("../data/texturas/count3.png", 200, 100);
	coinBlue = LoadGLTexture("../data/texturas/PowerUpCoinBlue.png", 200, 200);
	coinGold = LoadGLTexture("../data/texturas/PowerUpCoinGold.png", 200, 200);
	coinRed = LoadGLTexture("../data/texturas/PowerUpCoinRed.png", 200, 200);
	magnet = LoadGLTexture("../data/texturas/PowerUpMagnet.png", 400, 400);
	star = LoadGLTexture("../data/texturas/PowerUpStar.png", 400, 400);
	monsterJumpLogo = LoadGLTexture("../data/texturas/Logo.png", 400, 160);

	credits[LANG_ESP] = LoadGLTexture("../data/texturas/CreditsSpanish.png",1024, 690);
	credits[LANG_ENG] = LoadGLTexture("../data/texturas/CreditsEnglish.png",1024, 690);
	credits[LANG_DEU] = LoadGLTexture("../data/texturas/CreditsGerman.png",1024, 690);
	gameover[LANG_ESP] = LoadGLTexture("../data/texturas/GameOverSadSpanish.png",1024, 690);
	gameover[LANG_ENG] = LoadGLTexture("../data/texturas/GameOverSadEnglish.png",1024, 690);
	gameover[LANG_DEU] = LoadGLTexture("../data/texturas/GameOverSadGerman.png",1024, 690);

	// Initializes the array of textures of the possible powerups
	for (int i = 0; i < 24; i++){
		coinType[i] = coinGold;
	}


// Sets up the images in the array of textures for the intro animation of hopster
	for (int i = 0; i<48; i++)
	{

		//Obtain individual path
		string pathInit = "../data/texturas/AnimationIntro (";

		string pathEnding = ").png";

		stringstream stream;
		stream << pathInit << i + 1 << pathEnding;
		string path = stream.str();

		cout << path << endl;
		animationIntro[i] = LoadGLTexture(path.c_str(), 700, 880);
	}


	 //Sets up the images in the array of textures for the jump animation of hopster
	for (int i = 0; i<48; i++)
	{

		//Obtain individual path
		string pathInit = "../data/texturas/AnimationJump (";

		string pathEnding = ").png";

		stringstream stream;
		stream << pathInit << i + 1 << pathEnding;
		string path = stream.str();

		cout << path << endl;
		animationJump[i] = LoadGLTexture(path.c_str(), 700, 880);
	}



	initInterTextures();

	glEnable(GL_DEPTH_TEST); // to differenciate which vertices are closer/farer away

		

	// Sets up the different methods needed for the game
	glutDisplayFunc(dibuja);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKey);
	glutSpecialUpFunc(specialKeyUp);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMotion);
	glutReshapeFunc(reshape);
	atexit(writeIniFile); // calls method writing configuration data in file before exiting
	glutMainLoop();
	return 0;
}
