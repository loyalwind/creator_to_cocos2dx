/****************************************************************************
 Copyright (c) 2017 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "lua-bindings/creator_reader_bindings.hpp"

#include "scripting/lua-bindings/manual/CCLuaEngine.h"

#include "lua-bindings/reader/lua_creator_reader_auto.hpp"
#include "lua-bindings/reader/lua_creator_reader_manual.hpp"

#include "lua-bindings/dragonbones/lua_dragonbones_auto.hpp"
#include "lua-bindings/dragonbones/lua_dragonbones_manual.hpp"

int register_creator_reader_module(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    register_all_creator_reader(L);
    register_all_creator_reader_manual(L);
    
    register_all_creator_dragonbones(L);
    register_all_creator_dragonbones_manual(L);

    return 0;
}
