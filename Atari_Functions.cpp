//these are functions used for my Breakout game
#include <iostream>
#include <string>
#include <cstdlib> 
#include <time.h> 
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <SFML/Audio.hpp>

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

void loadMusic(SoundBuffer& buff, string path) {
	if (!buff.loadFromFile(path)) {
		cout << "music file not found at " << path << endl;
		exit(1);
	}
}

void displayStaticRectangles(vector<PhysicsRectangle> sprites, RenderWindow &window) {
	for (PhysicsRectangle pr : sprites) {
		window.draw(pr);
	}
}

void movePaddle(PhysicsRectangle &paddle, int ellapsedMS, float pixelConstant, bool &hasAppliedBoost) {
	
	Vector2f currPos(paddle.getCenter());

	//if play hits the top layer of red bricks a boost is applied to the ball. To supplement faster ball -> faster paddle movement
	pixelConstant = hasAppliedBoost ? pixelConstant *= 1.05 : pixelConstant;

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

void dropBallIn(PhysicsRectangle& ball, World& world, bool &isPlaying, bool &hasAppliedBoost) {

	int randBool = rand() % 2;

	ball.setCenter(Vector2f(300, 420));
	ball.setVelocity(Vector2f(randBool ? 0.11 : -0.11, 0.25));
	world.AddPhysicsBody(ball);
	isPlaying = true;
	hasAppliedBoost = false;
}


void showStartingScreen(RenderWindow& window, Font font, Sound &countDownMusic) {
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
			countDownMusic.play();
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
/// <summary>
/// fills a the PhysicsShapeList<PhysicsRectangle> called 'bricks' with bricks and applies onCollision listener to each brick
/// </summary>
/// <param name="bricks"></param>
/// <param name="redBrick"></param>
/// <param name="orangeBrick"></param>
/// <param name="greenBrick"></param>
/// <param name="yellowBrick"></param>
/// <param name="ball"></param>
/// <param name="window"></param>
/// <param name="world"></param>
/// <param name="hasAppliedBoost"></param>
/// <param name="score"></param>
void fillBrickList(PhysicsShapeList<PhysicsRectangle>& bricks,
	Color redBrick, Color orangeBrick, Color greenBrick, Color yellowBrick, 
	PhysicsRectangle& ball, RenderWindow& window, World& world, 
	bool& hasAppliedBoost, int& score) {

	//should be 8 rows in production
	Color brickColor;
	for (int j(0); j < 8; j++) {
		int starterX(5);
		int starterY(170 + (j * 28));
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
			newBrick.onCollision = [&ball, &window, &world, &newBrick, &bricks, &score, &hasAppliedBoost, j, i, yellowBrick, greenBrick, orangeBrick, redBrick](PhysicsBodyCollisionResult res) {
				Color nbc(newBrick.getFillColor());
				if (res.object2 == ball) {
					world.RemovePhysicsBody(newBrick);
					bricks.QueueRemove(newBrick);

					if (nbc == yellowBrick) {
						score += 10;
					}
					else if (nbc == greenBrick) {
						score += 30;
					}
					else if (nbc == orangeBrick) {
						score += 50;
					}
					else if (nbc == redBrick) {
						score += 70;
					}
				}
				if (j == 0 && !hasAppliedBoost && bricks.Count() >= 1) {
					//dont apply impulse to next level, so the brick list will have 1 or 0 left in the list
					//makes ball go faster when contact with upper level of red bricks!
					hasAppliedBoost = true;

					bool isPositive = ball.getVelocity().y <= 0;
					ball.applyImpulse(Vector2f(0, (isPositive ? -0.12 : 0.12)));
				}

				};
		}
	}
}

bool isSameColor(Color first, Color second) {
	if ((int)first.r != (int)second.r)
		return false;
	if ((int)first.g != (int)second.g)
		return false;
	if ((int)first.b != (int)second.b)
		return false;
	return true;
}

void showEasterEgg(RenderWindow& window, Font font, int &lives, Sound &heavenSound) {
	//Gotta have a good ole' easter egg
	lives++;

	Texture griffTex;
	//400 x 650
	loadTex(griffTex, "./images/pixelated-griffin.png");

	Image griffImage;
	griffImage = griffTex.copyToImage();
	Vector2u sz = griffImage.getSize();
	Sprite sprite;
	sprite.setPosition(300 - (sz.x / 2), 400 - (sz.y / 2));

	//turn off opacity of image
	for (int y = 0; y < sz.y; y++) {
		for (int x = 0; x < sz.x; x++) {
			Color c = griffImage.getPixel(x, y);
				griffImage.setPixel(x, y, Color(c.r, c.g, c.b, 0));
		}
	}

	Text griffText;
	griffText.setFont(font);
	griffText.setString("GRIFFIN");
	FloatRect txtSz = griffText.getGlobalBounds();
	griffText.setPosition(Vector2f(300 - (txtSz.width / 2), 700 - (txtSz.height / 2)));

	Clock clock;
	Time lastTime(clock.getElapsedTime());
	int hiddenHeight(sz.y - 50);

	while (true) {
		Time currentTime(clock.getElapsedTime());
		int ellapsedMS((currentTime - lastTime).asMilliseconds());

		if (ellapsedMS >= 500) {
			hiddenHeight -= 50;
			if (hiddenHeight < 0) {
				wait(3);
				break;
			}
			//moving text position after 2 rounds
			if (hiddenHeight <= 500) {
				Vector2f pos = griffText.getPosition();
				if (hiddenHeight == 500) {
					griffText.setPosition(100, pos.y);
					//only start playing sound till 2nd round
					heavenSound.play();
					
				}
				else if (hiddenHeight == 0) {
					FloatRect gb = griffText.getGlobalBounds();
					griffText.setPosition(Vector2f(300 - (gb.width / 2), 700 - (gb.height / 2)));
				}
				else {

					float w = griffText.getGlobalBounds().width;
					griffText.setPosition(pos.x == 100 ? (500 - w) : 100, pos.y);
				}
			}

			lastTime = currentTime;
			window.clear();

			for (int y = hiddenHeight; y < hiddenHeight + 50; y++) {
				for (int x = 0; x < sz.x; x++) {
					Color c = griffImage.getPixel(x, y);
					griffImage.setPixel(x, y, Color(c.r, c.g, c.b, 255));

				}
			}
			griffTex.loadFromImage(griffImage);

			sprite.setTexture(griffTex);


			window.draw(sprite);
			window.draw(griffText);
			window.display();

		}

	}
}

void showSecondLevelScreen(RenderWindow &window, Font font, Color redBrick, Color orangeBrick, Color greenBrick, Color yellowBrick, Sound &nextLevelSound, Sound &heavenSound, int &lives) {

	nextLevelSound.play();
	
	RectangleShape backboard;
	backboard.setSize(Vector2f(400, 200));
	backboard.setPosition(Vector2f(100, 300));

	Text secondLevelText;
	secondLevelText.setFont(font);
	secondLevelText.setCharacterSize(48);
	//secondLevelText.setFillColor(Color(0, 0, 0));
	secondLevelText.setString("Level 2");
	FloatRect slt(secondLevelText.getGlobalBounds());
	secondLevelText.setPosition(300 - (slt.width / 2), 400 - (slt.height / 2));


	Clock clock;
	srand(time(0));
	vector<Color> colors = { redBrick, yellowBrick, orangeBrick, greenBrick };
	Color oldColor(redBrick);
	Color newColor(redBrick);
	Time lastTime(clock.getElapsedTime());
	int numOfChanges(0);

	while (true) {
		// 1s = 1000ms
		if (nextLevelSound.getStatus() == SoundSource::Stopped) {
			// only play twice
			nextLevelSound.play();
		}
		Time currentTime = clock.getElapsedTime();
		int ellapsedSeconds((currentTime - lastTime).asMilliseconds());
		if (ellapsedSeconds >= 200) {
			if (++numOfChanges == 25) {
				break;
			}
			while (isSameColor(newColor, oldColor)) {
				int rand4 = rand() % 4;
				newColor = colors[rand4];
			}
			backboard.setFillColor(newColor);
			oldColor = newColor;
			lastTime = currentTime;
		}
		//if holding shift+G => produce easter egg
		if ((Keyboard::isKeyPressed(Keyboard::LShift) || Keyboard::isKeyPressed(Keyboard::LShift)) && Keyboard::isKeyPressed(Keyboard::G)) {
			nextLevelSound.stop();
			showEasterEgg(window, font, lives, heavenSound);
			numOfChanges = 0; //restarts 2nd level screen
		}

		window.clear();
		window.draw(backboard);
		window.draw(secondLevelText);
		window.display();
	}
}

void showEndingScreen(RenderWindow& window, Font font, int score, Sound &endGameMusic, Sound &heavenSound, bool hasWon, Color yellowBrick) {
	//play music

	RectangleShape backboard;
	backboard.setSize(Vector2f(400, 200));
	backboard.setPosition(Vector2f(100, 300));
	if (hasWon) {
		backboard.setFillColor(yellowBrick);
		heavenSound.play();
	}
	else {
		backboard.setFillColor(Color(34, 32, 246));
		endGameMusic.play();
	}

	Text gameOverText;
	gameOverText.setFont(font);
	if (hasWon) {
		gameOverText.setCharacterSize(36);
		gameOverText.setString("You Won!!");
	}
	else {
		gameOverText.setCharacterSize(30);
		gameOverText.setString("Game Over!!");

	}
	FloatRect goSz(gameOverText.getGlobalBounds());
	gameOverText.setPosition(300 - (goSz.width / 2), 340 - (goSz.height / 2));

	Text ysText;
	ysText.setFont(font);
	ysText.setString("Your score:");
	FloatRect ysSz(ysText.getGlobalBounds());
	ysText.setPosition(300 - (ysSz.width / 2), 410 - (ysSz.height / 2));

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setString(to_string(score));
	FloatRect stSz(scoreText.getGlobalBounds());
	scoreText.setPosition(300 - (stSz.width / 2), 455 - (stSz.height / 2));

	window.draw(backboard);
	window.draw(gameOverText);
	window.draw(ysText);
	window.draw(scoreText);
	window.display();
}
