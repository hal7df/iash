/*
 * CommandDispatcher.h
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#ifndef IASH_COMMANDDISPATCHER_H_
#define IASH_COMMANDDISPATCHER_H_

#include "iash.h"
#include "Command.h"
#include "UserCommand.h"
#include <string>
#include <map>
#include <iostream>

class CommandDispatcher {
public:
	CommandDispatcher(iash *parent);
	virtual ~CommandDispatcher();

	void registerCommand (Command *cmd);
	Command* unregisterCommand (std::string name);
	Command* unregisterCommand (Command *cmd);

	int dispatch (UserCommand *userCmd, std::istream &is, std::ostream &os);
private:
	iash *m_parent;
	std::map<std::string,Command*> m_registry;
	std::map<std::string,Command*> m_aliasRegistry;
};

#endif /* IASH_COMMANDDISPATCHER_H_ */
