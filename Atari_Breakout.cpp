// Atari_Breakout.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "Atari_Functions.h"

using namespace std;
using namespace sf;
using namespace sfp;


//options
const int pixelConstant(3);
const Color wallColor(255, 255, 255);
const Color paddleColor(2, 89, 252);

int main()
{
    RenderWindow window(VideoMode(600, 800), "Breakout");
    World world(Vector2f(0, 0));

    //walls
    PhysicsRectangle leftWall;
    leftWall.setSize(Vector2f(10, 725));
    leftWall.setCenter(Vector2f(5, 437.5));
    leftWall.setStatic(true);
    world.AddPhysicsBody(leftWall);
    //right
    PhysicsRectangle rightWall;
    rightWall.setSize(Vector2f(10, 725));
    rightWall.setCenter(Vector2f(595, 437.5));
    rightWall.setStatic(true);
    world.AddPhysicsBody(rightWall);
    //ceiling
    PhysicsRectangle ceiling;
    ceiling.setSize(Vector2f(600, 50));
    ceiling.setCenter(Vector2f(300, 50));
    ceiling.setStatic(true);
    world.AddPhysicsBody(ceiling);

    //paddle and side bars
    PhysicsRectangle paddle;
    paddle.setSize(Vector2f(50, 25));
    paddle.setCenter(Vector2f(300, 725));
    paddle.setFillColor(paddleColor);
    world.AddPhysicsBody(paddle);
    //side bars
    PhysicsRectangle rightBar;
    rightBar.setSize(Vector2f(10, 25));
    rightBar.setCenter(Vector2f(5, 725));
    rightBar.setFillColor(paddleColor);
    world.AddPhysicsBody(rightBar);
    PhysicsRectangle leftBar;
    leftBar.setSize(Vector2f(10, 25));
    leftBar.setCenter(Vector2f(595, 725));
    leftBar.setFillColor(paddleColor);
    world.AddPhysicsBody(leftBar);
    



    vector<PhysicsRectangle> rects = vector<PhysicsRectangle> { leftWall, rightWall, ceiling, paddle, rightBar, leftBar };
    Clock clock;
    Time lastTime(clock.getElapsedTime());

    while (true) {
        Time currentTime(clock.getElapsedTime());
        int ellapsedMS((lastTime - currentTime).asMilliseconds());

        if (ellapsedMS > 3) {
            world.UpdatePhysics(ellapsedMS);
            lastTime = currentTime;
        }

        window.clear();
        displayStaticRectangles(rects, window);

        window.display();

    }
}

