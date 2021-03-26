#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level1Scene::Load() {
  cout << "Scene 1 Load" << endl;
  ls::loadLevelFile("res/level_1.txt", 40.0f);
  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
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
	  player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
  }

  // Add physics colliders to level tiles.
  {
	  // *********************************
	  auto walls = ls::findTiles(ls::WALL);
	  for (auto w : walls) {
		  auto pos = ls::getTilePosition(w);
		  pos += Vector2f(20.f, 20.f);
		  auto e = makeEntity();
		  e->setPosition(pos);
		  e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
	  }
	  // *********************************
  }

  //Simulate long loading times
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
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
	const auto pp = player->getPosition();
	if (ls::getTileAt(pp) == ls::END) {
		Engine::ChangeScene((Scene*)&level2);
	}
	else if (!player->isAlive()) {
		Engine::ChangeScene((Scene*)&level1);
	}
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
