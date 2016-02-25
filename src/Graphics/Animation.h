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

#include <iosfwd>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "Texture.h"
#include "../TransFlags.h"

namespace Falltergeist
{
namespace Graphics
{

class Animation
{
public:
    Animation(const std::string& filename);
    ~Animation();
    void render(int x, int y, unsigned int direction, unsigned int frame, bool transparency, bool light);
    bool opaque(unsigned int x, unsigned int y);
    void trans(Falltergeist::TransFlags::Trans _trans);
private:
    GLuint _vao;
    GLuint _coords;
    GLuint _texCoords;
    GLuint _ebo;
    Texture* _texture;
    int _stride;
    Falltergeist::TransFlags::Trans _trans = Falltergeist::TransFlags::Trans::NONE;

    GLint _uniformTex;
    GLint _uniformFade;
    GLint _uniformMVP;
    GLint _uniformCnt;
    GLint _uniformLight;
    GLint _uniformTrans;
    GLint _uniformOffset;

    GLint _attribPos;
    GLint _attribTex;
public:
};

}
}