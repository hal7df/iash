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
#include <fstream>
#include <sstream>

#include <vector>
#include <map>
#include <algorithm>

#include <string>

#include <cstdlib>
#include <cctype>

#ifdef __unix__
    #include <unistd.h>
    #include <term.h>
    #include <sys/stat.h>
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
    vector<string> parseCmdLine(string raw);
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
