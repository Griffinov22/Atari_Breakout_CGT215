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