#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <stdexcept>
#include "../minescape/game.h"

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;

sf::Keyboard::Key Engine::keyControls[6] = {
	Keyboard::W,
	Keyboard::S,
	Keyboard::A,
	Keyboard::D,
	Keyboard::Space,
	Keyboard::Enter
};

int Engine::joypadControls[2] = {
	0,
	5
};

bool Engine::keyPressed[6] = {
false,
false,
false,
false,
false,
false
};

float Engine::musicVolume;
float Engine::sfxVolume;

sf::Vector2f Engine::_resolution;
sf::Vector2f Engine::resolutions[4];
int Engine::resSelection;

bool Engine::_windowed = false;

void Loading_update(float dt, const Scene* const scn) {
	//  cout << "Eng: Loading Screen\n";
	if (scn->isLoaded()) {
		cout << "Eng: Exiting Loading Screen\n";
		loading = false;
	}
	else {
		loadingspinner += 220.0f * dt;
		loadingTime += dt;
	}
}
void Loading_render() {
	// cout << "Eng: Loading Screen Render\n";
	static CircleShape octagon(80, 8);
	octagon.setOrigin(80, 80);
	octagon.setRotation(loadingspinner);
	octagon.setPosition(Vcast<float>(Engine::getWindowSize()) * .5f);
	octagon.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));
	static Text t("Loading", *Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
	t.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));
	t.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.3f));
	Renderer::queue(&t);
	Renderer::queue(&octagon);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update()
{
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();

	bool joystickUsed = false;
	////joystick
	if (sf::Joystick::isConnected(0))
	{
		keyPressed[keybinds::Up] = sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -50.0f;
		keyPressed[keybinds::Down] = sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 50.0f;
		keyPressed[keybinds::Left] = sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -50.0f;
		keyPressed[keybinds::Right] = sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 50.0f;
		keyPressed[keybinds::Action1] = sf::Joystick::isButtonPressed(0, joypadControls[0]);
		keyPressed[keybinds::Action2] = sf::Joystick::isButtonPressed(0, joypadControls[1]);

		//check if joystick was used to avoid overwrite
		for (int i = 0; i < sizeof(keyPressed) / sizeof(keyPressed[0]); i++)
		{
			if (keyPressed[i])
			{
				joystickUsed = true;
				break;
			}
		}
	}

	if (!joystickUsed) {
		//keyboard
		keyPressed[keybinds::Up] = sf::Keyboard::isKeyPressed(keyControls[keybinds::Up]);
		keyPressed[keybinds::Down] = sf::Keyboard::isKeyPressed(keyControls[keybinds::Down]);
		keyPressed[keybinds::Left] = sf::Keyboard::isKeyPressed(keyControls[keybinds::Left]);
		keyPressed[keybinds::Right] = sf::Keyboard::isKeyPressed(keyControls[keybinds::Right]);
		keyPressed[keybinds::Action1] = sf::Keyboard::isKeyPressed(keyControls[keybinds::Action1]);
		keyPressed[keybinds::Action2] = sf::Keyboard::isKeyPressed(keyControls[keybinds::Action2]);
	}

	if (paused) dt = 0;
	{
		frametimes[++ftc] = dt;
		static string avg = _gameName + " FPS:";
		if (ftc % 60 == 0) {
			double davg = 0;
			for (const auto t : frametimes) {
				davg += t;
			}
			davg = 1.0 / (davg / 255.0);
			_window->setTitle(avg + toStrDecPt(2, davg));
		}
	}

	if (loading) {
		Loading_update(dt, _activeScene);
	}
	else if (_activeScene != nullptr) {
		Physics::update(dt);
		_activeScene->Update(dt);
	}
}

void Engine::Render(RenderWindow& window) {
	if (loading) {
		Loading_render();
	}
	else if (_activeScene != nullptr) {
		_activeScene->Render();
	}

	Renderer::render();
}

void Engine::Start(unsigned int width, unsigned int height,
	const std::string& gameName, Scene* scn) {

	setMusicVolume(100);
	setFXVolume(100);
	//window and scenes
	resSelection = 1;
	resolutions[0] = Vector2f(800, 600);
	resolutions[1] = Vector2f(1280, 720);
	resolutions[2] = Vector2f(1366, 768);
	resolutions[3] = Vector2f(1920, 1080);
	_resolution = resolutions[resSelection];
	Vector2f oldRes = _resolution;

	RenderWindow window(VideoMode(_resolution.x, _resolution.y), gameName);
	bool style = false;
	_gameName = gameName;
	_window = &window;
	Renderer::initialise(window);
	Physics::initialise();
	ChangeScene(scn);
	while (window.isOpen()) {
		Event event;
		if (_windowed != style || oldRes != _resolution)
		{
			if (_windowed)
			{
				window.create(VideoMode(_resolution.x, _resolution.y), gameName, sf::Style::Fullscreen);
				style = _windowed;
				cout << "Fullscreen" << endl;
				oldRes = _resolution;
			}
			else
			{

				window.create(VideoMode(_resolution.x, _resolution.y), gameName);
				cout << "Windowed" << endl;
				style = _windowed;
				oldRes = _resolution;
			}
		}

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		/* if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		   window.close();
		 }*/

		window.clear();
		Update();
		Render(window);
		window.display();
	}
	if (_activeScene != nullptr) {
		_activeScene->UnLoad();
		_activeScene = nullptr;
	}
	window.close();
	Physics::shutdown();
	//Render::shutdown();
}

std::shared_ptr<Entity> Scene::makeEntity() {
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }



void Engine::ChangeScene(Scene* s) {
	cout << "Eng: changing scene: " << s << endl;
	auto old = _activeScene;
	_activeScene = s;

	if (old != nullptr) {
		old->UnLoad(); // todo: Unload Async
	}

	if (!s->isLoaded()) {
		cout << "Eng: Entering Loading Screen\n";
		loadingTime = 0;
		_activeScene->LoadAsync();
		//_activeScene->Load();
		loading = true;
	}
}

void Engine::setMusicVolume(int vol) { musicVolume = vol; }
int Engine::getMusicVolume() { return musicVolume; }

void Engine::setFXVolume(int vol) { sfxVolume = vol; }
int Engine::getFXVolume() { return sfxVolume; }

void Engine::setResolution(bool direction)
{
	if (resSelection == 0 && !direction) resSelection = 3;
	else if (resSelection == 3 && direction) resSelection = 0;
	else if (direction) resSelection++;
	else if (!direction) resSelection--;

	_resolution = resolutions[resSelection];
}

Vector2f Engine::getResolution()
{
	return _resolution;
}

void Engine::setWindowMode(const bool iswindow)
{
	_windowed = iswindow;

}

void Scene::Update(const double& dt) { ents.update(dt); }

void Scene::Render() { ents.render(); }

bool Scene::isLoaded() const {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		// Are we already loading asynchronously?
		if (_loaded_future.valid() // yes
			&&                     // Has it finished?
			_loaded_future.wait_for(chrono::seconds(0)) ==
			future_status::ready) {
			// Yes
			_loaded_future.get();
			_loaded = true;
		}
		return _loaded;
	}
}
void Scene::setLoaded(bool b) {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		_loaded = b;
	}
}

void Scene::UnLoad() {
	ents.list.clear();
	setLoaded(false);
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

namespace timing {
	// Return time since Epoc
	long long now() {
		return std::chrono::high_resolution_clock::now()
			.time_since_epoch()
			.count();
	}
	// Return time since last() was last called.
	long long last() {
		auto n = now();
		static auto then = now();
		auto dt = n - then;
		then = n;
		return dt;
	}
} // namespace timing

Scene::~Scene() { UnLoad(); }