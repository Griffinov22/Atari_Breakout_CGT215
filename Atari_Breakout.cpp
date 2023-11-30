// Atari_Breakout.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <SFML/Audio.hpp>
#include "Atari_Functions.h"

using namespace std;
using namespace sf;
using namespace sfp;


//options
const float pixelConstant(0.40);
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
    bool hasAppliedBoost(false);
    bool hasWon(false);
    bool isFirstLevel(true);
    srand(time(0));
    Font gameFont;
    gameFont.loadFromFile("Rubic.ttf");

    //music
    SoundBuffer countDownBuff;
    loadMusic(countDownBuff, "./music/countDown.ogg");
    Sound countDownMusic;
    countDownMusic.setBuffer(countDownBuff);

    SoundBuffer ballFailbuff;
    loadMusic(ballFailbuff, "./music/ballFail.ogg");
    Sound ballFailMusic;
    ballFailMusic.setBuffer(ballFailbuff);

    SoundBuffer endGameBuff;
    loadMusic(endGameBuff, "./music/endGame.ogg");
    Sound endGameMusic;
    endGameMusic.setBuffer(endGameBuff);
    
    SoundBuffer thudBuffer;
    loadMusic(thudBuffer, "./music/plink.ogg");
    Sound thudSound;
    thudSound.setBuffer(thudBuffer);

    SoundBuffer nextLevelBuffer;
    loadMusic(nextLevelBuffer, "./music/completeLevel.ogg");
    Sound nextLevelSound;
    nextLevelSound.setBuffer(nextLevelBuffer);

    SoundBuffer heavenBuffer;
    loadMusic(heavenBuffer, "./music/heaven-sound.ogg");
    Sound heavenSound;
    heavenSound.setBuffer(heavenBuffer);

    //ball
    PhysicsRectangle ball;
    ball.setSize(Vector2f(10, 10));
    ball.onCollision = [&thudSound, &ball](PhysicsBodyCollisionResult res) {
        if (ball.getCenter().y < 800) {
            thudSound.stop();
            thudSound.play();
        }
    };
    //walls
    PhysicsRectangle leftWall;
    leftWall.setSize(Vector2f(10, 750)); //start 75 - 825
    leftWall.setCenter(Vector2f(5, 450));
    leftWall.setStatic(true);
    world.AddPhysicsBody(leftWall);
    //right
    PhysicsRectangle rightWall;
    rightWall.setSize(Vector2f(10, 750));
    rightWall.setCenter(Vector2f(595, 450));
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
    paddle.setSize(Vector2f(75, 15));
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
                ball.applyImpulse(Vector2f(-0.12,0));
            }
            else if (ballPos.x > ((padSz.left + (padSz.width * 0.6)))) {
                ball.applyImpulse(Vector2f(0.12,0));
            }
        }
        };

    //floor
    PhysicsRectangle floor;
    floor.setSize(Vector2f(600, 50));
    floor.setCenter(Vector2f(300, 850));
    floor.setStatic(true);
    world.AddPhysicsBody(floor);
    floor.onCollision = [&lives, &isPlaying, &ball, &world, &ballFailMusic, &paddle](PhysicsBodyCollisionResult res) {
        if (res.object2 == ball) {
            paddle.setCenter(Vector2f(300, 725));
            ballFailMusic.play();
            isPlaying = false;
            lives--;
            world.RemovePhysicsBody(ball);

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
    fillBrickList(bricks, redBrick, orangeBrick, greenBrick, yellowBrick, ball, window, world, hasAppliedBoost, score);
    
    
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

    while (lives > 0) {
        Time currentTime(clock.getElapsedTime());
        int ellapsedMS((currentTime - lastTime).asMilliseconds());

        if (ellapsedMS > 4) {
            lastTime = currentTime;
            world.UpdatePhysics(ellapsedMS);
            movePaddle(paddle, ellapsedMS, pixelConstant, hasAppliedBoost);
            

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
            }

            if (!hasSeenStartingScreen) {
                hasSeenStartingScreen = true;
                showStartingScreen(window, gameFont, countDownMusic);
                dropBallIn(ball, world, isPlaying, hasAppliedBoost);
                clock.restart();
                
            }


            window.display(); //DISPLAYING CHANGES
            bricks.DoRemovals();
            //check if PhysicsShapeList has length
            if (bricks.Count() >= 0) {
                
                if (isFirstLevel == true) {
                    isFirstLevel = false;

                    paddle.setCenter(Vector2f(300, 725));
                    fillBrickList(bricks, redBrick, orangeBrick, greenBrick, yellowBrick, ball, window, world, hasAppliedBoost, score);
                    showSecondLevelScreen(window, gameFont, redBrick, orangeBrick, greenBrick, yellowBrick, nextLevelSound, heavenSound, lives);
                    
                    isPlaying = false;
                    world.RemovePhysicsBody(ball);
                    
                } else {
                    //has gone through two levels of bricks
                    hasWon = true;
                    break;
                }

            }


            if (!isPlaying && hasSeenStartingScreen) {
                wait(2);
                clock.restart();
                lastTime = lastTime.Zero;
                dropBallIn(ball, world, isPlaying, hasAppliedBoost);
            }
        }

    }

    window.clear();
    showEndingScreen(window, gameFont, score, endGameMusic, heavenSound, hasWon, yellowBrick);

    while (true) {
        if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Enter)) {
            break;
        }
    }
}

