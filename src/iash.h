/* iash -- Internal Applicaiton SHell
 *
 * This is a library designed to provide shell functionality
 * within a terminal application.
 *
 * License: GNU GPLv3
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

#ifndef IASH_H
#define IASH_H

#define IASH_VERSION_MAJOR 0
#define IASH_VERSION_MINOR 5
#define IASH_VERSION_PATCH 0

#include <iostream>
#include <string>

#include "Environment.h"
#include "CommandDispatcher.h"
#include "Directory.h"

class Command;
class UserCommand;

/**
 * The main shell class for iash.
 * <p>
 * This class provides an interface to register commands with the shell, and
 * then execute those commands in a number of ways. It also provides an
 * interface for commands to inspect (and, to an extent, modify) the current
 * state of the shell.
 * <p>
 * While this class has always existed, its interface was completely rewritten
 * in v0.5. Versions that are greater than or equal to v0.5 are not backwards-
 * compatible with prior versions.
 *
 * @since 0.5
 */
class iash
{
public:
	/**
	 * Creates an iash shell object with the given application name (for use in
	 * the prompt).
	 *
	 * @param appName	the name of the application, to be shown in prompts
	 * 					and to check for in scripts
	 */
    iash(const std::string &appName="iash");

    //IASH INTERNAL OBJECT RETRIEVAL *******************************************

    /**
     * Gets the current Environment object for this shell instance.
     *
     * @return	a pointer to the current Environment for this shell
     */
    Environment* getEnv ();

    /**
     * Gets the current working directory for this shell. Note that this
     * is NOT the same as the program's current working directory; this
     * is never changed by iash. Internally, iash stores a reference to another
     * directory that is specified by the current user to work in.
     *
     * @return	a pointer to a Directory object representing the shell's
     * 			current working directory.
     */
    const Directory* getCwd () const;

    //COMMAND REGISTRATION INTERFACE *******************************************

    /**
     * Adds a Command instance to this shell's CommandDispatcher registry. This
     * function is to be used by directly injecting dynamically allocated
     * Command object pointers into the iash class (iash will handle the
     * dynamic memory internally), for example:
     * ~~~{cpp}
     * iash shell ("myapp");
     * shell.addCommand(new FooCommand());
     * ~~~
     * This should not be called more than once per each unique command; the
     * CommandDispatcher will refuse to add the repeated command.
     *
     * @param cmd	a pointer to a dynamically allocated Command object
     */
    void addCommand (Command* cmd);

    //SHELL INVOCATION *********************************************************
    /**
     * Runs the shell in interactive mode. This will print out a command
     * prompt to stdout and read the user's commands in via stdin.
     * <p>
     * When running iash in interactive mode, this should be called directly in
     * the return statement for main, i.e.:
     * ~~~{cpp}
     * return shell.runInteractive();
     * ~~~
     *
     * @return	the exit status of the shell. Presently, this will always be
     * 0.
     */
    int runInteractive ();

    /**
     * Executes the commands in the specified script and quits. Command I/O
     * will be done from/to stdin/stdout unless otherwise specified by the
     * script.
     * <p>
     * iash scripts are identified by having this line at the top of the file:
     * ~~~
     * #!iash@appname
     * ~~~
     * where `appname` is the configured name of the application. If the
     * `@appname` specifier is missing, by default iash will refuse to run
     * the script.
     *
     * @param fname the filename (absolute or relative to the application's
     * 				current working directory) of the script to run.
     * @return		the exit status of the last command to execute.
     */
    int runScript (const char *fname);

    /**
     * Executes the specified command and quits. This does not support I/O
     * redirection; this will do all input and output from/to stdin/stdout.
     * To run a command with I/O redirection, use exec(UserCommand*).
     *
     * @param cmd	a string containing the command to execute
     * @return		the exit status of the command
     */
    int exec (std::string &cmd);

    /**
     * Executes the specified, already parsed command and quits. Command I/O is
     * done to/from the specified streams in the UserCommand object.
     *
     * @param cmd	a UserCommand to execute
     * @return		the exit status of the command
     */
    int exec (UserCommand *cmd);
private:
    int run (std::istream& cmdin, bool showPrompt=false);

    CommandDispatcher m_dispatcher;
    Environment m_env;
    Directory m_iashCwd;
    std::string m_appName;
};

#endif //IASH_H
