/*
 * CommandPreprocessor.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: paul
 */

#include "CommandPreprocessor.h"
#include "iash.h"
#include "UserCommand.h"
#include "tools/Token.h"
#include "tools/Tokenizer.h"

#include <fstream>
#include <sstream>

using namespace std;

CommandPreprocessor::CommandPreprocessor(iash *parent) : m_parent(parent) {}

CommandPreprocessor::~CommandPreprocessor()
{
	cleanup();
}

void CommandPreprocessor::cleanup ()
{
	m_allCommands.clear();

	for (ios* stream : m_dynamicStreams) delete stream;

	m_dynamicStreams.clear();
}

const vector<UserCommand>& CommandPreprocessor::process(const string& raw)
{
	istream *stdinCand;
	ostream *stdoutCand;
	stringstream *pipeStream = nullptr;
	const Token *commandToken = nullptr;

	//Clear the previous set of commands and dynamic streams
	cleanup();

	//Tokenize individual command calls
	Tokenizer parse (raw, ";|");
	vector<Token> cmds = parse.getTokens();

	//Now tokenize for input/output from/to files
	parse.setDelims("<>");

	for (Token &cmdToken : cmds)
	{
		parse.setSource(cmdToken.getToken());

		//Set default stdin and stdout for the command
		stdinCand = pipeStream == nullptr ? &cin : pipeStream;
		stdoutCand = &cout;

		//If the pipe stream is stdout, clear the pipeStream pointer
		if (stdinCand == pipeStream) pipeStream = nullptr;

		//If the user is performing a pipe, create a pipe stream and make it
		//stdin
		if (cmdToken.getFollowingDelimiter() == '|')
		{
			pipeStream = new stringstream (ios::in | ios::out);
			m_dynamicStreams.push_back(pipeStream);
			stdoutCand = pipeStream;
		}

		for (const Token &ioToken : parse.getTokens())
		{
			//If the token is the command call
			if (ioToken.getPrecedingDelimiter() == '\0') commandToken = &ioToken;

			//If the token is an input stream pathspec
			else if (ioToken.getPrecedingDelimiter() == '<')
			{
				if (ioToken.isToken(' ') && stdinCand == &cin)
				{
					string fname = (Token (ioToken)).finalize();

					if (*(fname.begin()) != '/')
					{
						const Directory &cwd = *(m_parent->getCwd());

						fname = cwd.resolvePath(fname);
					}

					if (Directory::isFile(fname.c_str()))
					{
						stdinCand = new ifstream(fname.c_str());
						m_dynamicStreams.push_back(stdinCand);
					}
					else throw FileNotFoundException (fname);
				}
				else if (stdinCand != &cin)
					throw SyntaxException("Input stream already specified");
				else throw SyntaxException("Unexpected ' ' in pathspec");
			}

			//If the token is an output stream pathspec
			else if (ioToken.getPrecedingDelimiter() == '>')
			{
				if (ioToken.isToken(' ') && stdoutCand == &cout)
				{
					string fname = (Token(ioToken)).finalize();

					if (*(fname.begin()) != '/')
					{
						const Directory &cwd = *(m_parent->getCwd());

						fname = cwd.resolvePath(fname);
					}

					if (Directory::isFile(fname.c_str()))
					{
						stdoutCand = new ofstream(fname.c_str());
						m_dynamicStreams.push_back(stdoutCand);
					}
					else throw FileNotFoundException(fname);
				}
				else if (stdoutCand != &cout)
					throw SyntaxException("Output stream already specified");
				else throw SyntaxException("Unexpected ' ' in pathspec");
			}
		}

		m_allCommands.push_back(UserCommand(*commandToken, *stdinCand, *stdoutCand));
	}


	return m_allCommands;
}

const vector<UserCommand>& CommandPreprocessor::getCommands () const
{
	return m_allCommands;
}

SyntaxException::SyntaxException (const string &reason)
	: runtime_error("syntax error: " + reason) {}
