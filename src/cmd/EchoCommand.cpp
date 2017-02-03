#include "EchoCommand.h"
using namespace std;


const std::string EchoCommand::getName () const {
	return "echo";
}

int EchoCommand::run (const UserCommand *cmd) {
	ostream &os = cmd->getStdout();

	const vector<string> &args = cmd->getWholeCommand();

	for(auto it = ++(args.begin()); it != args.end(); ++it) {
		os << *it;

		if (it != args.end()) os << ' ';
	}

	os << endl;

	return 0;
 }
