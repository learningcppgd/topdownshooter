#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>


class Map
{
private:
	Texture2D sprite;
	const static int LEVEL_WIDTH = 20;
	const static int LEVEL_HEIGHT = 15;
	const int TILE_SIZE = 64;

	int level[LEVEL_HEIGHT][LEVEL_WIDTH] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

public:
	Map();
	void draw();
	bool checkCollision(Rectangle rect);
};

Map::Map()
{
	sprite = LoadTexture("assets/Spritesheet/spritesheet_tiles.png");
}

void Map::draw()
{

	for (int y{ 0 }; y < LEVEL_HEIGHT; y++)
	{
		for (int x{ 0 }; x < LEVEL_WIDTH; x++)
		{
			Rectangle tileRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
			if (level[y][x] == 1)
				DrawRectangleRec(tileRect, GRAY); // wall
			else
			{
				Rectangle source{ 0, 0, 64, 64 };
				Rectangle dest{ x * TILE_SIZE, y * TILE_SIZE, 64, 64 };
				Vector2 origin{ 0, 0 };
				float rotation{ 0 };
				DrawTexturePro(sprite, source, dest, origin, rotation, WHITE);
			}
		}
	}
}

bool Map::checkCollision(Rectangle rect)
{
	for (int y{ 0 }; y < LEVEL_HEIGHT; y++)
	{
		for (int x{ 0 }; x < LEVEL_WIDTH; x++)
		{
			if (level[y][x] == 1)
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
	Vector2 getPlayerCenter() { return position; }
	Vector2 getRotatedOffset() { return rotatedOffset; }
	Rectangle getPlayerHitbox() { return Rectangle{ position.x - sprite.width / 2.0f, position.y - sprite.height / 2.0f, (float)sprite.width, (float)sprite.height }; }
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

	if (!map.checkCollision(hitboxX))
		position.x = newX;

	float newY = position.y + direction.y * speed * GetFrameTime();
	Rectangle hitboxY = {
		position.x - sprite.width / 2.0f,
		newY - sprite.height / 2.0f,
		(float)sprite.width,
		(float)sprite.height
	};

	if (!map.checkCollision(hitboxY))
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


class Bullet {
private:
	Vector2 position;
	Vector2 velocity;
	float radius{ 5.0f };
	float speed{ 600.0f };

public:
	Bullet(Vector2 pos, Vector2 direction);

	void draw();
	void update();
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


int main()
{	
	InitWindow(700, 700, "idk");

	Player player{ Vector2 {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f} };
	Map map;
	
	std::vector<Bullet> bullets;

	Camera2D camera{};
	camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 0.5f;

	while (!WindowShouldClose())
	{
		//update
		camera.target = player.getPlayerCenter();
		player.update(camera, map);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Vector2 start = player.getGunPoint();
			Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
			Vector2 dir = Vector2Subtract(mouseWorld, start);
			bullets.push_back(Bullet(start, dir));
		}

		for (Bullet& bullet : bullets)
		{
			bullet.update();
		}

		//draw
		BeginDrawing();
		{
			ClearBackground(DARKGRAY);
			BeginMode2D(camera);
			{
				map.draw();
				player.draw();
				for (Bullet& bullet : bullets)
				{
					bullet.draw();
				}
			}
			EndMode2D();
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}