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

#ifdef __unix

void CrossLib::clearScreen()
{
    if (!cur_term)
    {
        int result;
        setupterm(NULL, STDOUT_FILENO, &result);
        if (result <= 0)
        return;
    }

    putp(tigetstr("clear"));
}

int CrossLib::mkdir(const char *pathname)
{
    return ::mkdir(pathname,0755);
}

bool CrossLib::isdir(const char *pathname)
{
    struct stat st;

    stat(pathname,&st);

    return S_ISDIR(st.st_mode);
}

bool CrossLib::isfile(const char* pathname)
{
    struct stat st;

    stat(pathname,&st);

    return S_ISREG(st.st_mode);
}

std::string CrossLib::getHomeDir()
{
    return std::string(getenv("HOME"));
}

std::string CrossLib::getConfigDir()
{
    return std::string(getenv("HOME"))+"/.config/";
}

std::string CrossLib::getWorkingDir()
{
    char* buf;
    std::string path;

    buf = getcwd(NULL,0);

    path = std::string(buf);
    free(buf);

    return path;
}

#elif _WIN32

void CrossLib::clearScreen()
{
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
      hStdOut,
      (TCHAR) ' ',
      cellCount,
      homeCoords,
      &count
      )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
      hStdOut,
      csbi.wAttributes,
      cellCount,
      homeCoords,
      &count
      )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition( hStdOut, homeCoords );
}

int CrossLib::mkdir(const char *pathname)
{
    return _mkdir(pathname);
}

bool CrossLib::isdir(const char *pathname)
{
    return PathIsDirectory(pathname);
}

bool CrossLib::isfile(const char *pathname)
{
    return PathFileExists(pathname);
}

std::string CrossLib::getHomeDir()
{
    return std::string(getenv("HOMEPATH"));
}

std::string CrossLib::getConfigDir()
{
    OSVERSIONINFO version;

    if (version.dwMajorVersion >= 6)
        return (std::string)getenv("LOCALAPPDATA");
    else
        return (std::string)getenv("APPDATA");
}

std::string CrossLib::getWorkingDir()
{
    char* buf;
    std::string path;

    buf = _getcwd(NULL,0);

    path = std::string(buf);
    free(buf);

    return path;
}

#endif
