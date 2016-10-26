/*
 * UserCommand.h
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#ifndef IASH_USERCOMMAND_H_
#define IASH_USERCOMMAND_H_

#include <string>
#include <vector>

class UserCommand {
public:
	UserCommand(std::string inputCommand);
	virtual ~UserCommand();

	std::vector<std::string> getWholeCommand () const;
	std::vector<std::string> getArguments () const;
	std::vector<std::string> getOptions () const;
	std::string getContextualArgument (char opt) const;
	std::string getContextualArgument (std::string opt) const;
private:
	std::vector<std::string> m_commandParts;
	std::string m_raw;
};

#endif /* IASH_USERCOMMAND_H_ */
