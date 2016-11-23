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
#include <fstream>
using namespace std;

iash::iash (string appName)
	: m_appName(appName), m_dispatcher(this), m_env(appName), m_iashCwd()
{

}

Environment* iash::getEnv () const
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

}

int iash::exec (string cmd)
{

}

int iash::run (istream& cmdin, bool showPrompt)
{
	string raw;

	do
	{
		if (showPrompt) cout << m_appName << "> ";
		getline(cmdin, raw);

		if (cmdin)
		{

		}
	} while (cmdin);
}
