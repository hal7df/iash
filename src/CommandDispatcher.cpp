/*
 * CommandDispatcher.cpp
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#include "../src/CommandDispatcher.h"

#include <cassert>
using namespace std;

CommandDispatcher::CommandDispatcher (iash *parent) : m_parent(parent) {}

CommandDispatcher::~CommandDispatcher ()
{
	for (auto &keyPair : m_registry) delete keyPair.second;
}

void CommandDispatcher::registerCommand (Command *cmd)
{
	cmd->init(m_parent);
	m_registry.insert(pair<string,Command*>(cmd->getName(), cmd));

	vector<string> cmdAliases = cmd->getAliases();

	for (string alias : cmdAliases)
	{
		if (m_aliasRegistry.find(alias) == m_aliasRegistry.end())
			m_aliasRegistry.insert(pair<string,Command*>(alias, cmd));
	}
}

Command* CommandDispatcher::unregisterCommand (string name)
{
	map<string,Command*>::iterator pos = m_registry.find(name);

	assert(pos != m_registry.end());

	Command *v = *pos;
	m_registry.erase(pos);

	for (map<string,Command*>::iterator it = m_aliasRegistry.begin(); it != m_aliasRegistry.end(); ++it)
	{
		if (it->second == v) m_registry.erase(it--);
	}

	return v;
}

Command* CommandDispatcher::unregisterCommand (Command *cmd)
{
	bool commandExists = false;

	for (map<string,Command*>::iterator it = m_registry.begin(); it != m_registry.end(); ++it)
	{
		if (it->second == cmd)
		{
			commandExists = true;
			m_registry.erase(it);
		}
	}

	assert(commandExists);

	for (map<string,Command*>::iterator it = m_aliasRegistry.begin(); it != m_aliasRegistry.end(); ++it)
	{
		if (it->second == cmd) m_registry.erase(it--);
	}

	return cmd;
}

int CommandDispatcher::dispatch (UserCommand *userCmd)
{
	map<string,Command*>::iterator it;

	it = m_registry.find(userCmd->getWholeCommand()[0]);

	if (it != m_registry.end())
	{
		int retVal;
		if (!it->second->validate(userCmd))
		{
			it->second->showUsageMessage(userCmd);
			retVal = 1;
		}
		else retVal = it->second->run(userCmd);

		if (retVal == 127) retVal = 1;

		return retVal;
	}
	else return 127;
}
