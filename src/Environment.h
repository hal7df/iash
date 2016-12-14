/*
 * Environment.h
 *
 *  Created on: Oct 10, 2016
 *      Author: paul
 */

#ifndef SRC_ENVIRONMENT_H_
#define SRC_ENVIRONMENT_H_

#include <map>
#include <vector>

/**
 * Environment variable manager for iash.
 * <p>
 * Using environment variables is one method that iash provides to allow
 * different Command instances to communicate with each other. Internally,
 * environment variables are stored as string instances, but Environment
 * provides functions to convert numeric and boolean data types to and from
 * the string format.
 * <p>
 * As of version 0.5, iash has the following protected environment variables,
 * which can be read but not modified:
 * <ul>
 * 	<li>`IASH_APP_NAME`: (since v0.1) The name of the application</li>
 * 	<li>`CWD`: (since v0.5) The absolute path to the iash current working
 * 		directory</li>
 * </ul>
 *
 * @since 0.5
 */
class Environment {
public:
	friend class iash;

	/**
	 * Creates an Environment instance with the given application name. iash
	 * instantiates its own Environment instance, so there is no need to
	 * construct one manually.
	 *
	 * @param appName	the name of the application
	 */
	Environment (const std::string appName = "iash");
	virtual ~Environment ();

	/**
	 * Sets the environment variable `key` to `value`. `key` will automatically
	 * be converted to upper case, if it is not already.
	 *
	 * @param key	the name of the environment variable
	 * @param value	the new value of the environment variable
	 */
	void set (const std::string &key, const std::string &value);

	/**
	 * Sets the environment variable `key` to the boolean value `value`. `key`
	 * will automatically be converted to upper case, if it is not already.
	 *
	 * @param key	the name of the environment variable
	 * @param value	the new value of the environment variable
	 */
	void set (const std::string &key, const bool value);

	/**
	 * Sets the environment variable `key` to the integer value `value`. `key`
	 * will automatically be converted to upper case, if it is not already.
	 *
	 * @param key	the name of the environment variable
	 * @param value	the new value of the environment variable
	 */
	void set (const std::string &key, const int value);

	/**
	 * Sets the environment variable `key` to the floating-point value `value`.
	 * `key` will automatically be converted to upper case, if it is not
	 * already.
	 *
	 * @param key	the name of the environment variable
	 * @param value	the new value of the environment variable
	 */
	void set (const std::string &key, const double value);

	/**
	 * Gets the environment variable `key` as a string. `key` will automatically
	 * be converted to upper case, if it is not already.
	 *
	 * @param key	the name of the environment variable
	 * @return		the value of the environment variable, as a string
	 */
	const std::string& getString (const std::string &key) const;

	/**
	 * Gets the environment variable `key` as a boolean value. `key` will
	 * automatically be converted to upper case, if it is not already.
	 *
	 * @param key	the name of the environment variable
	 * @return		the value of the environment variable, as a string
	 */
	bool getBool (const std::string &key) const;

	/**
	 * Gets the environment variable `key` as an integer. `key` will
	 * automatically be converted to upper case, if it is not already.
	 *
	 * @param key	the name of the environment variable
	 * @return		the value of the environment variable, as a string
	 */
	int getInt (const std::string &key) const;

	/**
	 * Gets the environment variable `key` as a floating-point value. `key` will
	 * automatically be converted to upper case, if it is not already.
	 *
	 * @param key	the name of the environment variable
	 * @return		the value of the environment variable, as a string
	 */
	double getDouble (const std::string &key) const;

	/**
	 * Removes the environment variable `key`. `key` will automatically be
	 * converted to upper case, if it is not already.
	 *
	 * @param key	the name of the environment variable to delete
	 */
	void rm (const std::string &key);
private:
	void setProtected (const std::string &key, const std::string &value);
	bool isBuiltinVar (const std::string &key) const;
	static std::string convName (const std::string &key);

	std::map<std::string,std::string> m_envMap;
	std::vector<std::string> m_builtins;
};

#endif /* SRC_ENVIRONMENT_H_ */
