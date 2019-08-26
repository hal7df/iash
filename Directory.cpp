/*
 * Directory.cpp
 *
 *  Created on: Oct 30, 2016
 *      Author: paul
 */

#include "Directory.h"

#include <cassert>
#include <cctype>
#include <iostream>

#ifdef __unix
    #include <sys/stat.h>
    #include <unistd.h>
#elif __WIN32
	#include <windows.h>
	#include <direct.h>
        #include <cstring>
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
		m_dirpath = toIash(dir);
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
	string newPath = resolvePath(relPath);

	return changeDirAbs(newPath);
}

bool Directory::changeDirAbs (const std::string &absPath)
{
	if (isDir(absPath.c_str()))
	{
		m_dirpath = toIash(absPath);
		ensureTrailingSlash();
		return true;
	}
	else return false;
}

int Directory::mkSubdir (const string &dirName) const
{
	return mkdir(resolvePath(dirName).c_str());
}

bool Directory::dirExists (const string &dirName) const
{
	return Directory::isDir(resolvePath(dirName).c_str());
}

bool Directory::fileExists (const string &filename) const
{
	return Directory::isFile(resolvePath(filename).c_str());
}

string Directory::toIash (const string &path)
{
#ifdef __WIN32
	string newPath = path;
	unsigned backslashPos;

	while ((backslashPos = newPath.find('\\')) != static_cast<unsigned>(string::npos))
	{
		newPath.replace(backslashPos, 1, "/");
	}

	return newPath;
#else
        return path;
#endif
}

string Directory::toPlatform (const string &path)
{
#ifdef __WIN32
	string newPath = path;
	unsigned slashPos;

	while ((slashPos = newPath.find('/')) != static_cast<unsigned>(string::npos))
	{
		newPath.replace(slashPos, 1, "\\");
	}

	return newPath;
#else
        return path;
#endif
}

string Directory::resolvePath (const string &relPath) const
{
	string curPath;
	string curPathEl;
	unsigned firstSearchPos = 0;
	unsigned curSearchPos;

	if (relPath[0] == '/')
	{
#ifdef __unix
		curPath = "/";
		firstSearchPos = 1;
#elif __WIN32
                //Get drive letter of current directory
                curPath = m_dirpath.substr(0, 3);
                firstSearchPos = 1;
#endif
	}
#ifdef __WIN32
        else if (relPath.size() >= 2 && isalpha(relPath[0]) && relPath[1] == ':')
        {
            curPath = relPath.substr(0, 3);
            firstSearchPos = 3;
        }
#endif
	else curPath = m_dirpath;

	while ((curSearchPos = relPath.find('/', firstSearchPos)) != static_cast<unsigned>(string::npos))
	{
		curPathEl = relPath.substr(firstSearchPos, (curSearchPos - firstSearchPos));

#ifdef __unix
		if (curPathEl == ".." && curPath.size() > 1)
#elif  __WIN32
                if (curPathEl == ".." && curPath.size() > 3)
#endif
                {
                    curPath = curPath.substr(0, (curPath.find('/', curPath.length() - 1)));
                }
		else if (curPathEl != "." && curPathEl != "..")
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
	return ::mkdir(pathname ,0755);
#elif __WIN32
	return _mkdir(toPlatform(pathname));
#endif
}

bool Directory::isDir (const char *pathname)
{
#ifdef __unix
	struct stat st;

	stat(pathname, &st);

	return S_ISDIR(st.st_mode);
#elif __WIN32
        DWORD fAttrib = GetFileAttributes(toPlatform(pathname).c_str());

        return (fAttrib != INVALID_FILE_ATTRIBUTES && (fAttrib & FILE_ATTRIBUTE_DIRECTORY));
#endif
}

bool Directory::isFile (const char *pathname)
{
#ifdef __unix
    struct stat st;

    stat(pathname, &st);

    return S_ISREG(st.st_mode);
#elif __WIN32
        DWORD fAttrib = GetFileAttributes(toPlatform(pathname).c_str());

        return (fAttrib != INVALID_FILE_ATTRIBUTES && !(fAttrib & FILE_ATTRIBUTE_DIRECTORY));
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

    memset(&version, 0, sizeof(OSVERSIONINFO));
    version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&version);

    if (version.dwMajorVersion >= 6)
        return Directory(string(getenv("LOCALAPPDATA")));
    else
        return Directory(string(getenv("APPDATA")));
#endif
}

Directory Directory::getWorkingDir()
{
	char* buf;
	string path;

#ifdef __unix
    buf = getcwd(NULL,0);
#elif __WIN32
    buf = _getcwd(NULL,0);
#endif

    path = string(buf);
    free(buf);

    return Directory(path);
}

FileNotFoundException::FileNotFoundException (const string &fname)
	: runtime_error(fname + ": file not found") {}
