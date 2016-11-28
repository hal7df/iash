#ifndef IASH_CMD_ECHOCOMMAND_H_
#define IASH_CMD_ECHOCOMMAND_H_

#include "../Command.h"

class EchoCommand :public Command{

public:
	virtual std::string getName () const;
	virtual int run (UserCommand *cmd);
private:

};

#endif /* IASH_CMD_ECHOCOMMAND_H_ */
