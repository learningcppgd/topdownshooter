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
	const static int LEVEL_WIDTH = 20;
	const static int LEVEL_HEIGHT = 15;
	const int TILE_SIZE = 64;

	enum GroundType {
		GRASS = 0,
		WALL = 1
	};

	std::array<std::array<int, LEVEL_WIDTH>, LEVEL_HEIGHT> level = { {
	{{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1}},
	{{1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1}},
	{{1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1}},
	{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}
	}};

public:
	Map();
	~Map();
	void draw();
	bool checkCollisionWallPlayer(Rectangle rect);

	const std::array<std::array<int, LEVEL_WIDTH>, LEVEL_HEIGHT> getLevel()& { return level; }
	int getLevelWidth() const { return LEVEL_WIDTH; }
	int getLevelHeight() const { return LEVEL_HEIGHT; }
	int getTileSize() const { return TILE_SIZE; }
};

Map::Map()
{
	sprite = LoadTexture("assets/Spritesheet/spritesheet_tiles.png");
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
			Rectangle tileRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
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

bool Map::checkCollisionWallPlayer(Rectangle rect)
{
	for (int y{ 0 }; y < LEVEL_HEIGHT; y++)
	{
		for (int x{ 0 }; x < LEVEL_WIDTH; x++)
		{
			if (level[y][x] == WALL)
			{
				Rectangle wallRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
				if (CheckCollisionRecs(wallRect, rect)) return true;
			}
		}
	}
	return false;
}


class Player {
private:
	Vector2 position;
	float speed{300.0f};
	Texture2D sprite;
	float rotation;
	Vector2 rotatedOffset;

public:

	Player(Vector2 pos);
	~Player();

	void draw();
	void update(Camera2D camera, Map& map);

	float calculateAngle(Camera2D camera);
	Vector2 getGunPoint();

	//geter
	Vector2 getPlayerCenter() const { return position; }
	Vector2 getRotatedOffset() const { return rotatedOffset; }
	Rectangle getPlayerHitbox() const { return Rectangle{ position.x - sprite.width / 2.0f, position.y - sprite.height / 2.0f, (float)sprite.width, (float)sprite.height }; }
};

Player::Player(Vector2 pos)
	:position(pos)
{
	sprite = LoadTexture("assets/PNG/Hitman 1/hitman1_gun.png");
}

Player::~Player()
{
	UnloadTexture(sprite);
}

void Player::draw()
{
	Rectangle source{0, 0, sprite.width, sprite.height};
	Rectangle dest{position.x, position.y, sprite.width, sprite.height};
	Vector2 origin{sprite.width / 2.0f, sprite.height / 2.0f };
	DrawTexturePro(sprite, source, dest, origin, rotation, WHITE);
	DrawRectangleLinesEx(getPlayerHitbox(), 1, RED);
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
}

float Player::calculateAngle(Camera2D camera)
{
	Vector2 mouseposition_toworld = GetScreenToWorld2D(GetMousePosition(), camera);
	Vector2 dir = Vector2{ mouseposition_toworld.x - getPlayerCenter().x, mouseposition_toworld.y - getPlayerCenter().y };
	float rotation = atan2f(dir.y, dir.x) * RAD2DEG;
	return rotation;
}

class Enemy
{
private:
	Vector2 position;
	float speed{ 250.0f };
	Texture2D sprite;
	float rotation;
	bool alive{ true };

public:
	Enemy(Vector2 position);
	~Enemy();
	void draw();
	void update(const Player& player);
	float calculateAngle(const Player& player);

	Rectangle getRect() { return Rectangle{
		position.x - sprite.width / 2.0f, 
		position.y - sprite.height / 2.0f,
		(float)sprite.width,
		(float)sprite.height};
	}
	bool isAlive() { return alive; }
	void kill() { alive = false; }
};

Enemy::Enemy(Vector2 pos)
	: position(pos)
{
	sprite = LoadTexture("assets/PNG/Zombie 1/zoimbie1_stand.png");
}

Enemy::~Enemy()
{
	UnloadTexture(sprite);
}

void Enemy::draw()
{
	Rectangle source{ 0, 0, sprite.width, sprite.height };
	Rectangle dest{ position.x, position.y, sprite.width, sprite.height };
	Vector2 origin{ sprite.width / 2.0f, sprite.height / 2.0f };
	DrawTexturePro(sprite, source, dest, origin, rotation, WHITE);
	//draw rectangle around enemy
	Rectangle enemy_rect{ position.x - sprite.width / 2.0f, position.y - sprite.height / 2.0f, sprite.width, sprite.height };
	DrawRectangleLinesEx(enemy_rect, 1, WHITE);
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

class Bullet {
private:
	Vector2 position;
	Vector2 velocity;
	float radius{ 5.0f };
	float speed{ 800.0f };

public:
	Bullet(Vector2 pos, Vector2 direction);

	void draw();
	void update();
	bool checkCollisionMap(Map& map);
	bool checkCollisionEnemy(Enemy& enemy);

	Rectangle getBulletRect() { return Rectangle{ position.x, position.y, radius, radius }; }
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
				Rectangle wallRect = { x * map.getTileSize(), y * map.getTileSize(), map.getTileSize(), map.getTileSize()};
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
		enemy.kill();
		return true;
	}
	return false;
}

int main()
{	
	InitWindow(700, 700, "idk");

	Map map;
	Player player{ Vector2 {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f} };

	std::vector<Bullet*> bullets;
	std::vector<Enemy*> enemies;

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
			Enemy* enemy = new Enemy{ Vector2 {
				(float)GetRandomValue(0, GetScreenWidth()),
				(float)GetRandomValue(0, GetScreenHeight())
			}};
			enemies.push_back(enemy);
			counter = 0;
		}
		counter += GetFrameTime();

		for (Enemy* enemy : enemies)
		{
			if (enemy->isAlive())
			{
				//same as ->, *enemy gets the value of enemy
				(*enemy).update(player);
			}
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Vector2 start = player.getGunPoint();
			Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
			Vector2 dir = Vector2Subtract(mouseWorld, start);
			bullets.push_back(new Bullet(start, dir));
		}

		for (int i{ 0 }; i < bullets.size();)
		{
			Bullet* bullet = bullets[i];
			bullet->update();

			bool removeBullet = false;

			if (bullet->checkCollisionMap(map))
			{
				removeBullet = true;
			}
			else
			{
				for (Enemy* enemy : enemies)
				{
					if (bullet->checkCollisionEnemy(*enemy))
					{
						removeBullet = true;
						break;
					}
				}
			}
			if (removeBullet)
			{
				delete bullet;
				bullets.erase(bullets.begin() + i);
			}
			else
			{
				++i;
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
				for (Enemy* enemy : enemies)
				{
					if (enemy->isAlive())
					{
						enemy->draw();
					}
				}

				for (Bullet* bullet : bullets)
				{
					(*bullet).draw(); // same as bullet->draw()
				}
			}
			EndMode2D();
		}
		EndDrawing();
	}
	for (Bullet* bullet : bullets)
	{
		delete bullet;
	}

	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	CloseWindow();
	return 0;
}