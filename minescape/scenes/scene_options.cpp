#include "scene_options.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void OptionsScene::Load() {
    cout << "Options Loaded";
    OptionsScene::selection = 0;
    {
        auto txt = makeEntity();
        auto t = txt->addComponent<TextComponent>("Options");
        auto op1 = makeEntity();
        op1->setPosition(Vector2f(10, 100));
        t = op1->addComponent<TextComponent>("< Volume >\n" + to_string(Engine::getVolume()));
        t->setPosition(op1->getPosition());
        options[0] = op1;
        cout << options[0]->getPosition() << endl;

        auto op2 = makeEntity();
        op2->setPosition(Vector2f(10, 300));
        t = op2->addComponent<TextComponent>("Return");
        t->setPosition(op2->getPosition());
        options[1] = op2;

    }
    setLoaded(true);
}

void OptionsScene::Update(const double& dt)
{
    // cout << "Menu Update "<<dt<<"\n";
    static float buttonCD;

    if (sf::Keyboard::isKeyPressed(keyControls[keybinds::Up]) && buttonCD <= 0)
    {
        if (selection == 0) selection = 1;
        else selection--;
        buttonCD = 0.25f;

        changeText();
    }

    if (sf::Keyboard::isKeyPressed(keyControls[keybinds::Down]) && buttonCD <= 0)
    {
        if (selection == 1) selection = 0;
        else selection++;
        buttonCD = 0.25f;
        changeText();
        
    }

    if (sf::Keyboard::isKeyPressed(keyControls[keybinds::Action1]) && !enterDown || sf::Keyboard::isKeyPressed(keyControls[keybinds::Action2]) && !enterDown)
    {
        switch (selection)
        {
        case(1):
            Engine::ChangeScene(&menu);
            break;

        }
        enterDown = true;
    }

    if (!sf::Keyboard::isKeyPressed(keyControls[keybinds::Action1]) && !sf::Keyboard::isKeyPressed(keyControls[keybinds::Action2]))
    {
        enterDown = false;
    }

    if (sf::Keyboard::isKeyPressed(keyControls[keybinds::Left]) && buttonCD <= 0 && selection == 0 && Engine::getVolume() > 0)
    {
        Engine::setVolume(Engine::getVolume() - 5);
        changeText();
        buttonCD = 0.2f;
    }

    if (sf::Keyboard::isKeyPressed(keyControls[keybinds::Right]) && buttonCD <= 0 && selection == 0 && Engine::getVolume() < 100) 
    {
        Engine::setVolume(Engine::getVolume() + 5);
        changeText();
        buttonCD = 0.2f;
    }

    Scene::Update(dt);
    buttonCD -= dt;
}



void OptionsScene::changeText()
{
    auto t = options[0]->GetCompatibleComponent<TextComponent>();

    t[0]->SetText("Volume\n" + to_string(Engine::getVolume()));
    t = options[1]->GetCompatibleComponent<TextComponent>();
    t[0]->SetText("Return");

    switch (selection)
    {
      case(0):
          t = options[0]->GetCompatibleComponent<TextComponent>();
          t[0]->SetText("< Volume >\n" + to_string(Engine::getVolume()));
          break;
      case(1):
          t = options[1]->GetCompatibleComponent<TextComponent>();
          t[0]->SetText("[ Return ]");
          break;
    }
}