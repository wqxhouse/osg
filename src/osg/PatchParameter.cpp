/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/
#include <osg/PatchParameter>
#include <osg/State>
#include <osg/Notify>
#include <osg/io_utils>

using namespace osg;

PatchParameter::PatchParameter(GLint vertices):
    _vertices(vertices),
    _patchDefaultInnerLevel(1.0f,1.0f),
    _patchDefaultOuterLevel(1.0f,1.0f,1.0f,1.0f)
{
}


PatchParameter::~PatchParameter()
{
}

void PatchParameter::apply(State& state) const
{
    GL2Extensions* extensions = state.get<GL2Extensions>();
    if (extensions->areTessellationShadersSupported )
    {

        extensions->glPatchParameteri( GL_PATCH_VERTICES, _vertices );
        extensions->glPatchParameterfv( GL_PATCH_DEFAULT_INNER_LEVEL, _patchDefaultInnerLevel.ptr() );
        extensions->glPatchParameterfv( GL_PATCH_DEFAULT_OUTER_LEVEL, _patchDefaultOuterLevel.ptr() );
    }
}
