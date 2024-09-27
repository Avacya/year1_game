
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

Vector2f ballVelocity;
const float velocityMultiplier = 1.1f;
bool isPlayer1Serving = false;
const float initialVelocityX = 100.f;
const float initialVelocityY = 60.f;

const Keyboard::Key controls[4] = {
	Keyboard::A,
	Keyboard::Z,
	Keyboard::Up,
	Keyboard::Down,
};
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;

CircleShape ball;
RectangleShape paddles[2];

void Reset() {

	paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
	paddles[1].setPosition(Vector2f(gameWidth - (paddleSize.x + paddleOffsetWall) / 2.f, gameHeight / 2.f));

	ball.setPosition(Vector2f(gameWidth / 2, gameHeight / 2));

}

void Load() {
	
	for (auto&p : paddles) {
		p.setSize(paddleSize);
		p.setOrigin(paddleSize / 2.f);
	}

	ball.setRadius(ballRadius);
	ball.setOrigin(ballRadius, ballRadius);

	Reset();

	ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
}

void Update(RenderWindow &window) {
	
	static Clock clock;
	float dt = clock.restart().asSeconds();

	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}
	
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}

	float direction = 0.0f;
	if (Keyboard::isKeyPressed(controls[0])) {
		direction--;
	}
	if (Keyboard::isKeyPressed(controls[1])) {
		direction++;
	}
	paddles[0].move(Vector2f(0.f, direction * paddleSpeed * dt));
	float direction2 = 0.0f;
	if (Keyboard::isKeyPressed(controls[2])) {
		direction2--;
	}
	if (Keyboard::isKeyPressed(controls[3])) {
		direction2++;
	}
	paddles[1].move(Vector2f(0.f, direction2 * paddleSpeed * dt));

	ball.move(ballVelocity * dt);

	const float bx = ball.getPosition().x;
	const float by = ball.getPosition().y;
	if (by > gameHeight) {
		ballVelocity.x *= velocityMultiplier;
		ballVelocity.y *= -velocityMultiplier;
		ball.move(Vector2f(0.f, -10.f));
	}
	else if (by < 0) {
		ballVelocity.x *= velocityMultiplier;
		ballVelocity.y *= -velocityMultiplier;
		ball.move(Vector2f(0.f, 10.f));
	}
	else if (bx > gameWidth) {
		Reset();
	}
	else if (bx < 0) {
		Reset();
	}
	else if (
		bx < paddleSize.x + paddleOffsetWall &&
		by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
		by < paddles[0].getPosition().y + (paddleSize.y * 0.5))
	{
		ballVelocity.x *= -velocityMultiplier;
		ballVelocity.y *= velocityMultiplier;
		ball.move(Vector2f(10.f, 0.f));
	}
	else if (
		bx > gameWidth - (paddleSize.x + paddleOffsetWall) &&
		by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
		by < paddles[1].getPosition().y + (paddleSize.y * 0.5))
	{
		ballVelocity.x *= -velocityMultiplier;
		ballVelocity.y *= velocityMultiplier;
		ball.move(Vector2f(-10.f, 0.f));
	}


}

void Render(RenderWindow &window) {
	
	window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
}

int main() {
	RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
	Reset();
	Load();
	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;
}


