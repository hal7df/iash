/*
 * UserCommand.h
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#ifndef IASH_USERCOMMAND_H_
#define IASH_USERCOMMAND_H_

#include <iostream>
#include <string>
#include <vector>
#include "tools/Token.h"

/**
 * Used to retain command arguments and the proper input and output streams for
 * a command call.
 * <p>
 * Note that, with the exception of @link UserCommand::getWholeCommand @endlink,
 * all of the functions that retrieve options or arguments from the user's
 * command input iterate over the internal vector used to store it, and thus
 * may slow down your program if they are called too often.
 *
 * @since 0.5
 */
class UserCommand {
public:
	/**
	 * Constructs a new UserCommand object with the given command, input, and
	 * output. This also invokes the command parser, parsing the user's command
	 * into its components. (This does not parse out the command's options).
	 *
	 * @param inputCommand	the command the user input
	 * @param stdin			the stream to use as stdin for the command
	 * @param stdout		the stream to use as stdout for the command
	 */
	UserCommand (std::string inputCommand, std::istream &stdin,
				std::ostream *stdout);

	/**
	 * Constructs a new UserCommand object with the given command, input, and
	 * output. This also invokes the command parser, parsing the user's command
	 * into its components. (This does not parse out the command's options).
	 *
	 * @param inputCommand	the command the user input as a Token
	 * @param stdin			the stream to use as stdin for the command
	 * @param stdout		the stream to use as stdout for the command
	 */
	UserCommand (Token inputCommand, std::istream &stdin, std::ostream &stdout);
	virtual ~UserCommand();

	//COMMAND INFORMATION RETRIEVAL ********************************************

	/**
	 * Returns the entire command (including the command name), parsed into
	 * arguments as a vector.
	 *
	 * @return	the entire command, including the command name, as a vector
	 */
	const std::vector<std::string>& getWholeCommand () const;

	/**
	 * Returns all arguments passed by the user that are not options/switches
	 * (arguments identified with a `-` or `--` at the beginning).
	 *
	 * @return	all non-option arguments
	 */
	std::vector<std::string> getArguments () const;

	/**
	 * Returns all of the options/flags that the user specified (arguments with
	 * a leading `-` or `--`).
	 * <p>
	 * Note that iash option syntax allows users to chain single-letter options
	 * together (like `-abcde`), and will report all of the characters present
	 * after a single minus as individual options (so, in this example, `a`,
	 * `b`, `c`, `d`, `e` will all be reported as options). If you want iash to
	 * report whole words as options, have users enter them with a double-minus
	 * prefix (so `--abcde` will be reported as `abcde`).
	 *
	 * @return	all options/flags
	 */
	std::vector<std::string> getOptions () const;

	/**
	 * Finds the argument following the specified single-letter option or flag.
	 * If the specified option does not have an argument that follows it, or the
	 * option is embedded in a chained single-letter argument and is not at the
	 * end (for example, `c` in `-abcde`), then this will return an empty
	 * string.
	 * <p>
	 * Note that this does not guarantee that the returned argument is actually
	 * specifying a parameter for the given option. In order to insure this,
	 * Command::validate should be overridden to check that the command
	 * structure is valid.
	 *
	 * @param opt	the single-letter option to find a contextual argument for
	 * @return		the argument following the option, or a blank string if one
	 * 				could not be found
	 */
	std::string getContextualArgument (char opt) const;

	/**
	 * Finds the argument following the specified multi-character option or
	 * flag. If the specified option dooes not have an argument that follows it,
	 * then this will return an empty string.
	 * <p>
	 * Note that this does not guarantee that the returned argument is actually
	 * specifying a parameter for the given option. In order to insure this,
	 * Command::validate should be overridden to check that the command
	 * structure is valid.
	 *
	 * @param opt	a multi-character option to find a contextual argument for
	 * @return		the argument following the option, or a blank string if one
	 * 				could not be found
	 */
	std::string getContextualArgument (std::string opt) const;

	/**
	 * Gets a reference to the input stream to be used as stdin for this command
	 * invocation.
	 *
	 * @return	a reference to the input stream for this command
	 */
	std::istream& getStdin () const;

	/**
	 * Gets a reference to the output stream to be used as stdout for this
	 * command invocation.
	 *
	 * @return	a reference to the output stream for this command
	 */
	std::ostream& getStdout () const;
private:
	std::istream &m_stdin;
	std::ostream &m_stdout;
	std::vector<std::string> m_commandParts;
	std::string m_raw;
};

#endif /* IASH_USERCOMMAND_H_ */
