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
        t->SetSize(64);
        auto op1 = makeEntity();
        op1->setPosition(Vector2f(10, 150));
        t = op1->addComponent<TextComponent>("< Volume >\n" + to_string(Engine::getMusicVolume()));
        t->setPosition(op1->getPosition());
        t->SetSize(45);
        options[0] = op1;
        cout << options[0]->getPosition() << endl;

        auto op2 = makeEntity();
        op2->setPosition(Vector2f(10, 300));
        t = op2->addComponent<TextComponent>("Key Bindings");
        t->setPosition(op2->getPosition());
        t->SetSize(45);
        options[1] = op2;

        auto op3 = makeEntity();
        op3->setPosition(Vector2f(10, 450));
        t = op3->addComponent<TextComponent>("Return");
        t->setPosition(op3->getPosition());
        t->SetSize(45);
        options[2] = op3;

    }
    setLoaded(true);
}

void OptionsScene::Update(const double& dt)
{
    // cout << "Menu Update "<<dt<<"\n";
    //static float buttonCD;

    if (sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Up]) && buttonCD <= 0)
    {
        if (selection == 0) selection = 2;
        else selection--;
        buttonCD = 0.25f;

        changeText();
    }

    if (sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Down]) && buttonCD <= 0)
    {
        if (selection == 2) selection = 0;
        else selection++;
        buttonCD = 0.25f;
        changeText();
    }

    if (sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Action1]) && !enterDown || 
        sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Action2]) && !enterDown)
    {
        switch (selection)
        {
        case(1):
            Engine::ChangeScene(&keyBindScene);
            break;
        case(2):
            Engine::ChangeScene(&menu);
            break;

        }
        enterDown = true;
    }

    if (!sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Action1]) && 
        !sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Action2]))
    {
        enterDown = false;
    }

    if (sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Left]) 
        && buttonCD <= 0 && selection == 0 && Engine::getMusicVolume() > 0)
    {
        Engine::setMusicVolume(Engine::getMusicVolume() - 5);
        changeText();
        buttonCD = 0.2f;
    }

    if (sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Right]) && buttonCD <= 0 &&
        selection == 0 && Engine::getMusicVolume() < 100)
    {
        Engine::setMusicVolume(Engine::getMusicVolume() + 5);
        changeText();
        buttonCD = 0.2f;
    }

    Scene::Update(dt);
    buttonCD -= dt;
}


void OptionsScene::changeText()
{
    auto t = options[0]->GetCompatibleComponent<TextComponent>();

    t[0]->SetText("Volume\n" + to_string(Engine::getMusicVolume()));
    t = options[1]->GetCompatibleComponent<TextComponent>();
    t[0]->SetText("Key Bindings");
    t = options[2]->GetCompatibleComponent<TextComponent>();
    t[0]->SetText("Return");

    switch (selection)
    {
      case(0):
          t = options[0]->GetCompatibleComponent<TextComponent>();
          t[0]->SetText("< Volume >\n" + to_string(Engine::getMusicVolume()));
          break;
      case(1):
          t = options[1]->GetCompatibleComponent<TextComponent>();
          t[0]->SetText("[ Key Bindings ]");
          break;
      case(2):
          t = options[2]->GetCompatibleComponent<TextComponent>();
          t[0]->SetText("[ Return ]");
          break;
    }
}