/*
 * CommandPreprocessor.h
 *
 *  Created on: Nov 21, 2016
 *      Author: paul
 */

#ifndef IASH_COMMANDPREPROCESSOR_H_
#define IASH_COMMANDPREPROCESSOR_H_

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <exception>
#include "UserCommand.h"
#include "iash.h"

class CommandPreprocessor {
public:
	CommandPreprocessor(iash *parent);
	virtual ~CommandPreprocessor();

	std::vector<UserCommand>& process (const std::string& raw);
	std::vector<UserCommand>& getCommands () const;
	void cleanup ();
private:
	iash *m_parent;

	std::vector<std::ios*> m_dynamicStreams;
	std::vector<UserCommand> m_allCommands;
};

class SyntaxException : public std::runtime_error {
public:
	SyntaxException(const std::string &reason);
};

class FileNotFoundException : public std::runtime_error {
	FileNotFoundException(const std::string &fname);
};

#endif /* IASH_COMMANDPREPROCESSOR_H_ */
