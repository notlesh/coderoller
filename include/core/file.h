#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <string>
#include <list>

#include "types.h"

namespace cr {

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

using std::string;
using std::list;

/**
 * Abstraction for a "file path"
 */
class File {

	public:

		/**
		 * Empty constructor -- creates an invalid file
		 */
		File();
		
		/**
		 * Constructor
		 */
		File( const string& path );

		/**
		 * Constructor
		 */
		File( const File& dir, const string& filename );

		/**
		 * Constructor
		 */
		File( const File& dir, const File& file );

		/**
		 * Returns the file name if a file is associated with this path.
		 */
		const string& getFileName() const { return _fileName; };

		/**
		 * If this is a file, returns the directory containing it. If it is a directory, it returns itself.
		 */
		const string& getDir() const { return _dir; };

		/**
		 * If this is a file, returns the directory containing it. If it is a directory, it returns the directory's parent.
		 */
		File getParentDir() const;

		/**
		 * Creates this directory. If the file exists and is not a directory, this will throw. 
		 * If it exists and is a directory, it will silently be ignored. If the parent directory
		 * doesn't exist, it will be created only if the createParents parameter is true.
		 */
		void mkdir( bool createParents = false );

		/**
		 * Delete the file. Will fail if this is a directory.
		 */
		void rm() const;

		/**
		 * Returns the full path.
		 */
		const string& getFullPath() const { return _path; };

		/**
		 * Returns true if this is a directory, false otherwise. This is undefined if the file does
		 * not exist.
		 */
		bool isDir() const { return _isDir; };

		/**
		 * Returns whether or not the file existed at the time this object was instantiated.
		 */
		bool exists() const;

		/**
		 * Returns whether or not this file is valid (expresses a compliant file path). It can be
		 * valid and not exist.
		 */
		bool isValid() const { return _valid; };

		/**
		 * Operator +
		 */
		File operator+( const string& file ) const;

		/**
		 * Operator +=
		 */
		File& operator+=( const string& file );

		/**
		 * Operator +
		 */
		File operator+( const File& file ) const;

		/**
		 * Operator +=
		 */
		File& operator+=( const File& file );

		/**
		 * Operator = (string)
		 */
		File& operator=( const string& path );

		/**
		 * Operator <
		 */
		bool operator<( const File& file ) const;

		/**
		 * Operator >
		 */
		bool operator>( const File& file ) const;

		/**
		 * Operator ==
		 */
		bool operator==( const File& file ) const;

		/**
		 * Prints to log (i) a debug string describing this file
		 */
		void log();

		/**
		 * Fetch the mtime info for this file
		 */
		ui64 getMTime() const;

		/**
		 * Fetch the size in bytes of this file
		 */
		ui64 getSize() const;

		/**
		 * Attempts to reload information about this file, Some information (whether the file exists
		 * and whether it is a directory, etc.) is cached; some is not (such as size and mtime).
		 */
		void refresh();

		/**
		 * Lists the contents of a directory.
		 */
		list<File> listFiles();

		/**
		 * Sort a file listing alpha-numerically
		 */
		static void sortAlphaNumeric( list<File>& files );

	private:

		/**
		 * Initializes the instance
		 */
		void initialize();

		/**
		 * Evaluate the file
		 */
		void evaluate() const;

		string _path;
		string _dir;
		string _fileName;
		mutable bool _isDir;
		mutable bool _exists;
		mutable bool _valid;


};

} // namespace cr

#endif // !FILE_H_INCLUDED
