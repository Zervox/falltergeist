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
#include "../Input/Mouse.h"

// C++ standard includes
#include <memory>

// Falltergeist includes
#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Texture.h"
#include "../ResourceManager.h"
#include "../Settings.h"
#include "../UI/Animation.h"
#include "../UI/AnimationQueue.h"
#include "../UI/Image.h"

// Third party includes
#include <SDL.h>

namespace Falltergeist
{

namespace Input
{

Mouse::Mouse()
{
    SDL_ShowCursor(0); // Hide cursor
    _ui.insert(std::make_pair(Cursor::BIG_ARROW, std::make_unique<UI::Image>("art/intrface/stdarrow.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_W, std::make_unique<UI::Image>("art/intrface/scrwest.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_W_X, std::make_unique<UI::Image>("art/intrface/scrwx.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_N, std::make_unique<UI::Image>("art/intrface/scrnorth.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_N_X, std::make_unique<UI::Image>("art/intrface/scrnx.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_S, std::make_unique<UI::Image>("art/intrface/scrsouth.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_S_X, std::make_unique<UI::Image>("art/intrface/scrsx.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_E, std::make_unique<UI::Image>("art/intrface/screast.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_E_X, std::make_unique<UI::Image>("art/intrface/screx.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_NW, std::make_unique<UI::Image>("art/intrface/scrnwest.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_NW_X, std::make_unique<UI::Image>("art/intrface/scrnwx.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_SW, std::make_unique<UI::Image>("art/intrface/scrswest.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_SW_X, std::make_unique<UI::Image>("art/intrface/scrswx.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_NE, std::make_unique<UI::Image>("art/intrface/scrneast.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_NE_X, std::make_unique<UI::Image>("art/intrface/scrnex.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_SE, std::make_unique<UI::Image>("art/intrface/scrseast.frm")));
    _ui.insert(std::make_pair(Cursor::SCROLL_SE_X, std::make_unique<UI::Image>("art/intrface/scrsex.frm")));
    _ui.insert(std::make_pair(Cursor::HEXAGON_RED, std::make_unique<UI::Image>("art/intrface/msef000.frm")));
    _ui.insert(std::make_pair(Cursor::ACTION, std::make_unique<UI::Image>("art/intrface/actarrow.frm")));
    _ui.insert(std::make_pair(Cursor::HAND, std::make_unique<UI::Image>("art/intrface/hand.frm")));
    _ui.insert(std::make_pair(Cursor::SMALL_DOWN_ARROW, std::make_unique<UI::Image>("art/intrface/sdnarrow.frm")));
    _ui.insert(std::make_pair(Cursor::SMALL_UP_ARROW, std::make_unique<UI::Image>("art/intrface/suparrow.frm")));
    auto queue = std::make_unique<UI::AnimationQueue>();
    queue->animations().push_back(std::make_unique<UI::Animation>("art/intrface/wait.frm"));
    queue->setRepeat(true);
    queue->start();
    _ui.insert(std::make_pair(Cursor::WAIT, std::move(queue)));
}

Mouse::~Mouse()
{
    SDL_ShowCursor(1); // Show cursor
}

int Mouse::x() const
{
    return _position.x();
}

int Mouse::y() const
{
    return _position.y();
}

void Mouse::setX(int x)
{
    setPosition({x, _position.y()});
}

void Mouse::setY(int y)
{
    setPosition({_position.x(), y});
}

const Point& Mouse::position() const
{
    return _position;
}

void Mouse::setPosition(const Point& pos)
{
    _position = pos;
    auto renderer = Game::getInstance()->renderer();
    float scaleX = renderer->scaleX();
    float scaleY = renderer->scaleY();
    SDL_WarpMouseInWindow(renderer->sdlWindow(), (int)(pos.x() * scaleX), (int)(pos.y() * scaleY));
}

void Mouse::setState(Cursor state)
{
    _states.clear();
    pushState(state);
}

void Mouse::popState()
{
    if (_states.size() == 0) return;
    if (_states.size() == 1)
    {
        _setType(Cursor::NONE);
    }
    else
    {
        _setType(_states.at(_states.size() - 2));
    }
    _states.pop_back();
}

void Mouse::pushState(Cursor state)
{
    _setType(state);
    _states.push_back(state);
}

Mouse::Cursor Mouse::state() const
{
    return _states.empty() ? Cursor::NONE : _states.back();
}

void Mouse::_setType(Cursor state)
{
    if (this->state() == state || state == Cursor::NONE) return;
    auto cur = _ui.find(state)->second.get();
    auto csize = cur->size();
    Point p;

    if (state == Cursor::SCROLL_E || state == Cursor::SCROLL_E_X || state == Cursor::SCROLL_NE || state == Cursor::SCROLL_NE_X || state == Cursor::SCROLL_SE || state == Cursor::SCROLL_SE_X) {
        p.setX( -csize.x);
    }
    else if (state == Cursor::SCROLL_N || state == Cursor::SCROLL_N_X || state == Cursor::SCROLL_S || state == Cursor::SCROLL_S_X || state == Cursor::HEXAGON_RED || state == Cursor::WAIT) {
		p.setX(-csize.x / 2);
    }

    if (state == Cursor::SCROLL_S || state == Cursor::SCROLL_S_X || state == Cursor::SCROLL_SW || state == Cursor::SCROLL_SW_X || state == Cursor::SCROLL_SE || state == Cursor::SCROLL_SE_X) {
		p.setY( -csize.y);
    }
    else if (state == Cursor::SCROLL_W || state == Cursor::SCROLL_W_X || state == Cursor::SCROLL_E || state == Cursor::SCROLL_E_X || state == Cursor::HEXAGON_RED || state == Cursor::WAIT) {
		p.setY(-csize.y / 2);
    }

    if (state == Cursor::SMALL_DOWN_ARROW) {
		p.setX(-5);
		p.setY(-10);
    }
    else if (state == Cursor::SMALL_UP_ARROW) {
		p.setX(-5);
    }
    cur->setOffset(p);
}

void Mouse::render()
{
    if (state() == Cursor::NONE) return;

    if (state() != Cursor::HEXAGON_RED)
    {
        _ui.find(state())->second->setPosition(position());
    }
    _ui.find(state())->second->render();
}

void Mouse::think()
{
    SDL_GetMouseState(&_position.x, &_position.y);
    _position = Point(
        static_cast<int>(_position.x() / Game::getInstance()->renderer()->scaleX()),
        static_cast<int>(_position.y() / Game::getInstance()->renderer()->scaleY())
    );
    if (state() == Cursor::NONE)return;
    _ui.find(state())->second->think();
}

bool Mouse::scrollState()
{
    switch(this->state())
    {
        case Cursor::SCROLL_W:
        case Cursor::SCROLL_N:
        case Cursor::SCROLL_E:
        case Cursor::SCROLL_S:
        case Cursor::SCROLL_W_X:
        case Cursor::SCROLL_NW:
        case Cursor::SCROLL_NW_X:
        case Cursor::SCROLL_N_X:
        case Cursor::SCROLL_NE:
        case Cursor::SCROLL_NE_X:
        case Cursor::SCROLL_E_X:
        case Cursor::SCROLL_SE:
        case Cursor::SCROLL_SE_X:
        case Cursor::SCROLL_S_X:
        case Cursor::SCROLL_SW:
        case Cursor::SCROLL_SW_X:
            return true;
        default:
            return false;
    }
}

UI::Base* Mouse::ui()
{
    return _ui.get();
}

std::vector<Mouse::Cursor>* Mouse::states()
{
    return &_states;
}

unsigned Mouse::cursor() const
{
    return static_cast<unsigned>(state());
}

void Mouse::setCursor(unsigned value)
{
    if (value > static_cast<unsigned>(Cursor::HAND)) return;
    setState(static_cast<Cursor>(value));
}

void Mouse::renderOutline()
{
    if (state() == Cursor::NONE) return;

    auto vbase = _ui.find(state())->second.get();
    if (state() != Cursor::HEXAGON_RED) {
        vbase->setPosition(position());
    }
    vbase->setOutline(1);
    vbase->render();
    vbase->setOutline(0);
}
}
}
