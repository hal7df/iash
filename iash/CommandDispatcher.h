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

class CommandDispatcher {
public:
	CommandDispatcher(iash *parent);
	virtual ~CommandDispatcher();

	void registerCommand (Command *cmd);
	Command* unregisterCommand (std::string name);
	Command* unregisterCommand (Command *cmd);

	int dispatch (UserCommand *userCmd);
private:
	iash *m_parent;
	std::map<std::string,Command*> m_registry;
};

#endif /* IASH_COMMANDDISPATCHER_H_ */
