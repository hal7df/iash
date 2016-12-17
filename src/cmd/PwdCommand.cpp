/*
 * PwdCommand.cpp
 *
 *  Created on: Dec 14, 2016
 *      Author: Chad
 */

#include "PwdCommand.h"
using namespace std;

const string PwdCommand::getName () const {
	return "pwd";
}

int PwdCommand::run (const UserCommand *cmd) {
	ostream &os = cmd->getStdout();

	os << m_parent->getCwd()->getAbsPath() << endl;

	return 0;
}
