#include "EchoCommand.h"
using namespace std;


virtual std::string getName () const {
	return "echo";
}

virtual int run (UserCommand *cmd) {
	ostream &os = cmd->getStdout();

	const vector<string> &args = cmd->getWholeCommand();

	for(auto it = ++(args.begin()); it != args.end(); ++it) {
		os << *it;
	}

	os << endl;

	return 0;
 }
