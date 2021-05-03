#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <ecm.h>
#include <future>
#include <maths.h>
#include <mutex>
#include <string>

class Scene {
public:
  Scene() = default;
  virtual ~Scene();
  virtual void Load() = 0;
  virtual void LoadAsync();
  virtual void UnLoad();
  virtual void Update(const double& dt);
  virtual void Render();
  bool isLoaded() const;
  std::shared_ptr<Entity> makeEntity();

  EntityManager ents;

protected:
  void setLoaded(bool);
private:
  mutable bool _loaded;
  mutable std::future<void> _loaded_future;
  mutable std::mutex _loaded_mtx;
};

class Engine {
public:

	static sf::Keyboard::Key keyControls[6];

	static enum keybinds
	{
		Up = 0,
		Down = 1,
		Left = 2,
		Right = 3,
		Action1 = 4,
		Action2 = 5
	};

	static float musicVolume;
	static float sfxVolume;

	static sf::Vector2f _resolution;
	static sf::Vector2f resolutions[4];
	static int resSelection;

	static bool _windowed;
	static bool _gamepad;

  Engine() = delete;
  static void Start(unsigned int width, unsigned int height,
                    const std::string& gameName, Scene* scn);
  static void ChangeScene(Scene*);
  static sf::RenderWindow& GetWindow();
  static sf::Vector2u getWindowSize();
  static void setVsync(bool b);
  static void setMusicVolume(int vol);
  static int getMusicVolume();
  static void setFXVolume(int vol);
  static int getFXVolume();
  static void setResolution(bool direction);
  static sf::Vector2f getResolution();
  static void setWindowMode(bool iswindow);
  static void setUseGamepad(bool gamepad);

private:
  static Scene* _activeScene;
  static std::string _gameName;
  static void Update();
  static void Render(sf::RenderWindow& window);
};

namespace timing {
// Return time since Epoc
long long now();
// Return time since last() was last called.
long long last();
} // namespace timing