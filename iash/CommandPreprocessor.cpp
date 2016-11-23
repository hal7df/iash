/*
 * CommandPreprocessor.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: paul
 */

#include "CommandPreprocessor.h"
#include <fstream>
#include <sstream>
using namespace std;

CommandPreprocessor::CommandPreprocessor(iash *parent) : m_parent(parent) {}

CommandPreprocessor::~CommandPreprocessor()
{
	cleanup();
}

vector<UserCommand>& CommandPreprocessor::process(const string& raw)
{
	m_allCommands.clear();



	return m_allCommands;
}

vector<UserCommand>& CommandPreprocessor::getCommands() const
{
	return m_allCommands;
}
