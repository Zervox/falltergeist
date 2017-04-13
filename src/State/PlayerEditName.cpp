/*
 * Copyright 2012-2016 Falltergeist Developers.
 *
 * This file is part of Falltergeist.
 *
 * Falltergeist is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Falltergeist is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Falltergeist.  If not, see <http://www.gnu.org/licenses/>.
 */

// Related headers
#include "../State/PlayerEditName.h"

// C++ standard includes
#include <ctype.h>

// Falltergeist includes
#include "../functions.h"
#include "../Game/DudeObject.h"
#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
#include "../ResourceManager.h"
#include "../UI/TextArea.h"
#include "../UI/ImageButton.h"
#include "../UI/Image.h"
#include "../UI/Rectangle.h"

// Third party includes

namespace Falltergeist
{
    namespace State
    {
        std::map<char, char> PlayerEditName::_keyCodes = {
            { SDLK_a, 'a' },{ SDLK_b, 'b' },{ SDLK_c, 'c' },{ SDLK_d, 'd' },{ SDLK_e, 'e' },{ SDLK_f, 'f' },{ SDLK_g, 'g' },{ SDLK_h, 'h' },{ SDLK_i, 'i' },{ SDLK_j, 'j' },
            { SDLK_k, 'k' },{ SDLK_l, 'l' },{ SDLK_m, 'm' },{ SDLK_n, 'n' },{ SDLK_o, 'o' },{ SDLK_p, 'p' },{ SDLK_q, 'q' },{ SDLK_r, 'r' },{ SDLK_s, 's' },{ SDLK_t, 't' },
            { SDLK_u, 'u' },{ SDLK_v, 'v' },{ SDLK_w, 'w' },{ SDLK_x, 'x' },{ SDLK_y, 'y' },{ SDLK_z, 'z' },{ SDLK_1, '1' },{ SDLK_2, '2' },{ SDLK_3, '3' },{ SDLK_4, '4' },
            { SDLK_5, '5' },{ SDLK_6, '6' },{ SDLK_7, '7' },{ SDLK_8, '8' },{ SDLK_9, '9' },{ SDLK_0, '0' },{ SDLK_KP_1, '1' },{ SDLK_KP_2, '2' },{ SDLK_KP_3, '3' },{ SDLK_KP_4, '4' },
            { SDLK_KP_5, '5' },{ SDLK_KP_6, '6' },{ SDLK_KP_7, '7' },{ SDLK_KP_8, '8' },{ SDLK_KP_9, '9' },{ SDLK_KP_0, '0' },
        };


        PlayerEditName::PlayerEditName() : State()
        {
        }

        PlayerEditName::~PlayerEditName()
        {
        }

        void PlayerEditName::init()
        {
            if (_initialized) return;
            State::init();

            setFullscreen(false);
            setModal(true);

            Point bgPos = Point((Game::getInstance()->renderer()->size() - Point(640, 480)) / 2);
            int bgX = bgPos.x();
            int bgY = bgPos.y();

            _timer = SDL_GetTicks();

            auto bg = new UI::Image("art/intrface/charwin.frm");
            bg->setPosition(bgPos + Point(22, 0));

            auto nameBox = new UI::Image("art/intrface/namebox.frm");
            nameBox->setPosition(bgPos + Point(35, 10));

            auto doneBox = new UI::Image("art/intrface/donebox.frm");
            doneBox->setPosition(bgPos + Point(35, 40));

            auto doneLabel = new UI::TextArea(_t(MSG_EDITOR, 100), bgX+65, bgY+43);
            doneLabel->setFont("font3.aaf", {0xb8, 0x9c, 0x28, 0xff});

            auto doneButton = new UI::ImageButton(UI::ImageButton::Type::SMALL_RED_CIRCLE, bgX+45, bgY+43);
            doneButton->mouseClickHandler().add(std::bind(&PlayerEditName::onDoneButtonClick, this, std::placeholders::_1));

            _name = new UI::TextArea(Game::getInstance()->player()->name(), bgX+43, bgY+15);

            _cursor = new UI::Rectangle(bgPos + Point(83, 15) ,{5,8}, {0x3F, 0xF8, 0x00, 0xFF});

            addUI(bg);
            addUI(nameBox);
            addUI(doneBox);
            addUI(doneLabel);
            addUI(doneButton);
            addUI(_name);
            addUI(_cursor);
        }

        void PlayerEditName::onTextAreaKeyDown()
        {
            std::string text = _name->text();

            if (Kb.bp(SDLK_BACKSPACE)) 
            {
                if (text.length() > 0)
                {
                    text = text.substr(0, text.length() - 1);
                    _name->setText(text.c_str());
                    return;
                }
                return;
            }
            if (Kb.bp(SDLK_RETURN)) {doDone();return;}
            if (Kb.bp(SDLK_ESCAPE)) {doBack();}
            if (Kb.bp(SDLK_LCTRL) || Kb.bp(SDLK_RCTRL)) {return;}
            if (Kb.bp(SDLK_LALT) || Kb.bp(SDLK_RALT)) {return;}

            if (text.length() == 11) return;

            for (auto v : _keyCodes) 
           {
                if (Kb.bp(v.first)) 
                {
                    text += (Kb.b(SDLK_LSHIFT) ? toupper(v.second) : v.second);
                    _name->setText(text.c_str());
                }
           }

        }

        void PlayerEditName::onDoneButtonClick(Event::Mouse* event)
        {
            doDone();
        }

        void PlayerEditName::think()
        {
            int bgX = (Game::getInstance()->renderer()->width() - 640) / 2;
            State::think();
            if (SDL_GetTicks() - _timer > 300)
            {
                _cursor->setVisible(!_cursor->visible());
                _timer = SDL_GetTicks();
            }
            _cursor->setPosition({bgX + _name->textSize().width() + 45, _cursor->position().y()});
            if (Kb.AnyKey()) 
            {
                onTextAreaKeyDown();
            }
        }

        void PlayerEditName::doBack()
        {
            Game::getInstance()->popState();
        }

        void PlayerEditName::doDone()
        {
            std::string text(_name->text());
            if (text.length() > 0)
            {
                Game::getInstance()->player()->setName(text.c_str());
            }
            Game::getInstance()->popState();
        }
    }
}
