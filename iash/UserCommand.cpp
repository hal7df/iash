/*
 * UserCommand.cpp
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#include "UserCommand.h"
using namespace std;

UserCommand::UserCommand (string inputCommand, istream &stdin, ostream &stdout)
		: m_raw(inputCommand), m_stdin(&stdin), m_stdout(&stdout)
{
	unsigned space, lastSpace;

	lastSpace = 0;

	do
	{
		space = m_raw.find(' ', lastSpace);

		if (lastSpace == 0 && space == string::npos)
			m_commandParts.push_back(m_raw);
		else if (space == string::npos)
			m_commandParts.push_back(m_raw.substr(lastSpace));
		else
		{
			while (m_raw[space - 1] == '\\')
			{
				m_raw.erase(m_raw.begin() + (space - 1));
				space = m_raw.find(' ', space + 1);
			}

			m_commandParts.push_back(m_raw.substr(lastSpace + 1, (space - lastSpace)));
			lastSpace = space + 1;
		}
	} while (space != string::npos);
}

UserCommand::~UserCommand () {}

vector<string> UserCommand::getWholeCommand () const { return m_commandParts; }

vector<string> UserCommand::getArguments () const
{
	vector<string> args;

	for (unsigned cmd = 0; cmd < m_commandParts.size(); ++cmd)
	{
		if (m_commandParts[cmd].find('-') > 0)
			args.push_back(m_commandParts[cmd]);
	}

	return args;
}

vector<string> UserCommand::getOptions() const
{
	vector<string> opts;

	for (unsigned cmd = 0; cmd < m_commandParts.size(); ++cmd)
	{
		if (m_commandParts[cmd].find('-') == 0)
		{
			if (m_commandParts[cmd].find("--") == 0)
				opts.push_back(m_commandParts[cmd].substr(2));
			else if (m_commandParts[cmd].length() > 2)
			{
				for (unsigned i = 0; i < m_commandParts[cmd].length(); ++i)
					opts.push_back(string(1, m_commandParts[cmd][i]));
			}
			else opts.push_back(m_commandParts[cmd].substr(1));
		}
	}

	return opts;
}

string UserCommand::getContextualArgument (char opt) const
{
	bool hasContextualArg;

	for (unsigned cmd = 0; cmd < m_commandParts.size() - 1; ++cmd)
	{
		hasContextualArg = (m_commandParts[cmd].find('-') == 0);
		hasContextualArg &= (m_commandParts[cmd].back() == opt);
		hasContextualArg &= (m_commandParts[cmd + 1].find('-') > 0);

		if (hasContextualArg) return m_commandParts[cmd + 1];
	}

	return "";
}

string UserCommand::getContextualArgument (string opt) const
{
	if (opt.length() == 1) return getContextualArgument(opt[0]);
	else
	{
		bool hasContextualArg;

		for (unsigned cmd = 0; cmd < m_commandParts.size() - 1; ++cmd)
		{
			hasContextualArg = (m_commandParts[cmd].find("--") == 0);
			hasContextualArg &= (m_commandParts[cmd].substr(2) == opt);
			hasContextualArg &= (m_commandParts[cmd + 1].find('-') > 0);

			if (hasContextualArg) return m_commandParts[cmd + 1];
		}

		return "";
	}
}

istream& UserCommand::getStdin () const
{
	return *m_stdin;
}

ostream& UserCommand::getStdout () const
{
	return *m_stdout;
}
