#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_camera.h"
#include "../components/cmp_bullet.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
using namespace std;
using namespace sf;

#define TILE_SIZE 64.0f

static shared_ptr<Entity> player;
static shared_ptr<Entity> camera;

void Level1Scene::Load() {
	cout << "Scene 1 Load" << endl;
	ls::loadLevelFile("res/level_1.txt", TILE_SIZE);
	auto ho = Engine::getWindowSize().y - (ls::getHeight() * TILE_SIZE);
	//auto ho = 0;
	ls::setOffset(Vector2f(0, ho));

	// Create player
	{
		// *********************************
		player = makeEntity();
		player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		auto s = player->addComponent<ShapeComponent>();
		s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
		s->getShape().setFillColor(Color::Yellow);
		s->getShape().setOrigin(10.f, 15.f);
		// *********************************
		player->addTag("player");
		auto p = player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
		p->setMass(1000000.f);
		p->setRestitution(.1f);
		p->setFriction(.001f);
	}

	// Create camera
	{
		// *********************************
		camera = makeEntity();
		camera->addTag("camera");
		//need to change resolution for variable once settings are made
		auto c = camera->addComponent<CameraComponent>(Vector2f(1280.0f, 720.0f), Vector2f(0.0f, 0.0f));
		c->setTarget(player);
		c->setLayer(0);
	}

	// Add physics colliders to level tiles.
	{
		// *********************************
		auto walls = ls::findTiles(ls::WALL);
		for (auto w : walls) {
			auto pos = ls::getTilePosition(w);
			pos += Vector2f(TILE_SIZE / 2, TILE_SIZE / 2);
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<PhysicsComponent>(false, Vector2f(TILE_SIZE, TILE_SIZE));

		}
		// *********************************
	}

	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	cout << " Scene 1 Load Done" << endl;

	setLoaded(true);
}

void Level1Scene::UnLoad() {
	cout << "Scene 1 UnLoad" << endl;
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {


	/*
	  if (ls::getTileAt(player->getPosition()) == ls::END) {
		Engine::ChangeScene((Scene*)&level2);
	  }
	  Scene::Update(dt);
	  */
	Scene::Update(dt);
	/*
	const auto pp = player->getPosition();
	if (ls::getTileAt(pp) == ls::END) {
		Engine::ChangeScene((Scene*)&level2);
	}
	else if (!player->isAlive()) {
		Engine::ChangeScene((Scene*)&level1);
	}
	*/
	static float rocktime = 0.0f;
	rocktime -= dt;

	if (rocktime <= 0.f) {
		rocktime = 5.f;
		auto rock = makeEntity();
		rock->setPosition(ls::getTilePosition(ls::findTiles('r')[0]) +
			Vector2f(0, 40));
		rock->addComponent<BulletComponent>(30.f);
		auto s = rock->addComponent<ShapeComponent>();
		s->setShape<sf::CircleShape>(40.f);
		s->getShape().setFillColor(Color::Cyan);
		s->getShape().setOrigin(40.f, 40.f);
		auto p = rock->addComponent<PhysicsComponent>(true, Vector2f(75.f, 75.f));
		p->setRestitution(.4f);
		p->setFriction(.0001f);
		p->impulse(Vector2f(-100.f, 0));
		p->setMass(1000000000.f);
		p->setCollidable(true);
	}
}

void Level1Scene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}
