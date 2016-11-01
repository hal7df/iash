/*
 * Environment.cpp
 *
 *  Created on: Oct 10, 2016
 *      Author: paul
 */

#include <string>
#include <cctype>
#include <cassert>
#include "Environment.h"
#include "Directory.h"
using namespace std;

Environment::Environment (string appName)
{
	setProtected("IASH_APP_NAME", appName);
	setProtected("CWD", Directory::getWorkingDir());

	m_builtins.push_back("IASH_APP_NAME");
	m_builtins.push_back("CWD");
}

Environment::~Environment () {}

void Environment::set (const string &key, const string &value)
{
	assert(key.find(' ') == string::npos);
	string keyInCaps = convName(key);

	if (!isBuiltinVar(key)) setProtected(key, value);
}

void Environment::set (const string &key, const bool value)
{
	if (value) set(key, "true");
	else set(key, "false");
}

void Environment::set (const string &key, const int value)
{
	set(key, to_string(value));
}

void Environment::set (const string &key, const double value)
{
	set(key, to_string(value));
}

const string& Environment::getString (const string &key) const
{
	assert(key.find(' ') == string::npos);

	try {
		return m_envMap.at(convName(key));
	} catch (out_of_range &noKey) {
		return "";
	}
}

bool Environment::getBool (const string &key) const
{
	string envString = getString(key);

	assert(envString == "true" || envString == "false");

	return envString == "true";
}

int Environment::getInt (const string &key) const
{
	string envString = getString(key);

	assert(envString.length() > 0);

	return stoi(envString);
}

double Environment::getDouble (const string &key) const
{
	string envString = getString(key);

	assert(envString.length() > 0);

	return stod(envString);
}

void Environment::rm (const string &key)
{
	map<string,string>::iterator it = m_envMap.find(key);

	if (it != m_envMap.end()) m_envMap.erase(it);
}

void Environment::setProtected (const string &key, const string &value)
{
	assert(key.find(' ') == string::npos);
	string keyInCaps = convName (key);

	m_envMap[keyInCaps] = value;
}

bool Environment::isBuiltinVar (const string &key) const
{
	for (unsigned i = 0; i < m_builtins.size(); ++i)
	{
		if (key == m_builtins.at(i)) return true;
	}

	return false;
}

string Environment::convName (const string &key) const
{
	string keyInCaps = key;

	for (int i = 0; i < key.length(); ++i)
		keyInCaps[i] = toupper(key[i]);

	return keyInCaps;
}
