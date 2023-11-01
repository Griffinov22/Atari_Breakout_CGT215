#pragma once
//these are functions used for my Breakout game
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

void shout();

void loadTex(Texture &tex, string path);

void displayStaticRectangles(vector<PhysicsRectangle> sprites, RenderWindow &window);

void movePaddle(PhysicsRectangle &paddle, int ellapsedMS, float pixelConstant);

void showStartingScreen(RenderWindow& window, Font font);

void dropBallIn(PhysicsRectangle& ball, World& world, bool &isPlaying);

/// <summary>
/// Seconds to freeze program. Equal to JavaScript .setTimeOut(()=>{...},duration);
/// </summary>
/// <param name="duration"></param>
void wait(int duration);