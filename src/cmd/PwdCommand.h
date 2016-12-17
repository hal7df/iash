/*
 * PwdCommand.h
 *
 *  Created on: Dec 14, 2016
 *      Author: Chad
 */

#ifndef SRC_CMD_PWDCOMMAND_H_
#define SRC_CMD_PWDCOMMAND_H_

#include "../Command.h"
#include "../iash.h"

class PwdCommand :public Command{

public:
	virtual const std::string getName () const;
	virtual int run (const UserCommand *cmd);
private:

};

#endif /* SRC_CMD_PWDCOMMAND_H_ */
