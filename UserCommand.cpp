/*
 * UserCommand.cpp
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#include "UserCommand.h"
#include "tools/Tokenizer.h"
#include <algorithm>
using namespace std;

UserCommand::UserCommand (const string &inputCommand, istream &in, ostream &out)
		: m_stdin(in), m_stdout(out), m_raw(inputCommand)
{
	Tokenizer despace (inputCommand);

	for (Token token : despace.getTokens())
	{
		token.finalize();
		m_commandParts.push_back(token.getToken());
	}
}

UserCommand::UserCommand (const Token &inputCommand, istream &in, ostream &out)
	: m_stdin(in), m_stdout(out), m_raw(inputCommand.getToken())
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

	for (const string &part : m_commandParts)
	{
		if (part.empty() || (part.size() > 1 && part[0] != '-'))
			args.push_back(part);
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

bool UserCommand::hasOption(char opt) const
{
    for (const std::string &part : m_commandParts)
    {
        //If not an option argument, continue to the next argument
        if (part.size() < 2 || part[0] != '-' || part[1] == '-') continue;

        auto it = part.begin();

        while (++it != part.end())
            if (*it == opt) return true;
    }

    return false;
}

bool UserCommand::hasOption(const std::string &opt) const
{
    for (const std::string &part : m_commandParts)
    {
        if (part.size() != (opt.size() + 2) || !(part[0] == '-' && part[1] == '-'))
            continue;

        if (std::equal(opt.cbegin(), opt.cend(), (part.cbegin() + 2)))
            return true;
    }

    return false;
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
