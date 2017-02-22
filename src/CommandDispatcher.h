/*
 * CommandDispatcher.h
 *
 *  Created on: Oct 7, 2016
 *      Author: paul
 */

#ifndef SRC_COMMANDDISPATCHER_H_
#define SRC_COMMANDDISPATCHER_H_

#include <string>
#include <map>
#include <iostream>
#include <utility>

class iash;
class Command;
class UserCommand;

/**
 * Class to register and execute Commands in an iash shell. iash has an internal
 * instance of this class, there is typically no need to instantiate it
 * manually.
 * <p>
 * iash does not provide direct access to this API through its own API, but
 * commands can be registered through iash#addCommand.
 *
 * @since 0.5
 */
class CommandDispatcher {
public:
	/**
	 * Initializes a CommandDispatcher with the given shell parent.
	 *
	 * @param parent	the shell this CommandDispatcher belongs to
	 */
	CommandDispatcher(iash *parent);
	virtual ~CommandDispatcher();

	/**
	 * Registers a given Command with this CommandDispatcher instance. This will
	 * call Command#init to set the parent shell and add the Command's name and
	 * aliases to the command registry.
	 * <p>
	 * This function requires a dynamically allocated Command, passed like
	 * so:
	 * ~~~{cpp}
	 * CommandDispatcher disp (&shell);
	 * disp.registerCommand(new MyCommand);
	 * ~~~
	 * The CommandDispatcher will automatically free the allocated memory, so
	 * there is no need to somehow catch the pointer to the allocated memory.
	 * <p>
	 * If a Command instance with the same name and type is already registered
	 * with this class, this function will not add the new instance. If the
	 * classes must be replaced, the old one should be unregistered first.
	 * <p>
	 * This is not the recommended method to use when registering commands.
	 * CommandDispatcher#registerCommand() should be used instead.
	 *
	 * @param cmd	a dynamically allocated Command object to register
	 */
	void registerCommand (Command *cmd);

	/**
	 * Registers the given Command type with this CommandDispatcher instance.
	 * This function will allocate the memory for the Command internally; all
	 * the client must do is provide the type:
	 * ~~~{cpp}
	 * CommandDispatcher disp (&shell);
	 * disp.registerCommand<MyCommand>();
	 * ~~~
	 * If a Command instance of the same type is already registered with this
	 * class, this function will not add the new instance. If the classes must
	 * be replaced, the old one should be unregistered first.
	 *
	 * @param args			the arguments to pass to the constructor
	 * @tparam ExtCommand	the Command type to register
	 * @tparam Args			the types of the arguments to pass to the Command
	 * 						constructor
	 */
	template <typename ExtCommand, typename... Args>
	void registerCommand (Args&&... args)
	{
		registerCommand(new ExtCommand(std::forward(args)...));
	}

	/**
	 * Removes the command with the given name from the registry, and removes
	 * any of its registered aliases. Note that, since Commands are typically
	 * created with dynamic memory, you <b>must</b> catch the pointer returned
	 * by this function and `delete` it.
	 *
	 * @param name	the name of the Command to unregister
	 * @return		a pointer to the unregistered Command object
	 */
	Command* unregisterCommand (const std::string& name);

	/**
	 * Removes the command at the given memory address. Note that, since
	 * Commands are dynamically allocated, the Command should be deleted after
	 * this call.
	 *
	 * @param cmd	a pointer to the Command to unregister
	 * @return		a pointer to the unregistered Command object
	 */
	const Command* unregisterCommand (const Command *cmd);

	/**
	 * Searches the command and alias registries and calls the appropriate
	 * Command.
	 *
	 * @param userCmd	the user's input command
	 * @return			the return value of the Command#run
	 * 					for the given command, or 127 if no command was found
	 */
	int dispatch (const UserCommand *userCmd);
private:
	iash *m_parent;
	std::map<std::string,Command*> m_registry;
	std::map<std::string,Command*> m_aliasRegistry;
};

#endif /* SRC_COMMANDDISPATCHER_H_ */
