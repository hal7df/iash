#ifndef SRC_CMD_CATCOMMAND_H_
#define SRC_CMD_CATCOMMAND_H_

#include "../Command.h"
#include "../UserCommand.h"

class CatCommand : public Command {
    static const constexpr int MARGIN_WIDTH = 6;

public:
    virtual const std::string getName () const;
    virtual int run (const UserCommand *cmd);
};

#endif //SRC_CMD_CATCOMMAND_H_
