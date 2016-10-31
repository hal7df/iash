/* crosslib -- see crosslib.h for a full description.
 *
 * Copyright (C) 2014  Paul Bonnen
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "crosslib.h"
using namespace std;

#ifdef __unix

int CrossLib::mkdir(const char *pathname)
{
#ifdef __unix
    return ::mkdir(pathname,0755);
#elif __WIN32
    return _mkdir(pathname);
#endif
}

bool CrossLib::isdir(const char *pathname)
{
#ifdef __unix
    struct stat st;

    stat(pathname,&st);

    return S_ISDIR(st.st_mode);
#elif __WIN32
    return PathIsDirectory(pathname);
#endif
}

bool CrossLib::isfile(const char* pathname)
{
#ifdef __unix
    struct stat st;

    stat(pathname,&st);

    return S_ISREG(st.st_mode);
#elif __WIN32
    return PathFileExists(pathname);
#endif
}

std::string CrossLib::getHomeDir()
{
#ifdef __unix
    return std::string(getenv("HOME"));
#elif __WIN32
    return std::string(getenv("HOMEPATH"));
#endif
}

std::string CrossLib::getConfigDir()
{
#ifdef __unix
    return std::string(getenv("HOME"))+"/.config/";
#elif __WIN32
    OSVERSIONINFO version;

    if (version.dwMajorVersion >= 6)
        return (std::string)getenv("LOCALAPPDATA");
    else
        return (std::string)getenv("APPDATA");
#endif
}

std::string CrossLib::getWorkingDir()
{
#ifdef __unix
    char* buf;
    std::string path;

    buf = getcwd(NULL,0);

    path = std::string(buf);
    free(buf);

    return path;
#elif __WIN32
    char* buf;
    std::string path;

    buf = _getcwd(NULL,0);

    path = std::string(buf);
    free(buf);

#endif
}
