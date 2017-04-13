#ifndef FILE_CPP_INCLUDED
#define FILE_CPP_INCLUDED

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdexcept>
#include <dirent.h>

#ifdef _WIN32
# ifndef S_ISDIR
#  define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
# endif
#else
# include <unistd.h>
#endif

#include "core/log.h"
#include "core/file.h"
#include "core/exception.h"

namespace cr {

using std::list;
using std::string;
using std::runtime_error;

// Constructor
File::File() : 
		_exists(false), _valid(false) {
}

// Constructor
File::File( const string& path ) : _path(path), _exists(false), _valid(false) { 
	initialize();

};

// Constructor
File::File( const File& dir, const string& fileName ) : _exists(false), _valid(false) {
	_path = dir._dir + fileName;
	initialize();
}

// Constructor
File::File( const File& dir, const File& file ) : _exists(false), _valid(false) {
	_path = dir._dir + file.getFullPath();
	initialize();
}

// initialize
void File::initialize() {

	evaluate();

	if ( _valid ) {

		// prune any trailing slash (/) if this is a dir
		if ( _isDir && _path.size() > 1 && PATH_SEPARATOR == _path[_path.size()-1] ) {
			_path = _path.substr( 0, _path.size() - 1 );

		} else if ( ! _isDir ) {

			// if this isn't a dir, split the file path / name
			i32 lastSeparator = _path.find_last_of(PATH_SEPARATOR);
			_fileName = _path.substr( lastSeparator + 1 );
			_dir = _path.substr( 0, lastSeparator ) + PATH_SEPARATOR;

		}

		// if this is a dir, copy _path to _dir
		if ( _isDir ) {
			_dir = _path + PATH_SEPARATOR;
		}
	}
}

// evaluate
void File::evaluate() const {

	// use stat() to validate the file
	struct stat fileStats;
	i32 status = stat( _path.c_str(), &fileStats );
	_exists = (0 == status);
	_valid = (_exists || errno != EBADF);
	if ( _exists ) {
		_isDir = S_ISDIR( fileStats.st_mode );
	} else {
		_isDir = false;
	}
}

// getParentDir
File File::getParentDir() const {

	if ( ! exists() ) {

		i32 lastSeparator = _path.find_last_of( PATH_SEPARATOR, _path.size() - 2 );
		string newPath = _path.substr( 0, lastSeparator );
		return File( newPath );

	} else if ( isDir() ) {

		i32 lastSeparator = _path.find_last_of(PATH_SEPARATOR);
		string newPath = _path.substr( 0, lastSeparator );
		return File( newPath );
	} else {
		return File( _dir );
	}
}

// mkdir
void File::mkdir( bool createParents ) {
#ifdef _WIN32
	STUB // fix for windows!
#else
	if ( exists() ) {
		if ( ! isDir() ) {
			throw CRException( 
					"Cannot create directory %s because it is an existing file",
					getFullPath().c_str() );
		}
	} else {

		File parentDir = getParentDir();

		if ( createParents ) {

			// go back up heirarchy until we find a parent that exists
			list<File> ancestors;
			while ( ! parentDir.exists() ) {
				ancestors.push_front( parentDir );
				parentDir = parentDir.getParentDir();
			}

			for ( File& dir : ancestors ) {
				dir.mkdir( false );
			}

		} else if ( ! parentDir.exists() ) {

			throw CRException( 
					"Cannot create directory %s because its parent does not exist",
					getFullPath().c_str() );

		}

		// why not try, and make yourself? make... your... self...
		i32 error = ::mkdir( getFullPath().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
		if ( error ) {
			throw CRException( "Error(%d) while trying to mkdir %s: %s",
					errno,
					getFullPath().c_str(),
					strerror(errno) );
		}

		// now chmod -- the permissions specified above may be ignored because of process umask
		error = ::chmod( getFullPath().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
		if ( error ) {
			throw CRException( "Error(%d) while trying to chmod %s: %s",
					errno,
					getFullPath().c_str(),
					strerror(errno) );
		}
	}
#endif
}

// rm
void File::rm() const {

	evaluate();

#ifdef _WIN32
	STUB // fix for windows!
#else
	if ( ! exists() ) {
		throw CRException( "Cannot rm non-existent file: %s", getFullPath().c_str() );
	} else {
		if ( isDir() ) {
			throw CRException( "Cannot rm directory (not supported): %s", getFullPath().c_str() );
		}

		i32 error = unlink( getFullPath().c_str() );
		if ( error ) {
			throw CRException( "Failed to delete file (error: %d): %s", error, getFullPath().c_str() );
		}
	}
#endif
}

// log
void File::log() {

	Log::i( "File: \n    _path: %s\n    _dir: %s\n    _fileName: %s",
			_path.c_str(), _dir.c_str(), _fileName.c_str() );
	Log::i( "    _valid: %s\n    _isDir: %s\n    _exists: %s\n", 
			_valid ? "true" : "false", 
			_isDir ? "true" : "false", 
			_exists ? "true" : "false" );
}

// exists
bool File::exists() const {
	evaluate();
	return _exists;
}

// operator+
File File::operator+( const string& file ) const {
	return File( _dir + file );
}

// operator+=
File& File::operator+=( const string& file ) {
	_path = _dir + file;
	initialize();
	return *this;
}

// operator+
File File::operator+( const File& file ) const {
	return File( _dir + file._path );
}

// operator+=
File& File::operator+=( const File& file ) {
	_path = _path + file._path;
	initialize();
	return *this;
}

// operator=
File& File::operator=( const string& path ) {
	_path = string(path);
	initialize();

	return *this;
}

// operator<
bool File::operator<( const File& file ) const {
	// TODO: use actual FS to determine this? would that even work?
	return (_path < file._path);
}

// operator>
bool File::operator>( const File& file ) const {
	// TODO: use actual FS to determine this? would that even work?
	return (_path > file._path);
}

// operator==
bool File::operator==( const File& file ) const {
	// TODO: use actual FS to determine this? would that even work?
	return (_path == file._path);
}

// getMTime
ui64 File::getMTime() const {
	if ( ! _valid || ! _exists ) {
		throw runtime_error( "File does not exist" );
	}

	struct stat fileStats;
	i32 status = stat( _path.c_str(), &fileStats );
	if ( 0 != status ) {
		throw runtime_error( "File does not exist" );
	}

	return fileStats.st_mtime;

}

// getSize
ui64 File::getSize() const {
	if ( ! _valid || ! _exists ) {
		throw runtime_error( "File does not exist" );
	}

	struct stat fileStats;
	i32 status = stat( _path.c_str(), &fileStats );
	if ( 0 != status ) {
		throw runtime_error( "File does not exist" );
	}

	return fileStats.st_size;
}

// refresh
void File::refresh() {
	initialize();
}

// listFiles
list<File> File::listFiles() {

	DIR* dp = nullptr;
	try  {
		if ( ! isDir() ) {
			throw CRException( "Cannot list contents of non-directory %s", getFullPath().c_str() );
		}

		list<File> files;

		struct dirent* dirp;
		dp = opendir( getFullPath().c_str() );
		if ( dp == nullptr) {
			throw CRException( "Error(%d) while trying to open %s",
					errno,
					getFullPath().c_str() );
		}

		dirp = readdir( dp );
		while ( dirp != NULL ) {
			files.push_back( File( dirp->d_name ));
			dirp = readdir( dp );
		}

		closedir( dp );

		return files;

	} catch ( ... ) {
		try {
			if ( dp != nullptr ) {
				closedir( dp );
			}
		} catch ( ... ) {
			Log::w( "Warning: double exception while trying to close dir %s", getFullPath().c_str());
		}
		throw;
	}
}

// sortAlphaNumeric
void File::sortAlphaNumeric( list<File>& files ) {
	files.sort( []( File& f1, File& f2 ) {
		return f1.getFileName() < f2.getFileName();
	} );
}

} // namespace cr

#endif // !FILE_CPP_INCLUDED
