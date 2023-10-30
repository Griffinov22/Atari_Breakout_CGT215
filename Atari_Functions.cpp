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

void showStartingScreen(RenderWindow& window, Font font) {
	PhysicsRectangle backboard;
	backboard.setSize(Vector2f(400, 200));
	backboard.setCenter(Vector2f(300, 400));
	backboard.setFillColor(Color(250, 200, 60));
	backboard.setStatic(true);

	Text welcomeText;
	welcomeText.setFont(font);
	welcomeText.setCharacterSize(20);
	welcomeText.setString("Welcome to Breakout.\nPress space\nto start.");
	FloatRect welSz(welcomeText.getGlobalBounds());
	welcomeText.setPosition(300 - (welSz.width / 2), 340 - (welSz.height / 2));

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
		if (hasClickedSpace) {
			currentTime = clock.getElapsedTime().asMilliseconds();
		}
		int ellapsedSeconds(currentTime - lastTime);

		window.clear();
		window.draw(backboard);
		window.draw(welcomeText);
		countDownText.setString(to_string(countDown));
		if (hasClickedSpace) {
			window.draw(countDownText);
		}
		window.display();
		if (!hasClickedSpace) {
			while (!Keyboard::isKeyPressed(Keyboard::Space));
			hasClickedSpace = true;
		}

		if (ellapsedSeconds >= 1000) {
			countDown--;
			lastTime = currentTime;
			if (countDown == 0) {
				window.clear();
				break;
			}
		}
	}
}