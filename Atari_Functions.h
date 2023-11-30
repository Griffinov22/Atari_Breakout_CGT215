#pragma once
//these are functions used for my Breakout game
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

void shout();

void loadTex(Texture &tex, string path);

void loadMusic(SoundBuffer &buff, string path);

void displayStaticRectangles(vector<PhysicsRectangle> sprites, RenderWindow &window);

void movePaddle(PhysicsRectangle &paddle, int ellapsedMS, float pixelConstant, bool& hasAppliedBoost);

void showStartingScreen(RenderWindow& window, Font font, Sound &countDownMusic);

void dropBallIn(PhysicsRectangle& ball, World& world, bool &isPlaying, bool &hasAppliedBoost);

/// <summary>
/// Seconds to freeze program. Equal to JavaScript .setTimeOut(()=>{...},duration);
/// </summary>
/// <param name="duration"></param>
void wait(int duration);

void fillBrickList(PhysicsShapeList<PhysicsRectangle> &bricks,
	Color redBrick, Color orangeBrick, Color greenBrick, Color yellowBrick, 
	PhysicsRectangle &ball, RenderWindow &window, World &world, 
	bool &hasAppliedBoost, int &score);

void showEasterEgg(RenderWindow &window, Font font, int &lives);

void showSecondLevelScreen(RenderWindow &window, Font font, Color redBrick, Color orangeBrick, Color greenBrick, Color yellowBrick, Sound &nextLevelSound, int &lives);

void showEndingScreen(RenderWindow &window, Font font, int score, Sound &endGameMusic, bool hasWon, Color yellowBrick);