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

std::string CrossLib::getConfigDir()
{
    return std::string(getenv("HOME"))+"/.config/";
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
    if (CreateDirectory(pathname) == 0)
        return -1;
    else
        return 0;
}

bool CrossLib::isdir(const char *pathname)
{
    return PathIsDirectory(pathname);
}

bool CrossLib::isfile(const char *pathname)
{
    return PathFileExists(pathname);
}

std::string CrossLib::getConfigDir()
{
    OSVERSIONINFO version;

    if (version.dwMajorVersion >= 6)
        return (std::string)getenv("LOCALAPPDATA");
    else
        return (std::string)getenv("APPDATA");
}

#endif
