/*
 * UserCommand.cpp
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#include "UserCommand.h"
#include "tools/Tokenizer.h"
using namespace std;

UserCommand::UserCommand (const string &inputCommand, istream &stdin, ostream &stdout)
		: m_stdin(stdin), m_stdout(stdout), m_raw(inputCommand)
{
	Tokenizer despace (inputCommand);

	for (Token token : despace.getTokens())
	{
		token.finalize();
		m_commandParts.push_back(token.getToken());
	}
}

UserCommand::UserCommand (const Token &inputCommand, istream &stdin, ostream &stdout)
	: m_stdin(stdin), m_stdout(stdout), m_raw(inputCommand.getToken())
{
	Tokenizer despace (inputCommand);

	for (Token token : despace.getTokens())
	{
		token.finalize();
		m_commandParts.push_back(token.getToken());
	}
}

UserCommand::~UserCommand () {}

const vector<string>& UserCommand::getWholeCommand () const
{
	return m_commandParts;
}

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
	return m_stdin;
}

ostream& UserCommand::getStdout () const
{
	return m_stdout;
}
