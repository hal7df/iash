#ifndef SRC_CMD_CLEARCOMMAND_H_
#define SRC_CMD_CLEARCOMMAND_H_

#include "../Command.h"
#include "../iash.h"

class ClearCommand :public Command{

public:
	virtual const std::string getName () const;
	virtual int run (const UserCommand *cmd);
private:

};

#endif /* SRC_CMD_CLEARCOMMAND_H_ */
