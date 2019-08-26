/*
 * Directory.h
 *
 *  Created on: Oct 30, 2016
 *      Author: paul
 */

#ifndef SRC_DIRECTORY_H_
#define SRC_DIRECTORY_H_

#include <string>
#include <exception>
#include <stdexcept>

/**
 * Represents a directory on the filesystem. Intended to help with managing
 * filesystem resources in a cross-platform manner.
 * <p>
 * On Windows platforms, Directory will convert all directory separators to
 * forward slashes (`/`) from backslashes (`\`), and expects all arguments
 * to use forward slashes. Since fstream will accept either, there is usually
 * no need to change this.
 *
 * @since 0.5
 */
class Directory {
public:
	/**
	 * Creates a Directory instance pointed at the application's current working
	 * directory. Note that this is NOT the same as the iash current working
	 * directory; this is actually held as a Directory instance internal to
	 * iash.
	 */
	Directory();

	/**
	 * Creates a Directory instance pointed at the directory `dir`.
	 *
	 * @param dir	the directory path to create this Directory at
	 */
	Directory(const std::string &dir);
	virtual ~Directory();

	//INSTANCE FUNCTIONS *******************************************************
	/**
	 * Checks if the Directory object points to an existing directory.
	 *
	 * @return	true if the Directory exists, false otherwise
	 */
	bool isValid () const;

	/**
	 * Gets the absolute path to this Directory from the root of the filesystem.
	 *
	 * @return	the absolute path
	 */
	const std::string& getAbsPath () const;

	/**
	 * Changes this Directory instance to point at the directory with path
	 * `relPath` relative to the directory currently pointed to by this
	 * Directory.
	 *
	 * @return	if the change operation was successful (the new directory
	 * 			exists)
	 */
	bool changeDirRel (const std::string &relPath);

	/**
	 * Changes this Directory instance to point at the directory with the
	 * absolute path `absPath`.
	 *
	 * @return	if the change operationb was successful (the new directory
	 * 			exists)
	 */
	bool changeDirAbs (const std::string &absPath);

	/**
	 * Converts a path relative to the current Directory to an absolute
         * path.
	 *
	 * @param relpath   a path relative to the current Directory
	 * @return          the absolute path to the item referred to by relpath
	 */
        std::string resolvePath (const std::string &relpath) const;

	/**
	 * Creates a directory inside of the current Directory instance.
	 *
	 * @param dirName	the name for the new directory
	 * @return			the return value of Directory#mkdir for this operation
	 * @see				Directory::mkdir
	 */
	int mkSubdir (const std::string &dirName) const;

	/**
	 * Checks if the given dirname represents an existing directory inside of
	 * the current Directory.
	 *
	 * @param dirName	the name of the directory to check for existence
	 * @return			true if the directory exists, false otherwise
	 * @see				Directory#isDir
	 */
	bool dirExists (const std::string &dirName) const;

	/**
	 * Checks if the given filename represents an existing file inside of the
	 * current Directory.
	 *
	 * @param filename	the name of the file to check for existence
	 * @return			true if the file exists, false otherwise
	 * @see				Directory#isFile
	 */
	bool fileExists (const std::string &filename) const;

	//STATIC FUNCTIONS *********************************************************
	/**
	 * Creates a directory with the absolute path `pathname`.
	 *
	 * @param pathname	the absolute path of the new directory
	 * @return			0 if successful, -1 otherwise
	 * @see				https://linux.die.net/man/2/mkdir
	 */
	static int mkdir (const char *pathname);

	/**
	 * Checks if `pathname` refers to an existing directory.
	 *
	 * @param pathname	the path of the directory to check for existence
	 * @return			true if the path represents an existing directory, false
	 * 					otherwise
	 */
	static bool isDir (const char *pathname);

	/**
	 * Checks if `pathname` refers to an existing file.
	 *
	 * @param pathname	the path of the file to check for existence
	 * @return			true if the path represents an existing file, false
	 * 					otherwise
	 */
	static bool isFile (const char *pathname);

        /**
         * Converts `pathname` from system path representation to iash path
         * representation. On Unix/Linux/macOS, this does nothing.
         *
         * @param pathname  the path to convert to iash representation
         * @return          the path in iash representation
         */
        static std::string toIash (const std::string &pathname);
        
        /**
         * Converts `pathname` from iash path representation to the platform's
         * path representation. On Unix/Linux/macOS, this does nothing.
         *
         * @param pathname  the path to convert to platform representation
         * @return          the path in platform representation
         */
        static std::string toPlatform (const std::string &pathname);

	/**
	 * Gets the user's home directory as a Directory instance.
	 *
	 * @return	the user's home directory as a Directory
	 */
	static Directory getHomeDir ();

	/**
	 * Gets the user's root application configuration directory ad a Directory
	 * instance. This does not refer to the application's specific configuration
	 * directory.
	 *
	 * @return	the user's configuration directory
	 */
	static Directory getConfigDir ();

	/**
	 * Gets the working directory for this process. Note that this is not the
	 * same as the iash working directory; iash stores that internally as a
	 * Directory instance.
	 *
	 * @return	the process' current working directory
	 */
	static Directory getWorkingDir ();
private:
	void ensureTrailingSlash ();

	std::string m_dirpath;
	bool m_valid;
};

/**
 * Thrown when an iash component cannot find the requested file. This is caught
 * by the iash shell.
 */
class FileNotFoundException : public std::runtime_error {
public:
	/**
	 * Constructs a FileNotFoundException with the given filename.
	 */
	FileNotFoundException(const std::string &fname);
};

#endif /* SRC_DIRECTORY_H_ */
