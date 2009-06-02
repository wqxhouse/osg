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

#include <osgDB/DatabaseRevisions>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

using namespace osgDB;

////////////////////////////////////////////////////////////////////////////////////////////
//
// FilelList
//
FileList::FileList()
{
}

FileList::FileList(const FileList& fileList, const osg::CopyOp):
    _files(fileList._files)
{
}

FileList::~FileList()
{
}


////////////////////////////////////////////////////////////////////////////////////////////
//
// DatabaseRevision
//
DatabaseRevision::DatabaseRevision()
{
}

DatabaseRevision::DatabaseRevision(const DatabaseRevision& revision, const osg::CopyOp):
    _databasePath(revision._databasePath),
    _filesAdded(revision._filesAdded),
    _filesRemoved(revision._filesRemoved),
    _filesModified(revision._filesModified)
{
}

DatabaseRevision::~DatabaseRevision()
{
}

bool DatabaseRevision::isFileBlackListed(const std::string& filename) const
{
    osg::notify(osg::NOTICE)<<"DatabaseRevision("<<getName()<<")::isFileBlackListed("<<filename<<")"<<std::endl;

    if (_databasePath.length()>=filename.length()) return false;
    if (filename.compare(0,_databasePath.length(), _databasePath)!=0) return false;

    std::string localPath(filename,
                         _databasePath.empty() ? 0 : _databasePath.length()+1,
                         std::string::npos);

    osg::notify(osg::NOTICE)<<"    localPath = "<<localPath<<std::endl;

    return (_filesRemoved.valid() && _filesRemoved->contains(localPath)) ||
           (_filesModified.valid() && _filesModified->contains(localPath));
}


////////////////////////////////////////////////////////////////////////////////////////////
//
// DatabaseRevisions
//
DatabaseRevisions::DatabaseRevisions()
{
}

DatabaseRevisions::DatabaseRevisions(const DatabaseRevisions& revisions, const osg::CopyOp):
    _databasePath(revisions._databasePath),
    _revisionList(revisions._revisionList)
{
}

DatabaseRevisions::~DatabaseRevisions()
{
}

void DatabaseRevisions::addRevision(DatabaseRevision* revision)
{
    if (!revision) return;

    for(DatabaseRevisionList::iterator itr = _revisionList.begin();
        itr != _revisionList.end();
        ++itr)
    {
        if (*itr == revision) return;
        if ((*itr)->getName()==revision->getName())
        {
            (*itr) = revision;
            return;
        }
    }

    _revisionList.push_back(revision);
}

void DatabaseRevisions::removeRevision(DatabaseRevision* revision)
{
    for(DatabaseRevisionList::iterator itr = _revisionList.begin();
        itr != _revisionList.end();
        ++itr)
    {
        if (*itr == revision)
        {
            _revisionList.erase(itr);
            return;
        }
    }
}

bool DatabaseRevisions::isFileBlackListed(const std::string& filename) const
{
    for(DatabaseRevisionList::const_iterator itr = _revisionList.begin();
        itr != _revisionList.end();
        ++itr)
    {
        if ((*itr)->isFileBlackListed(filename))
        {
            osg::notify(osg::NOTICE)<<"File is black listed "<<filename<<std::endl;
            return true;
        }
    }
    return false;
}

