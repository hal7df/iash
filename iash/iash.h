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
#include <string>
#include <limits>

using namespace std;

typedef struct {
    string option;
    string argument;
} option_t;

class iash
{
public:
    iash() {}
    ~iash() {}

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
     * @return: all of the options and the arguments from those options
     */
    vector<option_t> getOptions();

    /**
     * @brief cmdNotFound: Prints a command not found message and clears the
     *        command buffer.
     */
    void cmdNotFound();

    /**
     * @brief clear: Clear the command buffer.
     */
    void clear();
private:
    void debugConsole (vector<string> cmd);

    vector<string> m_cmdLine;
};

#endif //IASH_H
