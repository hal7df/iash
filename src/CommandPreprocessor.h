/*
 * CommandPreprocessor.h
 *
 *  Created on: Nov 21, 2016
 *      Author: paul
 */

#ifndef SRC_COMMANDPREPROCESSOR_H_
#define SRC_COMMANDPREPROCESSOR_H_

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <exception>

#include "../src/iash.h"
#include "../src/UserCommand.h"

/**
 * Processes raw input for iash and converts the input into UserCommands that
 * can then be run by the CommandDispatcher. There is no need to instantiate
 * this manually, it is used internally by iash.
 *
 * @since 0.5
 */
class CommandPreprocessor {
public:
	/**
	 * Creates a CommandPreprocessor for the given shell.
	 *
	 * @param parent	the shell that this CommandPreprocessor is attached to
	 */
	CommandPreprocessor(iash *parent);
	virtual ~CommandPreprocessor();

	/**
	 * Takes the raw input string and converts it into a vector of UserCommands
	 * that can be passed to the CommandDispatcher. Note that this will
	 * automatically call cleanup().
	 *
	 * @param raw	the raw command string
	 * @return		a reference to a vector of the UserCommands contained in
	 * 				the raw command string.
	 */
	std::vector<UserCommand>& process (const std::string& raw);

	/**
	 * Gets the UserCommands that were parsed from the last call to process.
	 *
	 * @return	a reference to a vector of the UserCommands contained in the
	 * 			raw command string.
	 */
	std::vector<UserCommand>& getCommands () const;

	/**
	 * Cleans up stream objects that were opened on the last call to process.
	 * <p>
	 * In order to implement I/O redirection, CommandPreprocessor dynamically
	 * opens I/O streams when the user wants them redirected from/to file or
	 * another command. As such, it is necessary to clean up the streams after
	 * the commands have finished.
	 * <p>
	 * This is called automatically by process().
	 */
	void cleanup ();
private:
	iash *m_parent;

	std::vector<std::ios*> m_dynamicStreams;
	std::vector<UserCommand> m_allCommands;
};

/**
 * Thrown when the CommandPreprocessor encounters an iash syntax error. This is
 * automatically caught by the iash shell.
 */
class SyntaxException : public std::runtime_error {
public:
	/**
	 * Constructs a SyntaxException with the given reason.
	 *
	 * @param reason	the syntax error
	 */
	SyntaxException(const std::string &reason);
};

#endif /* SRC_COMMANDPREPROCESSOR_H_ */
