/*
 * Command.h
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#ifndef IASH_COMMAND_H_
#define IASH_COMMAND_H_

#include "iash.h"
#include "UserCommand.h"
#include <iostream>
#include <string>
#include <vector>

/**
 * Abstract interface for a command registered with the shell.
 * <p>
 * All commands in an iash application are descendants of Command. When a user
 * issues a command at the iash prompt, iash searches its registry for a
 * command with the same name as the command the user entered. If it finds one,
 * the shell executes that Command's @link Command::run @endlink function. The
 * executed Command then has access to the calling shell through the `m_parent`
 * protected member variable.
 * <p>
 * Command provides a number of extra functions to aid in handling improper
 * input, in addition to providing a mechanism for iash to recognize multiple
 * command names for the same Command (@link Command::getAliases @endlink and
 * @link Command::getAliasMapping @endlink), however these do not need to be
 * implemented by child classes of Command. The only functions that children
 * must have are:
 * <ul>
 * 	<li>@link Command::getName @endlink</li>
 * 	<li>@link Command::run @endlink</li>
 * </ul>
 * These functions are required for iash to be able to properly recognize and
 * execute commands.
 * <p>
 * Internally, iash stores instances of each registered Command. As such,
 * Command subclasses can have additional member variables not specified by the
 * Command interface.
 * <p>
 * Note that, unlike bash and other popular system shells, a Command's name
 * takes precedence over any aliases that any commands have configured. So, for
 * example, if there is a command with name `ls`, and another command has an
 * `ls=runKeylogger` alias configured, iash will still run `ls`. As the example
 * indicates, this is done for security reasons. As of v0.5, there is
 * currently no API for managing command defaults; Commands should be configured
 * to run in their default configuration when they are invoked by the user with
 * no arguments.
 *
 * @since 0.5
 */
class Command {
public:
	//INITIALIZATION ***********************************************************
	/**
	 * Creates a new Comamnd instance.
	 * <p>
	 * Note that this does NOT fully initialize a Command instance, the parent
	 * shell is only set upon registration of the Command with the shell.
	 * Assuming that no Command functions are called prior to Command
	 * registration, it is guaranteed that m_parent will point to the shell
	 * that owns the Command.
	 */
	Command();
	virtual ~Command();

	/**
	 * Sets the shell parent for this Command. There is no need to override
	 * this function (and is thus not virtual).
	 *
	 * @param parent	a pointer to the iash shell to which this Command is
	 * 					registered
	 */
	void init (iash *parent);

	//COMMAND INFORMATION RETRIEVAL ********************************************
	/**
	 * Returns the name of this Command to be used in iash.
	 * <p>
	 * This field is the first field iash looks at when determining which
	 * Command to execute (if any) when a user inputs a command.
	 *
	 * @return	the name of this Command
	 */
	virtual std::string getName () const = 0;

	/**
	 * Returns all builtin aliases for this Command. Aliases are used to invoke
	 * commands after a user-entered command does not come up with any matches
	 * in the main name registry. For the moment, iash only supports basic
	 * aliasing, although a future revision will include the ability to match
	 * a certain pattern, such as `d?=d -n $1`, where wildcards can be used
	 * to preserve information in the alias.
	 *
	 * @return	a vector containing all of the builtin aliases for this Command.
	 * 			By default, this returns an empty vector.
	 */
	virtual std::vector<std::string> getAliases () const;

	/**
	 * Converts a given alias for this command into a fully-qualified command
	 * string (using the Command's default name). For the moment, this function
	 * is unused by iash, but when dynamic aliasing is supported (i.e.
	 * wildcards), this will be used to handle dynamic aliases.
	 *
	 * @param alias	the alias that was entered by the user
	 * @return		a string to replace the aliased text in the original
	 *				command. This should begin with the Command's name.
	 */
	virtual std::string getAliasMapping (const std::string alias) const;

	/**
	 * Called when a command matching the current Command is issued.
	 * <p>
	 * This is conceptually equivalent to the `main` function of a C/C++
	 * program: this is where execution begins when a Command is called by the
	 * user. All of the information regarding the command issued by the user is
	 * stored in the object pointed to by cmd.
	 * <p>
	 * Note that, unlike a normal C++ program, the client SHOULD NOT directly
	 * use cin or cout, but rather print to and read from the provided ostream
	 * and istream references in the UserCommand passed into the function. This
	 * allows the user to make use of iash's I/O redirection capabilities. For
	 * the purposes of this function, the istream and ostream references in
	 * the provided UserCommand are stdin and stdout.
	 * <p>
	 * Return values of this function should respect <a
	 * href="http://www.tldp.org/LDP/abs/html/exitcodes.html">bash reserved
	 * exit statuses</a>. As of right now, the only exit code used by iash is
	 * 127 (to indicate command not found), but any time a function returns a
	 * reserved return value, iash will override it to 1.
	 *
	 * @param cmd	a pointer to the UserCommand containing command arguments
	 * 				and stdin/stdout for this command execution
	 * @return		an exit status
	 */
	virtual int run (UserCommand *cmd) = 0;

	/**
	 * Prints a usage message to the specified output stream. This function is
	 * called automatically by iash if a prior call to @link Command::validate
	 * @endlink fails. The return value for this Command call is then set to 1.
	 * <p>
	 * By default, this prints "@link Command:getName @endlink: invalid use of
	 * command".
	 *
	 * @param os	the stream to print to
	 */
	virtual void showUsageMessage (std::ostream &os) const;

	/**
	 * Validates the command entered by the user. This can be used to ensure
	 * that the command arguments are valid before @link Command::run @endlink
	 * is called by iash. By default, this always returns true.
	 * <p>
	 * DO NOT print any text to either cout or the provided output stream in
	 * cmd; usage messages will be handled automatically by iash through use of
	 * the @link Command::showUsageMessage @endlink function.
	 *
	 * @param cmd	a pointer to the UserCommand containing
	 * @return		if the command is valid or not.
	 */
	virtual bool validate (UserCommand *cmd) const;
protected:
	iash *m_parent;
};

#endif /* IASH_COMMAND_H_ */
