#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_camera.h"
#include "../components/cmp_rock.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../components/cmp_rope.h"
#include "../components/cmp_gas.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_canary_ai.h"
#include "../components/cmp_animation.h"

using namespace std;
using namespace sf;

#define TILE_SIZE 64.0f

static shared_ptr<Entity> player;
static shared_ptr<Entity> gas;
sf::Texture gasTex;
static shared_ptr<Entity> camera;
sf::Music music;


void Level1Scene::Load() {
  cout << "Scene 1 Load" << endl;
  music.setVolume(20);
  music.setLoop(true);
  ls::loadLevelFile("res/level_1.txt", TILE_SIZE);
  auto ho = Engine::getWindowSize().y - (ls::getHeight() * TILE_SIZE);
  //auto ho = 0;
  ls::setOffset(Vector2f(0, ho));

  // Create player
  {
	  // *********************************
	  player = makeEntity();
	  player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	  //auto s = player->addComponent<ShapeComponent>();
	  //s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
	  //s->getShape().setFillColor(Color::Yellow);
	  //s->getShape().setOrigin(10.f, 15.f);
	  // *********************************
	  player->addTag("player");
	  player->addComponent<PlayerPhysicsComponent>(Vector2f(32.f, 64.f));
	  player->addComponent<Animation>("res/character_walk.png", 4);
	  // *********************************
	  player->addComponent<RopeComponent>(200.0f,1.0f);
  }
	
  // Create gas
  {
	  if (!gasTex.loadFromFile("res/gas.png")) {
		  cerr << "Failed to load spritesheet!" << std::endl;
	  }
	  else {
		  std::cout << "Spritesheet load successful!" << std::endl;
	  }

	  // *********************************
	  gas = makeEntity();
	  gas->setPosition(Vector2f(0, 500));
	  auto s = gas->addComponent<SpriteComponent>();
	  s->getSprite().setTexture(gasTex);
	  auto g = gas->addComponent<GasComponent>();
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
		  pos += Vector2f(TILE_SIZE/2, TILE_SIZE/2);
		  auto e = makeEntity();
		  e->setPosition(pos);
		  e->addComponent<PhysicsComponent>(false, Vector2f(TILE_SIZE, TILE_SIZE));
	  }
	  // *********************************
	  if (music.openFromFile("res/sounds/music/minescape_main_theme.ogg")) music.play();
  }

  //Enemies
  {
	  //// *********************************
	  auto enemyTiles = ls::findTiles(ls::ENEMY);
	  for (auto n : enemyTiles) {
		  auto pos = ls::getTilePosition(n);
		  pos += Vector2f(TILE_SIZE / 2, TILE_SIZE / 2);
		  auto enemy = makeEntity();
		  enemy->setPosition(pos);
		  enemy->addComponent<CanaryAIComponent>(150.0f,5.0f, sf::Vector2f(85.0f, 100.0f), sf::Vector2f(250.0f, 65.0f));
		  enemy->addComponent<HurtComponent>();
		  auto a = enemy->addComponent<Animation>("res/canary.png", 5);
		  a->animate = true;
		  //auto s = enemy->addComponent<ShapeComponent>();
		  //s->setShape<sf::RectangleShape>(sf::Vector2f(20.f, 30.f));
		  //s->getShape().setFillColor(Color::Red);
		  //s->getShape().setOrigin(10.f, 15.f);
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
  music.stop();
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

	//Do rock stuff
	static float rocktime = 0.0f;
	rocktime -= dt;

	if (rocktime <= 0.f) {
		rocktime = 5.f;
		auto rock = makeEntity();
		rock->addTag("rock");
		rock->setPosition(ls::getTilePosition(ls::findTiles('r')[0]) +
			Vector2f(0, 40));
		rock->addComponent<Rock>();
		rock->addComponent<HurtComponent>();
		auto sc = rock->addComponent<Animation>("res/rock.png", 1);
		//sc->getSprite().setTexture(ls::spritesheet);
		//sc->getSprite().setTextureRect(sf::IntRect(0 * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		//sc->getSprite().setOrigin(Vector2f((TILE_SIZE / 2), TILE_SIZE / 2));
	}
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
