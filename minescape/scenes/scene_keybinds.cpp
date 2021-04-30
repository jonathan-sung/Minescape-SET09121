#include "scene_keybinds.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

//float KeybindsScene::buttonCD = 0;

void KeybindsScene::Load() {

    cout << "Options Loaded";
    KeybindsScene::selection = 0;
    {
        auto txt = makeEntity();
        auto t = txt->addComponent<TextComponent>("Keybinds");
        t->SetSize(64);

        auto op1 = makeEntity();
        op1->setPosition(Vector2f(10, 100));
        t = op1->addComponent<TextComponent>("[ Up ]");
        t->setPosition(op1->getPosition());
        t->SetSize(45);
        options[0] = op1;

        auto op2 = makeEntity();
        op2->setPosition(Vector2f(10, 150));
        t = op2->addComponent<TextComponent>("Down");
        t->setPosition(op2->getPosition());
        t->SetSize(45);
        options[1] = op2;

        auto op3 = makeEntity();
        op3->setPosition(Vector2f(10, 200));
        t = op3->addComponent<TextComponent>("Left");
        t->setPosition(op3->getPosition());
        t->SetSize(45);
        options[2] = op3;

        auto op4 = makeEntity();
        op4->setPosition(Vector2f(10, 250));
        t = op4->addComponent<TextComponent>("Right");
        t->setPosition(op4->getPosition());
        t->SetSize(45);
        options[3] = op4;

        auto op5 = makeEntity();
        op5->setPosition(Vector2f(10, 300));
        t = op5->addComponent<TextComponent>("Action 1");
        t->setPosition(op5->getPosition());
        t->SetSize(45);
        options[4] = op5;

        auto op6 = makeEntity();
        op6->setPosition(Vector2f(10, 350));
        t = op6->addComponent<TextComponent>("Action 2");
        t->setPosition(op6->getPosition());
        t->SetSize(45);
        options[5] = op6;

        
        auto op7 = makeEntity();
        op7->setPosition(Vector2f(10, 500));
        t = op7->addComponent<TextComponent>("Return");
        t->setPosition(op7->getPosition());
        t->SetSize(45);
        options[6] = op7;
        
    }
    rebinding = false;
    setLoaded(true);
}

void KeybindsScene::RebindKey(int index)
{
    Event event;
    bool done = false;
    while (!done)
    {
        while (Engine::GetWindow().pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                keyControls[index] = event.key.code;
                cout << "Redbinded:" << index << " to " << to_string(keyControls[index]);
                done = true;
                buttonCD = 0.25f;
                
            }
            if (event.type == Event::JoystickButtonPressed) {
                //change keycontrols to joystickcontrols
                keyControls[index] = event.key.code;
                cout << "Redbinded:" << index << " to " << to_string(keyControls[index]);
                done = true;
                buttonCD = 0.25f;
            }
        }
    }
    
}

void KeybindsScene::Update(const double& dt)
{
    // cout << "Menu Update "<<dt<<"\n";
    
    //static float buttonCD;

    if (!rebinding)
    {

        if (sf::Keyboard::isKeyPressed(keyControls[keybinds::Up]) && buttonCD <= 0)
        {
            if (selection == 0) selection = 6;
            else selection--;
            buttonCD = 0.25f;

            changeText();
        }

        if (sf::Keyboard::isKeyPressed(keyControls[keybinds::Down]) && buttonCD <= 0)
        {
            if (selection == 6) selection = 0;
            else selection++;
            buttonCD = 0.25f;
            changeText();

        }

        if (sf::Keyboard::isKeyPressed(keyControls[keybinds::Action1]) && !enterDown || sf::Keyboard::isKeyPressed(keyControls[keybinds::Action2]) && !enterDown)
        {
            switch (selection)
            {
            case(6):
                Engine::ChangeScene(&optionscene);
                break;
            default:
                cout << "rebinding" << endl;
                rebinding = true;
                break;
            }
            enterDown = true;
        }
    }
    else 
    {
        if (!enterDown)
        {
            cout << "press key to rebind" << endl;
            cout << "Seceltion no: ";
            cout << selection << endl;
            RebindKey(selection);
            rebinding = false;
            enterDown = true;
        }

    }

    if (!sf::Keyboard::isKeyPressed(keyControls[keybinds::Action1]) && !sf::Keyboard::isKeyPressed(keyControls[keybinds::Action2]))
    {
        enterDown = false;
    }


    Scene::Update(dt);
    buttonCD -= dt;
}



void KeybindsScene::changeText()
{
    auto t = options[0]->GetCompatibleComponent<TextComponent>();

    t[0]->SetText("Up");
    t = options[1]->GetCompatibleComponent<TextComponent>();
    t[0]->SetText("Down");
    t = options[2]->GetCompatibleComponent<TextComponent>();
    t[0]->SetText("Left");
    t = options[3]->GetCompatibleComponent<TextComponent>();
    t[0]->SetText("Right");
    t = options[4]->GetCompatibleComponent<TextComponent>();
    t[0]->SetText("Action 1");
    t = options[5]->GetCompatibleComponent<TextComponent>();
    t[0]->SetText("Action 2");
    t = options[6]->GetCompatibleComponent<TextComponent>();
    t[0]->SetText("Return");

    switch (selection)
    {
    case(0):
        t = options[0]->GetCompatibleComponent<TextComponent>();
        t[0]->SetText("[ Up ]");
        break;
    case(1):
        t = options[1]->GetCompatibleComponent<TextComponent>();
        t[0]->SetText("[ Down ]");
        break;
    case(2):
        t = options[2]->GetCompatibleComponent<TextComponent>();
        t[0]->SetText("[ Left ]");
        break;
    case(3):
        t = options[3]->GetCompatibleComponent<TextComponent>();
        t[0]->SetText("[ Right ]");
        break;
    case(4):
        t = options[4]->GetCompatibleComponent<TextComponent>();
        t[0]->SetText("[ Action 1 ]");
        break;
    case(5):
        t = options[5]->GetCompatibleComponent<TextComponent>();
        t[0]->SetText("[ Action 2 ]");
        break;
    case(6):
        t = options[6]->GetCompatibleComponent<TextComponent>();
        t[0]->SetText("[ Return ]");
        break;
    }
}