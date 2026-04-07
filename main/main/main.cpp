#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>
#include <array>

//t → copy
//t& → real object, can modify
//const t& → real object, read - only

class Map
{
private:
	Texture2D sprite;
	const static int LEVEL_WIDTH = 40;
	const static int LEVEL_HEIGHT = 30;
	const float TILE_SIZE = 64;

	enum GroundType {
		GRASS = 0,
		WALL = 1
	};

	std::array<std::array<int, LEVEL_WIDTH>, LEVEL_HEIGHT> level = { {
	{{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1}},
	{{1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1}},
	{{1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}
	} };

public:
	Map(Texture2D spriteMap);
	~Map();
	void draw();
	bool checkCollisionWallPlayer(Rectangle rect);

	const std::array<std::array<int, LEVEL_WIDTH>, LEVEL_HEIGHT> getLevel()& { return level; }
	int getLevelWidth() const { return LEVEL_WIDTH; }
	int getLevelHeight() const { return LEVEL_HEIGHT; }
	int getTileSize() const { return TILE_SIZE; }
};

Map::Map(Texture2D spriteMap)
	: sprite(spriteMap){
}

Map::~Map()
{
	UnloadTexture(sprite);
}

void Map::draw()
{

	for (int y{ 0 }; y < LEVEL_HEIGHT; y++)
	{
		for (int x{ 0 }; x < LEVEL_WIDTH; x++)
		{
			Rectangle tileRect = { (float)x * TILE_SIZE, (float)y * TILE_SIZE, (float)TILE_SIZE, (float)TILE_SIZE };
			if (level[y][x] == WALL)
				DrawRectangleRec(tileRect, GRAY);
			else
			{
				//draws grass
				Rectangle source{ 0, 0, 64, 64 };
				Rectangle dest{ x * TILE_SIZE, y * TILE_SIZE, 64, 64 };
				Vector2 origin{ 0, 0 };
				float rotation{ 0 };
				DrawTexturePro(sprite, source, dest, origin, rotation, WHITE);
			}
		}
	}
}

bool Map::checkCollisionWallPlayer(Rectangle rect) {
	int startX = (int)(rect.x / TILE_SIZE);
	int endX = (int)((rect.x + rect.width) / TILE_SIZE);
	int startY = (int)(rect.y / TILE_SIZE);
	int endY = (int)((rect.y + rect.height) / TILE_SIZE);

	for (int y = startY; y <= endY; y++) {
		for (int x = startX; x <= endX; x++) {
			if (x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT) {
				if (level[y][x] == WALL) return true;
			}
		}
	}
	return false;
}


class Player {
private:
	Vector2 position;
	float speed{ 300.0f };
	Texture2D sprite;
	float rotation;
	Vector2 rotatedOffset;

	const int maxHp{ 100 };

public:

	Player(Vector2 pos, Texture2D spritePlayer);
	~Player();

	int hp{ maxHp };

	void draw();
	void update(Camera2D camera, Map& map);

	float calculateAngle(Camera2D camera);
	Vector2 getGunPoint();

	// zracuna kolko se more zeleni bar premaknt v levo lolotorltorlotlro
	float calculateOffsetForHp();

	//geter
	Vector2 getPlayerCenter() const { return position; }
	Vector2 getRotatedOffset() const { return rotatedOffset; }
	Rectangle getPlayerHitbox() const { return Rectangle{ position.x - sprite.width / 2.0f, position.y - sprite.height / 2.0f, (float)sprite.width, (float)sprite.height }; }
};

Player::Player(Vector2 pos, Texture2D spritePlayer)
	:position(pos), sprite(spritePlayer) {
}

Player::~Player()
{
	UnloadTexture(sprite);
}

void Player::draw()
{
	Rectangle source{ 0, 0, sprite.width, sprite.height };
	Rectangle dest{ position.x, position.y, sprite.width, sprite.height };
	Vector2 origin{ sprite.width / 2.0f, sprite.height / 2.0f };
	DrawTexturePro(sprite, source, dest, origin, rotation, WHITE);
	/*DrawRectangleLinesEx(getPlayerHitbox(), 1, RED);*/


	//draw hp
	DrawRectangleV(Vector2{ position.x - sprite.width / 2.0f , position.y - (float)sprite.height }, Vector2{ (float)sprite.width, float(10.0f) }, RED);
	DrawRectangleV(Vector2{ position.x - sprite.width / 2.0f , position.y - (float)sprite.height }, Vector2{ calculateOffsetForHp(), float(10.0f) }, GREEN);
}

Vector2 Player::getGunPoint()
{
	Vector2 center = position;
	Vector2 localOffset = { 25.0f, 8.0f };

	// rotate the offset by the player's rotation
	float angleRad = rotation * DEG2RAD;
	// rotational matrix  
	rotatedOffset.x = localOffset.x * cosf(angleRad) - localOffset.y * sinf(angleRad);
	rotatedOffset.y = localOffset.x * sinf(angleRad) + localOffset.y * cosf(angleRad);
	return Vector2Add(center, rotatedOffset);
}

void Player::update(Camera2D camera, Map& map)
{
	Vector2 direction{ 0, 0 };
	if (IsKeyDown(KEY_A)) direction.x = -1;
	if (IsKeyDown(KEY_D)) direction.x = 1;
	if (IsKeyDown(KEY_W)) direction.y = -1;
	if (IsKeyDown(KEY_S)) direction.y = 1;

	if (Vector2Length(direction) > 1)
		direction = Vector2Normalize(direction);

	float newX = position.x + direction.x * speed * GetFrameTime();
	Rectangle hitboxX = {
		newX - sprite.width / 2.0f,
		position.y - sprite.height / 2.0f,
		(float)sprite.width,
		(float)sprite.height
	};

	if (!map.checkCollisionWallPlayer(hitboxX))
		position.x = newX;

	float newY = position.y + direction.y * speed * GetFrameTime();
	Rectangle hitboxY = {
		position.x - sprite.width / 2.0f,
		newY - sprite.height / 2.0f,
		(float)sprite.width,
		(float)sprite.height
	};

	if (!map.checkCollisionWallPlayer(hitboxY))
		position.y = newY;

	rotation = calculateAngle(camera);
	calculateOffsetForHp();
}

float Player::calculateAngle(Camera2D camera)
{
	Vector2 mouseposition_toworld = GetScreenToWorld2D(GetMousePosition(), camera);
	Vector2 dir = Vector2{ mouseposition_toworld.x - getPlayerCenter().x, mouseposition_toworld.y - getPlayerCenter().y };
	float rotation = atan2f(dir.y, dir.x) * RAD2DEG;
	return rotation;
}

float Player::calculateOffsetForHp()
{
	return (float)hp * sprite.width / maxHp;
}

class Enemy
{
private:
	Vector2 position;
	float speed{ 250.0f };
	Texture2D sprite;
	float rotation;
	bool alive{ true };
	int maxHp{ 20 };

public:
	Enemy(Vector2 position, Texture2D enemySprite);

	int hp{ maxHp };

	float calculateOffsetForHp();

	void draw();
	void update(const Player& player);
	float calculateAngle(const Player& player);
	void CollisionPlayer(Player& player);
	void applyKnockback(Vector2 dir, float distance);

	Rectangle getRect() {
		return Rectangle{
			position.x - sprite.width / 2.0f,
			position.y - sprite.height / 2.0f,
			(float)sprite.width,
			(float)sprite.height };
	}
	bool isAlive() { return alive; }
	void kill() { alive = false; }
	Vector2 getPosition() const { return position; }
};

Enemy::Enemy(Vector2 pos, Texture2D enemySprite)
	: position(pos), sprite(enemySprite){
}

void Enemy::draw()
{
	Rectangle source{ 0, 0, sprite.width, sprite.height };
	Rectangle dest{ position.x, position.y, sprite.width, sprite.height };
	Vector2 origin{ sprite.width / 2.0f, sprite.height / 2.0f };
	DrawTexturePro(sprite, source, dest, origin, rotation, WHITE);
	//draw rectangle around enemy
	/*Rectangle enemy_rect{ position.x - sprite.width / 2.0f, position.y - sprite.height / 2.0f, sprite.width, sprite.height };
	DrawRectangleLinesEx(enemy_rect, 1, WHITE);*/

	//draw hp
	DrawRectangleV(Vector2{ position.x - sprite.width / 2.0f , position.y - (float)sprite.height }, Vector2{ (float)sprite.width, float(10.0f) }, RED);
	DrawRectangleV(Vector2{ position.x - sprite.width / 2.0f , position.y - (float)sprite.height }, Vector2{ calculateOffsetForHp(), float(10.0f) }, GREEN);
}

void Enemy::update(const Player& player)
{
	Vector2 dir = Vector2Normalize(Vector2Subtract(player.getPlayerCenter(), position));

	position.x += dir.x * speed * GetFrameTime();
	position.y += dir.y * speed * GetFrameTime();

	rotation = calculateAngle(player);
}

float Enemy::calculateAngle(const Player& player)
{
	Vector2 dir = Vector2{ player.getPlayerCenter().x - position.x, player.getPlayerCenter().y - position.y };
	float rotation = atan2f(dir.y, dir.x) * RAD2DEG;
	return rotation;
}

void Enemy::CollisionPlayer(Player& player)
{
	if (isAlive() && CheckCollisionRecs(player.getPlayerHitbox(), getRect()))
	{
		kill();
		player.hp -= 10;
	}
}

void Enemy::applyKnockback(Vector2 dir, float distance)
{
	if (Vector2Length(dir) > 0.0f)
	{
		dir = Vector2Normalize(dir);
		position = Vector2Add(position, Vector2Scale(dir, distance));
	}
}

float Enemy::calculateOffsetForHp()
{
	return (float)hp * sprite.width / maxHp;
}

class Bullet {
private:
	Vector2 position;
	Vector2 velocity;
	float radius{ 5.0f };
	float speed{ 800.0f };
	int damage{ 5 };
	float knockback{ 30.0f };

public:
	Bullet(Vector2 pos, Vector2 direction);

	void draw();
	void update();
	bool checkCollisionMap(Map& map);
	bool checkCollisionEnemy(Enemy& enemy);

	Rectangle getBulletRect() { return Rectangle{ position.x, position.y, radius, radius }; }
	int getDamage() const { return damage; }
	Vector2 getDirection() const { return Vector2Normalize(velocity); }
};

Bullet::Bullet(Vector2 pos, Vector2 direction)
	:position(pos)
{
	velocity = Vector2Scale(Vector2Normalize(direction), speed);
}

void Bullet::draw()
{
	DrawCircleV(position, radius, BLACK);
}

void Bullet::update()
{
	position.x += velocity.x * GetFrameTime();
	position.y += velocity.y * GetFrameTime();

}


bool Bullet::checkCollisionMap(Map& map)
{
	for (int y{ 0 }; y < map.getLevelHeight(); y++)
	{
		for (int x{ 0 }; x < map.getLevelWidth(); x++)
		{
			if (map.getLevel()[y][x] == 1)
			{
				Rectangle wallRect = { x * map.getTileSize(), y * map.getTileSize(), map.getTileSize(), map.getTileSize() };
				if (CheckCollisionRecs(wallRect, getBulletRect())) return true;
			}
		}
	}
	return false;
}

bool Bullet::checkCollisionEnemy(Enemy& enemy)
{
	if (enemy.isAlive() && CheckCollisionRecs(enemy.getRect(), getBulletRect()))
	{
		enemy.hp -= getDamage();
		enemy.applyKnockback(getDirection(), knockback);

		if (enemy.hp <= 0)
		{
			enemy.kill();
		}

		return true;
	}
	return false;
}

class Particle
{
private:
	Vector2 position;
	Vector2 velocity;
	float maxlifetime;
	float radius;

public:
	float lifetime{ maxlifetime };

	Particle(Vector2 pos, Vector2 vel);
	void draw();
	void update();

};

Particle::Particle(Vector2 pos, Vector2 vel)
	: position(pos), velocity(vel)
{
	maxlifetime = (float)GetRandomValue(5, 10) / 10.0f;
	lifetime = maxlifetime;
	radius = (float)GetRandomValue(2, 5);
}

void Particle::update()
{
	position.x += velocity.x * GetFrameTime();
	position.y += velocity.y * GetFrameTime();
	lifetime -= GetFrameTime();
}

void Particle::draw()
{
	float alpha = lifetime / maxlifetime;
	DrawCircleV(position, radius, ColorAlpha(RED, alpha));
}

int main()
{
	InitWindow(700, 700, "idk");

	Map map{ LoadTexture("assets/Spritesheet/spritesheet_tiles.png")};
	Texture2D enemy_texture{ LoadTexture("assets/PNG/Zombie 1/zoimbie1_stand.png") };
	Player player{ Vector2 {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}, LoadTexture("assets/PNG/Hitman 1/hitman1_gun.png")};

	std::vector<Bullet> bullets;
	std::vector<Enemy> enemies;
	std::vector<Particle> particles;

	Camera2D camera{};
	camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 0.5f;

	int respawnTimer = 2;
	float counter = 0;

	while (!WindowShouldClose())
	{
		//update
		camera.target = player.getPlayerCenter();
		player.update(camera, map);

		if (counter > respawnTimer)
		{
			Enemy enemy = { Vector2 {
				(float)GetRandomValue(0, GetScreenWidth()),
				0
			}, enemy_texture};
			enemies.push_back(enemy);
			counter = 0;
		}
		counter += GetFrameTime();

		for (int i{ (int)enemies.size() - 1}; i >= 0; i--)
		{
			enemies[i].CollisionPlayer(player);
			if (enemies[i].isAlive())
			{
				enemies[i].update(player);
			}
			else
			{
				enemies.erase(enemies.begin() + i);
			}
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Vector2 start = player.getGunPoint();
			Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
			Vector2 dir = Vector2Subtract(mouseWorld, start);
			bullets.push_back(Bullet(start, dir));
		}

		for (int i{ 0 }; i < bullets.size();)
		{
			Bullet& bullet = bullets[i];
			bullet.update();

			bool removeBullet = false;

			if (bullet.checkCollisionMap(map))
			{
				removeBullet = true;
			}
			else
			{
				for (Enemy& enemy : enemies)
				{
					if (bullet.checkCollisionEnemy(enemy))
					{
						for (int j{ 0 }; j < 10; j++)
						{
							Vector2 random_velocity = Vector2{ (float)GetRandomValue(-150,150), (float)GetRandomValue(-150,150) };
							Particle particle{ enemy.getPosition(), random_velocity };
							particles.push_back(particle);
						}
						removeBullet = true;
						break;
					}
				}
			}
			if (removeBullet)
			{
				bullets.erase(bullets.begin() + i);
			}
			else
			{
				++i;
			}
		}

		for (int i{ (int)particles.size() - 1 }; i >= 0; i--)
		{
			particles[i].update();
			if (particles[i].lifetime < 0)
			{
				particles.erase(particles.begin() + i);
			}
		}

		//draw
		BeginDrawing();
		{
			ClearBackground(DARKGRAY);
			BeginMode2D(camera);
			{
				map.draw();
				player.draw();
				for (Enemy enemy : enemies)
				{
					if (enemy.isAlive())
					{
						enemy.draw();
					}
				}

				for (Bullet bullet : bullets)
				{
					bullet.draw(); 
				}

				for (Particle particle : particles)
				{
					particle.draw();
				}
			}
			EndMode2D();
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}