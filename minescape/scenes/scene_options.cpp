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
        op3->setPosition(Vector2f(10, 400));
        t = op3->addComponent<TextComponent>("Windowed");
        t->setPosition(op3->getPosition());
        t->SetSize(45);
        options[2] = op3;

        auto op4 = makeEntity();
        op4->setPosition(Vector2f(10, 500));
        string buttonText = to_string(Engine::getResolution().x) + ',' + to_string(Engine::getResolution().y);
        t = op4->addComponent<TextComponent>(buttonText);
        t->setPosition(op4->getPosition());
        t->SetSize(45);
        options[3] = op4;

        auto op5 = makeEntity();
        op5->setPosition(Vector2f(10, 600));
        t = op5->addComponent<TextComponent>("Return");
        t->setPosition(op5->getPosition());
        t->SetSize(45);
        options[4] = op5;

    }
    changeText();
    setLoaded(true);
}

void OptionsScene::Update(const double& dt)
{
    // cout << "Menu Update "<<dt<<"\n";
    //static float buttonCD;

    if (sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Up]) && buttonCD <= 0)
    {
        if (selection == 0) selection = 4;
        else selection--;
        buttonCD = 0.25f;

        changeText();
    }

    if (sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Down]) && buttonCD <= 0)
    {
        if (selection == 4) selection = 0;
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
            break;
        case(4):
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

    if (sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Left]) && buttonCD <= 0)
    {
        switch (selection)
        {
        case(0):
            if (Engine::getMusicVolume() > 0) {
                Engine::setMusicVolume(Engine::getMusicVolume() - 5);
                changeText();
            }
            break;
        case(2):
            cout << Engine::_windowed << endl;
            Engine::setWindowMode(!Engine::_windowed);
            changeText();
            break;
        case(3):
            Engine::setResolution(false);
            changeText();
            break;
        }
        
        buttonCD = 0.2f;
    }

    if (sf::Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Right]) && buttonCD <= 0)
    {
        switch (selection) 
        {
        case(0):
            if (Engine::getMusicVolume() < 100) {
                Engine::setMusicVolume(Engine::getMusicVolume() + 5);
                changeText();
            }
            break;
        case(2):
            cout << Engine::_windowed << endl;
            Engine::setWindowMode(!Engine::_windowed);
            changeText();
            break;
        case(3):
            Engine::setResolution(true);
            changeText();
            break;
        }
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
    if (Engine::_windowed) t[0]->SetText("Fullscreen");
    else t[0]->SetText("Windowed");

    t = options[3]->GetCompatibleComponent<TextComponent>();
    string buttonText = to_string(Engine::getResolution().x) + ',' + to_string(Engine::getResolution().y);
    t[0]->SetText(buttonText);

    t = options[4]->GetCompatibleComponent<TextComponent>();
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
          if (Engine::_windowed) t[0]->SetText("< Fullscreen >");
          else t[0]->SetText("< Windowed >");
          break;
      case(3):
          t = options[3]->GetCompatibleComponent<TextComponent>();
          buttonText = "< " + to_string(Engine::getResolution().x) + ',' + to_string(Engine::getResolution().y) + " >";
          t[0]->SetText(buttonText);
          break;
      case(4):
          t = options[4]->GetCompatibleComponent<TextComponent>();
          t[0]->SetText("[ Return ]");
          break;
    }
}