#ifndef CROSSLIB_H
#define CROSSLIB_H

#include <string>
#include <cstdlib>

#ifdef __unix
    #include <sys/stat.h>
    #include <term.h>
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
    #include <direct.h>
#endif

namespace CrossLib
{
    //TERMINAL FUNCTIONS *************

    /**
     * @brief clearScreen: Clears the terminal screen.
     */
    void clearScreen();

    //FILESYSTEM ACCESS **************

    /**
     * @brief mkdir: Create directory.
     * @param pathname: Path to the directory to create.
     * @return: 0 on success, -1 on failure.
     */
    int mkdir (const char* pathname);

    /**
     * @brief isdir: Checks to see if the path is a directory.
     * @param pathname: Path of the directory to check.
     * @return: Whether the path is a directory.
     */
    bool isdir (const char* pathname);

    /**
     * @brief isfile: Checks to see if the path is a file.
     * @param pathname: Path of the file to check.
     * @return: Whether the path is a file.
     */
    bool isfile (const char* pathname);

    /**
     * @brief getConfigDir: Gets the directory where application
     *          configuration is stored
     * @return: The absolute path to where the application configuration
     *          is stored.
     */
    std::string getConfigDir ();

    /**
     * @brief getWorkingDir: Get the current working directory.
     * @return : The current working directory.
     */
    std::string getWorkingDir ();
}

#endif // CROSSLIB_H
