#ifndef SRC_CMD_ECHOCOMMAND_H_
#define SRC_CMD_ECHOCOMMAND_H_

#include "../../src/Command.h"

class EchoCommand :public Command{

public:
	virtual std::string getName () const;
	virtual int run (UserCommand *cmd);
private:

};

#endif /* SRC_CMD_ECHOCOMMAND_H_ */
