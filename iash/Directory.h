/*
 * Directory.h
 *
 *  Created on: Oct 30, 2016
 *      Author: paul
 */

#ifndef IASH_DIRECTORY_H_
#define IASH_DIRECTORY_H_

#include <string>

class Directory {
public:
	Directory();
	Directory(std::string dir);
	virtual ~Directory();

	std::string getAbsPath () const;
	bool changeDirRel (std::string relPath);
	bool changeDirAbs (std::string absPath);
	std::string getPathToFileInDirectory (std::string filename);
	int mkSubdir (std::string dirName);

	static int mkdir (const char *pathname);
	static bool isDir (const char *pathname);
	static bool isFile (const char *pathname);
	static std::string getHomeDir ();
	static std::string getConfigDir ();
	static std::string getWorkingDir ();
private:
	std::string m_dirpath;
};

#endif /* IASH_DIRECTORY_H_ */
