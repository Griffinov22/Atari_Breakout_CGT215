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

void loadImage(Texture &tex, string path);

void displayStaticRectangles(vector<PhysicsRectangle> sprites, RenderWindow &window);

void getUserInput(PhysicsRectangle paddle, int ellapsedMS, int pixelConstant);