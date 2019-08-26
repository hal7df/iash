/*  See iash.h for the project description.
 *
 *  Copyright (C) 2014  Paul Bonnen
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "iash.h"
#include "cmd/builtins.h"
#include "tools/Tokenizer.h"
#include "CommandPreprocessor.h"
#include <fstream>
using namespace std;

iash::iash (const string &appName)
	: m_dispatcher(this), m_env(appName), m_iashCwd(), m_appName(appName)
{
	addCommand<EchoCommand>();
	addCommand<ExitCommand>();
	addCommand<PwdCommand>();
	addCommand<ClearCommand>();
}

Environment* iash::getEnv ()
{
	return &m_env;
}

const Directory* iash::getCwd () const
{
	return &m_iashCwd;
}

void iash::addCommand (Command *cmd)
{
	m_dispatcher.registerCommand(cmd);
}

int iash::runInteractive ()
{
	run(cin, true);
	return 0;
}

int iash::runScript (const char *fname)
{
	string filename = Directory::toPlatform(m_iashCwd.resolvePath(fname));

	ifstream fin (filename.c_str());

	if (fin) return run(fin);
	else
	{
		cout << m_appName << ": " << fname << ": script could not be opened";
		cout << endl;
		return 1;
	}
}

inline int iash::exec (string &cmd)
{
	UserCommand uCmd (cmd, cin, cout);
	return m_dispatcher.dispatch(&uCmd);
}

inline int iash::exec (UserCommand *cmd)
{
	return m_dispatcher.dispatch(cmd);
}

int iash::run (istream &cmdin, bool showPrompt)
{
	CommandPreprocessor processor (this);
	string raw;
	int returnCode = 0;

	m_exitFlag = false;

	if (showPrompt) cout << m_appName << "> ";

	while (!m_exitFlag && getline(cmdin, raw))
	{
		//Run commands
		try
		{
			const vector<UserCommand> &commands = processor.process(raw);

			for (const UserCommand &cmd : commands)
			{
				if (cmd.getWholeCommand().size() == 1 && 
				    cmd.getWholeCommand()[0].empty()) {
				    continue;
				}

				returnCode = m_dispatcher.dispatch(&cmd);

                                if (returnCode == 127) {
                                    cout << "iash: command not found: ";
                                    cout << cmd.getWholeCommand()[0] << endl;
                                }

				//Stop running commands if exit was called
				if (m_exitFlag) break;
			}
		}
		catch (TokenizeException &toke)
		{
			cout << "iash: " << toke.what() << endl;
		}
		catch (SyntaxException &syne)
		{
			cout << "iash: " << syne.what() << endl;
		}
		catch (FileNotFoundException &fnfe)
		{
			cout << "iash: " << fnfe.what() << endl;
		}

		//Update environment
		m_env.setProtected("?", to_string(returnCode));

		if (m_iashCwd.getAbsPath() != m_env.getString("CWD"))
			m_env.setProtected("CWD", m_iashCwd.getAbsPath());

		if (showPrompt && !m_exitFlag) cout << m_appName << "> ";
	}

	return m_env.getInt("?");
}

void iash::exitShell()
{
	m_exitFlag = true;
}
