/*
 * ExitCommand.cpp
 *
 *  Created on: Dec 16, 2016
 *      Author: paul
 */

#include "ExitCommand.h"
#include "../iash.h"
#include <cctype>
#include <iostream>
using namespace std;

const string ExitCommand::getName() const
{
	return "exit";
}

int ExitCommand::run(const UserCommand *cmd)
{
	m_parent->exitShell();

	if (cmd->getWholeCommand().size() == 1) return 0;
	else
	{
		const string& arg = cmd->getWholeCommand().at(1);
		bool allDigits = true;

		for (const char &c : arg)
		{
			if (!isdigit(c))
			{
				allDigits = false;
				break;
			}
		}

		if (allDigits) return stoi(arg);
		else return 1;
	}
}
