/*
 * Command.h
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#ifndef IASH_COMMAND_H_
#define IASH_COMMAND_H_

#include "iash.h"
#include "UserCommand.h"
#include <string>
#include <vector>

class Command {
public:
	Command();
	virtual ~Command();

	void init (iash *parent);

	virtual void getName () const = 0;
	virtual std::vector<std::string> getAliases () const;
	virtual std::string getAliasMapping (std::string alias) const;

	virtual int run (UserCommand *cmd) = 0;
	virtual void showUsageMessage ();
	virtual bool validate (UserCommand *cmd);
protected:
	iash *m_parent;
};

#endif /* IASH_COMMAND_H_ */
