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

	Environment (std::string appName = "iash", bool loadFromFile = false);
	virtual ~Environment ();

	void load (std::string filename);
	void save (std::string filename);

	void set (std::string key, std::string value);
	void set (std::string key, bool value);
	void set (std::string key, int value);
	void set (std::string key, double value);

	const std::string& getString (std::string key) const;
	bool getBool (std::string key) const;
	int getInt (std::string key) const;
	double getDouble (std::string key) const;

	void rm (std::string key);
private:
	void setProtected (std::string key, std::string value);
	bool isBuiltinVar (std::string key) const;
	std::map<std::string,std::string> m_envMap;
	std::vector<std::string> m_builtins;
};

#endif /* IASH_ENVIRONMENT_H_ */
