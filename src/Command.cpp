/*
 * Command.cpp
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#include "../src/Command.h"
using namespace std;

Command::Command() {
	m_parent = nullptr;
}

Command::~Command() {
	// TODO Auto-generated destructor stub
}

void Command::init (iash *parent)
{
	m_parent = parent;
}

vector<string> Command::getAliases() const
{
	return vector<string>;
}

string Command::getAliasMapping (string alias) const
{
	return getName();
}

void Command::showUsageMessage (ostream &os) const
{
	os << getName() << ": invalid use of command" << endl;
}

bool Command::validate (UserCommand *cmd) const
{
	return true;
}
