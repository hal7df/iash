/*
 * ExitCommand.h
 *
 *  Created on: Dec 16, 2016
 *      Author: paul
 */

#ifndef SRC_CMD_EXITCOMMAND_H_
#define SRC_CMD_EXITCOMMAND_H_

#include "../Command.h"

class ExitCommand: public Command {
public:
	ExitCommand() {}
	virtual ~ExitCommand() {}

	virtual std::string getName () const;
	virtual int run (UserCommand *cmd);
};

#endif /* SRC_CMD_EXITCOMMAND_H_ */
