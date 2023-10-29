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

void loadImage(Texture &tex, string path) {
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

void getUserInput(PhysicsRectangle paddle, int ellapsedMS, int pixelConstant) {
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		Vector2f oldCenter(paddle.getCenter());
		paddle.setCenter(oldCenter.x + (ellapsedMS + pixelConstant), oldCenter.y);
	}

}d