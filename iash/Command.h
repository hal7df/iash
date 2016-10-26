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
#include <iostream>
#include <string>
#include <vector>

class Command {
public:
	Command();
	virtual ~Command();

	void init (iash *parent);

	virtual void getName () const = 0;
	virtual std::vector<std::string> getAliases () const;
	virtual std::string getAliasMapping (const std::string alias) const;

	virtual int run (UserCommand *cmd, std::istream &is, std::ostream &os) = 0;
	virtual void showUsageMessage (std::ostream &os) const;
	virtual bool validate (UserCommand *cmd) const;
protected:
	iash *m_parent;
};

#endif /* IASH_COMMAND_H_ */
