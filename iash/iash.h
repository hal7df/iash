/* iash -- Internal Applicaiton SHell
 *
 * This is a library designed to provide shell functionality
 * within an application.
 *
 * License: GNU GPLv3
 * (I'll add the full license disclaimer later.)
 */

#ifndef IASH_H
#define IASH_H

#define IASH_VERSION_MAJOR 0
#define IASH_VERSION_MINOR 2
#define IASH_VERSION_PATCH 1

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <map>
#include <algorithm>

#include <string>

#include <cstdlib>
#include <cctype>

#include "crosslib.h"

using namespace std;

class iash
{
public:
    iash(string app_nm="iash", bool useNameInPrompt=true);
    ~iash();

    // CONFIGURATION **********************************************************

    /**
     * @brief setAppName: Set the application name. This can be used
     *                    on the prompt.
     * @param app_nm: The name of the application.
     */
    void setAppName (string app_nm);

    /**
     * @brief useAppNameInPrompt: Show the app name in the command
     *                            prompt.
     * @param name: Whether or not to use the app name in the command
     *              prompt.
     */
    void useAppNameInPrompt (bool name=true);

    // COMMAND LINE ***********************************************************

    /**
     * @brief getCmdLine: Clear the current shell contents and prompt a
     *        new command line.
     * @return: a vector containing all of the options and
     *          arguments, including the command.
     */
    vector<string> getCmdLine();

    /**
     * @brief getLast: Get the last command line.
     * @return: the vector containing the last command line.
     */
    vector<string> getLast() { return m_cmdLine; }

    /**
     * @brief getOptions: Get the options from the last getCmdLine() call
     * @return: all of the options passed at the commandline
     */
    vector<string> getOptions() { return getOptions(m_cmdLine); }

    /**
     * @brief clear: Clear the command buffer.
     */
    void clear() { m_cmdLine.clear(); }

    // ENVIRONMENT SYSTEM *****************************************************

    /**
     * @brief setEnv: Set internal environment variables
     * @param name: The name of the variable
     * @param value: The value of the variable
     */
    void setEnv (string name, string value);
    void setEnv (string name, int value) { setEnv(name,to_string(value)); }
    void setEnv (string name, float value) { setEnv(name,to_string(value)); }
    void setEnv (string name, bool value);

    /**
     * @brief getEnv: Get internal environment variables
     * @param name: The name of the variable
     * @return: The variable
     */
    string getEnv_string (string name);
    int getEnv_int (string name) { return stoi(getEnv_string(name)); }
    float getEnv_float (string name) { return stof(getEnv_string(name)); }
    bool getEnv_bool (string name);

    /**
     * @brief rmenv: Remove internal environment variables
     * @param name: The name of the variable to remove
     */
    void rmenv (string name);

    /**
     * @brief saveEnv: Save the environment to file.
     * @param filepath: The file to save to. If no file is passed,
     *          the default path at $IASH_CONFIG_PATH/iashenv is used.
     * @return: true on success, false on failure
     */
    bool saveEnv ();
    bool saveEnv (string filepath);

    /**
     * @brief loadEnv: Loads the environment from file.
     * @param filepath: The file to load from. If no path is passed,
     *          the default path at $IASH_CONFIG_PATH/iashenv is used.
     * @return: true on success, false on failure
     */
    bool loadEnv ();
    bool loadEnv (string filepath);

    // DIRECTORY SYSTEM *******************************************************

    /**
     * @brief changeDir: Changes the current working directory (within iash)
     *          based on path. It automatically determines if the path is
     *          relative or absolute.
     * @param path: The path to change to directory to.
     * @return: true on succes, false on failure (path doesn't exist)
     */
    bool changeDir (string path);

    /**
     * @brief getDir: Gets the current working directory as maintained by iash.
     * @return: The current working directory.
     */
    string getDir () { return getEnv_string("IASH_CWD"); }

    /**
     * @brief fileDelta: As iash changes directories internally, opening files
     *          as if they were in the current directory will not work. Use
     *          this function to get the actual path to the file.
     * @param origPath: The path of the file as given relatively to the shell's
     *          current working directory.
     * @return: the absolute path to the file.
     */

    string fileDelta (string origPath);

    // MISCELLANEOUS **********************************************************

    /**
     * @brief cmdNotFound: Prints a command not found message and clears the
     *        command buffer.
     */
    void cmdNotFound();

    /**
     * @brief clearScreen: Clear the screen.
     */
    void clearScreen() { CrossLib::clearScreen(); }

private:
    vector<string> getOptions(vector<string> cmd);
    vector<string> parseCmdLine(string raw);

    void debugConsole (vector<string> cmd);
    void debugConsole ();
    void cmdNotFound_dbg(string cmd);

    bool doesEnvVarExist (string name);
    string convEnv (string name);
    void updateAttached();

    vector<string> m_cmdLine;
    map<string,string> m_env;
    string m_appName;
    bool f_useAppNameInPrompt;
};

#endif //IASH_H
