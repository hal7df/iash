#include "EchoCommand.h"
using namespace std;


std::string EchoCommand::getName () const {
	return "echo";
}

int EchoCommand::run (UserCommand *cmd) {
	ostream &os = cmd->getStdout();

	const vector<string> &args = cmd->getWholeCommand();

	for(auto it = ++(args.begin()); it != args.end(); ++it) {
		os << *it;
	}

	os << endl;

	return 0;
 }
