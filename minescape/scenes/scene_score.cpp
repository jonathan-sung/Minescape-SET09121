#include "scene_score.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include <fstream>
#include "../components/cmp_text.h"
#include "../components/cmp_animation.h"

using namespace std;
using namespace sf;

void ScoreScene::Load() {
	Engine::GetWindow().setView(View(Vector2f(640, 360), Vector2f(Engine::getResolution().x, Engine::getResolution().y)));
	auto title = makeEntity();
	auto t = title->addComponent<TextComponent>("Times");
	t->setPosition(Vector2f(0, 0));
	t->SetSize(32);

	string myText;
	std::vector<float> scores;

	ifstream MyReadFile("scores.txt");
	while (getline(MyReadFile, myText)) {
		scores.push_back(stof(myText));
	}
	MyReadFile.close();
	std::sort(scores.begin(), scores.end());

	int i = 0;
	for (float time : scores) {
		auto txt = makeEntity();
		auto t = txt->addComponent<TextComponent>(to_string(time));
		t->SetSize(32);
		t->setPosition(Vector2f(0, t->GetSize() * 2 + i * t->GetSize()));
		cout << to_string(time);
		i++;
	}

	auto next = makeEntity();
	//cout << "hello " << Engine::keyControls[Engine::keybinds::Action1] << endl;
	auto tc = next->addComponent<TextComponent>("Press Action 1 to continue.");
	tc->SetSize(32);
	tc->setPosition(Vector2f(0, tc->GetSize() * 2 + i * tc->GetSize()));


	//t->setPosition(sf::Vector2f(100, 100));
	setLoaded(true);
}

void ScoreScene::UnLoad() {
	cout << "Scene 1 UnLoad" << endl;

	Scene::UnLoad();
}

void ScoreScene::Update(const double& dt) {
	if (Engine::keyPressed[Engine::Action1]) {
		Engine::ChangeScene((Scene*)&level1);
	}
	Scene::Update(dt);
}

void ScoreScene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}
