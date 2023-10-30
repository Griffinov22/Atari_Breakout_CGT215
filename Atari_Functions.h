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