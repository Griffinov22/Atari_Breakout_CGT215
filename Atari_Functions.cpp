//these are functions used for my Breakout game
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;


void shout() {
	cout << "HELLO WORLD" << endl;
}

void loadTex(Texture &tex, string path) {
	if (!tex.loadFromFile(path)) {
		cout << "Could not load " << path;
		exit(1);
	}
}

void displayStaticRectangles(vector<PhysicsRectangle> sprites, RenderWindow &window) {
	for (PhysicsRectangle pr : sprites) {
		window.draw(pr);
	}
}

void movePaddle(PhysicsRectangle &paddle, int ellapsedMS, float pixelConstant) {
	
	Vector2f currPos(paddle.getCenter());

	if (currPos.x < 37.5) {
		paddle.setCenter(Vector2f(37.5, currPos.y));
	} else if (currPos.x > 562.5) {
		paddle.setCenter(Vector2f(562.5, currPos.y));
	}
	else {
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			Vector2f paddleLoc(paddle.getCenter());
			paddleLoc.x = paddleLoc.x + (ellapsedMS * pixelConstant);
			paddle.setCenter(paddleLoc);
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			Vector2f paddleLoc(paddle.getCenter());
			paddleLoc.x = paddleLoc.x - (ellapsedMS * pixelConstant);
			paddle.setCenter(paddleLoc);
		}
	}
}

void wait(int duration) {
	Clock clock;
	Time lastTime(clock.getElapsedTime());
	while (true) {
		Time currentTime(clock.getElapsedTime());
		int ellapsedMS((currentTime - lastTime).asMilliseconds());
		if (ellapsedMS >= (duration * 1000)) {
			break;
		}
	}
}

void dropBallIn(PhysicsRectangle& ball, World& world, bool &isPlaying) {
	ball.setCenter(Vector2f(300, 420));
	ball.setVelocity(Vector2f(0.2, 0.2));
	world.AddPhysicsBody(ball);
	isPlaying = true;
}


void showStartingScreen(RenderWindow& window, Font font) {
	RectangleShape backboard;
	backboard.setSize(Vector2f(400, 200));
	backboard.setPosition(Vector2f(100, 300));
	backboard.setFillColor(Color(251, 128, 20));

	Text welcomeText;
	welcomeText.setFont(font);
	welcomeText.setCharacterSize(20);
	welcomeText.setString(" Welcome to Breakout\n\nPress space to start:");
	FloatRect welSz(welcomeText.getGlobalBounds());
	welcomeText.setPosition(300 - (welSz.width / 2), 360 - (welSz.height / 2));

	Text countDownText;
	int countDown(3);
	countDownText.setFont(font);
	countDownText.setString(to_string(countDown));
	FloatRect cdtSz(countDownText.getGlobalBounds());
	countDownText.setPosition(300 - (cdtSz.width / 2), 420 - (cdtSz.height / 2));


	Clock clock;
	int lastTime(0);
	int currentTime(0);
	bool hasClickedSpace(false);

	while (true) {
		int ellapsedSeconds(currentTime - lastTime);
		window.clear();
		window.draw(backboard);
		window.draw(welcomeText);

		if (hasClickedSpace) {
			currentTime = clock.getElapsedTime().asMilliseconds();
			countDownText.setString(to_string(countDown));
			window.draw(countDownText);
		}
		window.display();
		
		if (!hasClickedSpace) {
			while (!Keyboard::isKeyPressed(Keyboard::Space));
			hasClickedSpace = true;
			clock.restart(); //allows currentTime to be accurate
		}


		if (ellapsedSeconds >= 1000) {
			countDown--;
			lastTime = currentTime;
			if (countDown == 0) {
				break;
			}
		}
	}
}

void showEndingScreen(RenderWindow& window, Font font, int score) {
	RectangleShape backboard;
	backboard.setSize(Vector2f(400, 200));
	backboard.setPosition(Vector2f(100, 300));
	backboard.setFillColor(Color(34, 32, 246));

	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(30);
	gameOverText.setString("Game Over!!");
	FloatRect goSz(gameOverText.getGlobalBounds());
	gameOverText.setPosition(300 - (goSz.width / 2), 340 - (goSz.height / 2));

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setString("Your score: " + to_string(score));
	FloatRect stSz(scoreText.getGlobalBounds());
	scoreText.setPosition(300 - (stSz.width / 2), 420 - (stSz.height / 2));

	window.draw(backboard);
	window.draw(gameOverText);
	window.draw(scoreText);
	window.display();
}