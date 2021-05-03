#include "cmp_pausemenu.h"
#include "../game.h"
#include "cmp_text.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

int PauseMenu::selection = 0;

PauseMenu::PauseMenu(Entity* p) : Component(p)
{
	
	auto t1 = p->addComponent<TextComponent>();
	t1->SetText("Paused");
	t1->setPosition(Vector2f(p->getPosition().x + 200, p->getPosition().y + 50));

	auto t2 = p->addComponent<TextComponent>();
	t2->SetText("[ Return ]");
	t2->setPosition(Vector2f(p->getPosition().x + 200, p->getPosition().y + 150));
	options[0] = t2;

	auto t3 = p->addComponent<TextComponent>();
	t3->SetText("Restart");
	t3->setPosition(Vector2f(p->getPosition().x + 200, p->getPosition().y + 250));
	options[1] = t3;

	auto t4 = p->addComponent<TextComponent>();
	t4->SetText("Quit");
	t4->setPosition(Vector2f(p->getPosition().x + 200, p->getPosition().y + 350));
	options[2] = t4;

	selection = 0;
	cout << p->getPosition() << endl;
}

float PauseMenu::delta = 0;

void PauseMenu::update(double dt)
{
	
	if (paused) 
	{
		if (Engine::keyPressed[Engine::keybinds::Down] && buttonCD <= 0)
		{
			if (selection == 2) selection = 0;
			else selection++;
			changeText();
			buttonCD = 0.25f;
		}
		if (Engine::keyPressed[Engine::keybinds::Up] && buttonCD <= 0)
		{
			if (selection == 0) selection = 2;
			else selection--;
			changeText();
			buttonCD = 0.25;
		}

		if (Engine::keyPressed[Engine::keybinds::Action1] && !enterDown ||
			Engine::keyPressed[Engine::keybinds::Action2] && !enterDown)
		{
			cout << "ENTER PRESSED" << endl;
			switch (selection)
			{
			case(0):
				for each (shared_ptr<TextComponent> op in options)
				{
					op->setPosition(Vector2f(-10000, 10000));
				}
				paused = false;
				break;
			case(1):
				Engine::ChangeScene(Engine::restartSceneRef);
				paused = false;
				break;
			case(2):
				_parent->scene->UnLoad();
				paused = false;
				Engine::GetWindow().setView(View(Vector2f(Engine::getResolution().x / 2, Engine::getResolution().y / 2), Vector2f(Engine::getResolution().x, Engine::getResolution().y)));
				Engine::ChangeScene(&menu);
				break;
			}
			enterDown = true;
		}

		if (!Engine::keyPressed[Engine::keybinds::Action1] &&
			!Engine::keyPressed[Engine::keybinds::Action2])
		{
			enterDown = false;
		}
		buttonCD -= delta;
	}else
	{
		delta = dt;
	}

}

void PauseMenu::render()
{

}

void PauseMenu::pauseGame()
{
	if (paused) 
	{
		int loop = 0;
		for each (shared_ptr<TextComponent> tc in options)
		{
			//tc->setPosition(Vector2f(Engine::GetWindow().mapPixelToCoords(Vector2i(Engine::getWindowSize().x - 750,  Engine::getWindowSize().y - (100 * loop)))));
			tc->setPosition(Vector2f(_parent->getPosition().x - 450, _parent->getPosition().y - 250 + (100 * loop)));
			//cout << tc->getPosition() << endl;
			loop++;
		}
	}else
	{
		int loop = 0;
		for each (shared_ptr<TextComponent> tc in options)
		{
			//tc->setPosition(Vector2f(Engine::GetWindow().mapPixelToCoords(Vector2i(Engine::getWindowSize().x - 750,  Engine::getWindowSize().y - (100 * loop)))));
			tc->setPosition(Vector2f(-1000, -1000));
			cout << tc->getPosition() << endl;
			loop++;
		}
	}
}
void PauseMenu::changeText()
{
	options[0]->SetText("Return");
	options[1]->SetText("Restart");
	options[2]->SetText("Quit");

	switch (selection)
	{
	case(0):
		options[0]->SetText("[ Return ]");
		break;
	case(1):
		options[1]->SetText("[ Restart ]");
		break;
	case(2):
		options[2]->SetText("[ Quit ]");
		break;
	}
}