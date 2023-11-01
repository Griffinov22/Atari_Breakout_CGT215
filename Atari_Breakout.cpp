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
const float pixelConstant(0.5);
const Color wallColor(255, 255, 255);
const Color paddleColor(2, 89, 252);
    //brick colors
const Color redBrick(254, 31, 33);
const Color orangeBrick(251, 128, 20);
const Color greenBrick(61, 255, 64);
const Color yellowBrick(34, 32, 246);
    //character size
const int characterSz(50);

int main()
{
    RenderWindow window(VideoMode(600, 800), "Breakout");
    World world(Vector2f(0, 0));
    int lives(3);
    int score(0);
    bool isPlaying(false);
    bool hasSeenStartingScreen(false);

    //ball
    PhysicsRectangle ball;
    ball.setSize(Vector2f(10, 10));
    ball.onCollision = [](PhysicsBodyCollisionResult res) {
        cout << "collision" << endl;
    };
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
    //floor
    PhysicsRectangle floor;
    floor.setSize(Vector2f(600, 50));
    floor.setCenter(Vector2f(300, 850));
    floor.setStatic(true);
    world.AddPhysicsBody(floor);
    floor.onCollision = [&lives, &isPlaying, &ball, &world](PhysicsBodyCollisionResult res) {
        if (res.object2 == ball) {
            isPlaying = false;
            lives--;
            world.RemovePhysicsBody(ball);

        }
        };

    //paddle and side bars
    PhysicsRectangle paddle;
    paddle.setSize(Vector2f(50, 15));
    paddle.setCenter(Vector2f(300, 725));
    paddle.setFillColor(paddleColor);
    paddle.setStatic(true);
    world.AddPhysicsBody(paddle);
    paddle.onCollision = [&ball, &paddle](PhysicsBodyCollisionResult res) {
        if (res.object2 == ball) {
            FloatRect padSz(paddle.getGlobalBounds());
            Vector2f ballPos(ball.getCenter());

            //check if ball collided on left,center,right side of paddle
            //do not apply force if ball hits middle 20% of paddle
            if (ballPos.x < (padSz.left + (padSz.width * 0.4))) {
                ball.applyImpulse(Vector2f(-0.05,0));
            }
            else if (ballPos.x > ((padSz.left + (padSz.width * 0.6)))) {
                ball.applyImpulse(Vector2f(0.05,0));
            }
        }
        };
    //side bars
    PhysicsRectangle rightBar;
    rightBar.setSize(Vector2f(10, 15));
    rightBar.setCenter(Vector2f(5, 725));
    rightBar.setFillColor(paddleColor);
    world.AddPhysicsBody(rightBar);
    PhysicsRectangle leftBar;
    leftBar.setSize(Vector2f(10, 15));
    leftBar.setCenter(Vector2f(595, 725));
    leftBar.setFillColor(paddleColor);
    world.AddPhysicsBody(leftBar);


    PhysicsShapeList<PhysicsRectangle> bricks;
    //12 bricks here
    Color brickColor;
    for (int j(0); j < 8; j++) {
        int starterX(5);
        int starterY(175 + (j * 30));
        switch (j) {
        case 0:
        case 1:
            brickColor = redBrick;
            break;
        case 2:
        case 3:
            brickColor = orangeBrick;
            break;
        case 4:
        case 5:
            brickColor = greenBrick;
            break;
        case 6:
        case 7:
            brickColor = yellowBrick;
            break;
        default:
            break;
        }

        for (int i(0); i < 12; i++) {
            PhysicsRectangle& newBrick = bricks.Create();
            newBrick.setSize(Vector2f(42.9167, 20));
            newBrick.setFillColor(brickColor);
            newBrick.setCenter(Vector2f((15 + (42.9167 / 2)) + (i * 42.9167) + (i * starterX), starterY));
            newBrick.setStatic(true);
            world.AddPhysicsBody(newBrick);
            newBrick.onCollision = [&ball, &window, &world, &newBrick, &bricks, &score](PhysicsBodyCollisionResult res) {
                if (res.object1 == newBrick && res.object2 == ball) {
                    world.RemovePhysicsBody(newBrick);
                    bricks.QueueRemove(newBrick);
                    score += 10;
                }
                };
        }
    }
    
    Font gameFont;
    gameFont.loadFromFile("Rubic.ttf");
    
    Text livesText;
    livesText.setFont(gameFont);
    livesText.setCharacterSize(characterSz);
    livesText.setString(to_string(lives));

    Text scoreText;
    scoreText.setFont(gameFont);
    scoreText.setCharacterSize(characterSz);
    scoreText.setString(to_string(score));


    vector<PhysicsRectangle> rects = vector<PhysicsRectangle> { leftWall, rightWall, ceiling, rightBar, leftBar, floor };

    Clock clock;
    Time lastTime(clock.getElapsedTime());

    while (true) {
        Time currentTime(clock.getElapsedTime());
        int ellapsedMS((currentTime - lastTime).asMilliseconds());

        if (ellapsedMS > 3) {
            lastTime = currentTime;
            world.UpdatePhysics(ellapsedMS);
            movePaddle(paddle, ellapsedMS, pixelConstant);
            

            window.clear();
            window.draw(paddle);
            displayStaticRectangles(rects, window);
            for (PhysicsRectangle &brick : bricks) {
                window.draw(brick);
            }
            scoreText.setString(to_string(score));
            livesText.setString(to_string(lives));
            FloatRect sts(scoreText.getGlobalBounds());
            FloatRect lts(livesText.getGlobalBounds());
            scoreText.setPosition(Vector2f(450 - (sts.width / 2), 100 - (sts.height / 2)));
            livesText.setPosition(Vector2f(150 - (lts.width / 2), 100 - (lts.height / 2)));
            window.draw(scoreText);
            window.draw(livesText);

            if (isPlaying) {
                window.draw(ball);
            } else if (!isPlaying && hasSeenStartingScreen) {
                wait(1);
                clock.restart();
                lastTime = lastTime.Zero;
                dropBallIn(ball, world, isPlaying);
            }

            if (!hasSeenStartingScreen) {
                showStartingScreen(window, gameFont);
                hasSeenStartingScreen = true;
                dropBallIn(ball, world, isPlaying);
                clock.restart();
                
            }

            window.display(); //DISPLAYING CHANGES
            bricks.DoRemovals();
        }

    }
}

