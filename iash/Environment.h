/*
 * Environment.h
 *
 *  Created on: Oct 10, 2016
 *      Author: paul
 */

#ifndef IASH_ENVIRONMENT_H_
#define IASH_ENVIRONMENT_H_

#include <map>
#include <vector>

class Environment {
public:
	friend class iash;

	Environment (const std::string appName = "iash");
	virtual ~Environment ();

	void set (const std::string &key, const std::string &value);
	void set (const std::string &key, const bool value);
	void set (const std::string &key, const int value);
	void set (const std::string &key, const double value);

	const std::string& getString (const std::string &key) const;
	bool getBool (const std::string &key) const;
	int getInt (const std::string &key) const;
	double getDouble (const std::string &key) const;

	void rm (const std::string &key);
private:
	void setProtected (const std::string &key, const std::string &value);
	bool isBuiltinVar (const std::string &key) const;
	static std::string convName (const std::string &key) const;

	std::map<std::string,std::string> m_envMap;
	std::vector<std::string> m_builtins;
};

#endif /* IASH_ENVIRONMENT_H_ */
