#include "ClearCommand.h"
using namespace std;

const std::string ClearCommand::getName () const {
	return "clear";
}

int ClearCommand::run (const UserCommand *cmd) {
	ostream &os = cmd->getStdout();
	
	char const escape = 27;
	string clearASCII = "";
	clearASCII.append(sizeof(escape), escape);
	clearASCII.append("[H");
	clearASCII.append(sizeof(escape), escape);
	clearASCII.append("[2J");
	os << clearASCII;

	return 0;
 }