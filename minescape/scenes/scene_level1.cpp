#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_camera.h"
#include "../components/cmp_rock.h"
#include "../components/cmp_pausemenu.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../components/cmp_rope.h"
#include "../components/cmp_gas.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_animation.h"

using namespace std;
using namespace sf;

#define TILE_SIZE 64.0f

static shared_ptr<Entity> player;
static shared_ptr<Entity> gas;

static shared_ptr<Entity> pauseMenu;

sf::Texture gasTex;
static shared_ptr<Entity> camera;
sf::Music music;


void Level1Scene::Load() 
{
  paused = false;
  cout << "Scene 1 Load" << endl;
  music.setLoop(true);
  if (music.openFromFile("res/sounds/music/minescape_main_theme.ogg")) music.play();
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
	  player->addComponent<RopeComponent>(150.0f,1.0f);
	  cout << player->getPosition() << endl;
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
  }

//Create Pause Menu
{
	  pauseMenu = makeEntity();
	  pauseMenu->setPosition(Vector2f(-10000, 10000));
	  auto pm = pauseMenu->addComponent<PauseMenu>();
	  cout << "Pause menu at: ";
	  cout << to_string(pauseMenu->getPosition().x);
	  cout << to_string(pauseMenu->getPosition().y) << endl;
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

void Level1Scene::Update(const double& dt) 
{
	float delta = dt;
	if (paused) delta = 0;
	Scene::Update(delta);
	

	//Pause Menu
	if (Keyboard::isKeyPressed(Keyboard::P) && buttonCD <= 0)
	{
		togglePause();
		buttonCD = 0.2f;
	}

	//Do rock stuff
	static float rocktime = 0.0f;
	rocktime -= dt;

	if (rocktime <= 0.f) {
		rocktime = 2.f;
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
	buttonCD -= dt;
}



void Level1Scene::togglePause()
{
	paused = !paused;
	if(paused)
	{
		pauseMenu->setPosition(Vector2f(Engine::GetWindow().mapPixelToCoords(Vector2i(Engine::getWindowSize().x - 250, Engine::getWindowSize().y - 250))));
		pauseMenu->get_components<PauseMenu>()[0]->pauseGame();
		cout << "Pause Menu Loc";
		cout << pauseMenu->getPosition() << endl;
		
	}else
	{
		pauseMenu->setPosition(Vector2f(-1000, 1000));
		pauseMenu->get_components<PauseMenu>()[0]->pauseGame();
	}
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
