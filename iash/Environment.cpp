/*
 * Environment.cpp
 *
 *  Created on: Oct 10, 2016
 *      Author: paul
 */

#include "Environment.h"
#include "Directory.h"
using namespace std;

Environment::Environment (string appName, bool loadFromFile) {
	set("IASH_APP_NAME", appName);
	set("CWD", Directory::getWorkingDir());
}

Environment::~Environment () {}

