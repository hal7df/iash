/*
 * Directory.cpp
 *
 *  Created on: Oct 30, 2016
 *      Author: paul
 */

#include "Directory.h"
#include <cassert>

#ifdef __unix
    #include <sys/stat.h>
    #include <unistd.h>
#elif __WIN32
	#include <windows.h>
	#include <direct.h>
#endif

using namespace std;

Directory::Directory ()
{
	*this = getWorkingDir();
}

Directory::Directory (const string &dir)
{
	assert(isDir(dir.c_str()));

	if (isDir(dir.c_str()))
	{
		m_dirpath = dir;
		m_valid = true;
	}
	else m_valid = false;

	ensureTrailingSlash();
}

Directory::~Directory () {}

bool Directory::isValid () const
{
	return m_valid;
}

const string& Directory::getAbsPath () const
{
	return m_dirpath;
}

bool Directory::changeDirRel (const std::string &relPath)
{
	string newPath = computeRelative(relPath);

	return changeDirAbs(newPath);
}

bool Directory::changeDirAbs (const std::string &absPath)
{
	if (isDir(absPath.c_str()))
	{
		m_dirpath = absPath;
		ensureTrailingSlash();
		return true;
	}
	else return false;
}

const char* Directory::getPathToFileInDirectory (const string &filename) const
{
	return (m_dirpath + filename).c_str();
}

int Directory::mkSubdir (const string &dirName) const
{
	return mkdir((m_dirpath + dirName).c_str());
}

string Directory::computeRelative (const string &relPath) const
{
	string curPath;
	string curPathEl;
	unsigned firstSearchPos = 0;
	unsigned curSearchPos;

	if (relPath[0] == '/')
	{
		curPath = "/";
		firstSearchPos = 1;
	}
	else curPath = m_dirpath;

	while ((curSearchPos = relPath.find('/', firstSearchPos)) != string::npos)
	{
		curPathEl = relPath.substr(firstSearchPos, (curSearchPos - firstSearchPos));

		if (curPathEl == "..") curPath = curPath.substr(0, (curPath.find('/', curPath.length() - 1)));
		else if (curPathEl != ".")
		{
			curPath += (curPathEl + '/');
		}

		firstSearchPos = curSearchPos + 1;
	}

	return curPath;
}

void Directory::ensureTrailingSlash ()
{
	if (m_dirpath.back() != '/') m_dirpath += '/';
}

int Directory::mkdir (const char *pathname)
{
#ifdef __unix
	return ::mkdir(pathname,0755);
#elif __WIN32
	return _mkdir(pathname);
#endif
}

bool Directory::isDir (const char *pathname)
{
#ifdef __unix
	struct stat st;

	stat(pathname, &st);

	return S_ISDIR(st.st_mode);
#elif __WIN32
	return PathIsDirectory(pathname);
#endif
}

bool Directory::isFile (const char *pathname)
{
#ifdef __unix
    struct stat st;

    stat(pathname, &st);

    return S_ISREG(st.st_mode);
#elif __WIN32
    return PathFileExists(pathname);
#endif
}

Directory Directory::getHomeDir()
{
#ifdef __unix
    return Directory(string(getenv("HOME")));
#elif __WIN32
    return Directory(string(getenv("HOMEPATH")));
#endif
}

Directory Directory::getConfigDir()
{
#ifdef __unix
    return Directory(string(getenv("HOME")) + "/.config/");
#elif __WIN32
    OSVERSIONINFO version;

    if (version.dwMajorVersion >= 6)
        return Directory(string(getenv("LOCALAPPDATA")));
    else
        return Directory(string(getenv("APPDATA")));
#endif
}

Directory Directory::getWorkingDir()
{
#ifdef __unix
    char* buf;
    string path;

#ifdef __unix
    buf = getcwd(NULL,0);
#elif __WIN32
    buf = _getcwd(NULL,0);
#endif

    path = string(buf);
    free(buf);

    return path;
}
