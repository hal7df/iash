#ifndef SRC_CMD_ECHOCOMMAND_H_
#define SRC_CMD_ECHOCOMMAND_H_

#include "../Command.h"
#include "../UserCommand.h"

class EchoCommand :public Command{

public:
	virtual const std::string getName () const;
	virtual int run (const UserCommand *cmd);
private:

};

#endif /* SRC_CMD_ECHOCOMMAND_H_ */
