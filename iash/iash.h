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

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <limits>
#include <cstdlib>
#include <cctype>

#ifdef __unix__
    #include <unistd.h>
    #include <term.h>
#elif _WIN32
    #include <windows.h>
#endif

using namespace std;

class iash
{
public:
    iash(string app_nm="iash", bool useNameInPrompt=true);
    ~iash() {}

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
     * @brief setenv: Set internal environment variables
     * @param name: The name of the variable
     * @param value: The value of the variable
     */
    void setenv (string name, string value);
    void setenv (string name, int value) { setenv(name,to_string(value)); }
    void setenv (string name, float value) { setenv(name,to_string(value)); }
    void setenv (string name, bool value);

    /**
     * @brief getenv: Get internal environment variables
     * @param name: The name of the variable
     * @return: The variable
     */
    string getenv_string (string name) { return m_env.at(name); }
    int getenv_int (string name) { return stoi(m_env.at(name)); }
    float getenv_float (string name) { return stof(m_env.at(name)); }
    bool getenv_bool (string name);

    /**
     * @brief rmenv: Remove internal environment variables
     * @param name: The name of the variable to remove
     */
    void rmenv (string name);

    /**
     * @brief cmdNotFound: Prints a command not found message and clears the
     *        command buffer.
     */
    void cmdNotFound();

    /**
     * @brief clear: Clear the command buffer.
     */
    void clear() { m_cmdLine.clear(); }

    /**
     * @brief clearScreen: Clear the screen.
     */
    void clearScreen();
private:
    vector<string> getOptions(vector<string> cmd);
    void debugConsole (vector<string> cmd);
    void debugConsole ();
    void cmdNotFound_dbg();
    void updateAttached();

    vector<string> m_cmdLine;
    map<string,string> m_env;
    string m_appName;
    bool f_useAppNameInPrompt;
};

#endif //IASH_H
