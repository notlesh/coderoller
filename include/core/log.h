#ifndef LOG_MANAGER_H_INCLUDED
#define LOG_MANAGER_H_INCLUDED

#include <stdarg.h>
#include <string>

#include "core/types.h"

#define CONSOLE_FG_RED 31
#define CONSOLE_FG_GREEN 32
#define CONSOLE_FG_NORMAL 37
#define CONSOLE_FG_CYAN 36
#define CONSOLE_FG_YELLOW 33

using std::string;

namespace cr {

enum LogLevel { 
	LOG_LEVEL_FINE = 0, 
	LOG_LEVEL_INFO, 
	LOG_LEVEL_WARN, 
	LOG_LEVEL_ERROR
};
enum LogLevelMode { 
	LOG_LEVEL_MODE_NONE, 
	LOG_LEVEL_MODE_UNIX_TERMINAL, 
	LOG_LEVEL_MODE_FLAGS
};

/**
 * Log manager
 */
class Log {

	public:

		/**
		 * Sets the log level mode.
		 */
		static void setLogLevelMode( LogLevelMode mode );

		/**
		 * Sets the min log level that will be printed.
		 */
		static void setMinLogLevel( LogLevel level );

		/**
		 * log a statement at the default log level
		 */
		static i32 log( const char* fmt, ... );

		/**
		 * Hack for tolua
		 */
		static i32 log( const string& text ) { return log( text.c_str() ); };

		/**
		 * Log a statement at the default log level. takes a va_list parameter.
		 */
		static i32 vlog( const char* fmt, LogLevel level, va_list args );

		/**
		 * Static helper method to print a message at level FINE
		 */
		static i32 f( const char* fmt, ... );

		/**
		 * Hack for tolua
		 */
		static i32 f( const string& text ) { return f( text.c_str() ); };

		/**
		 * Static helper method to print a message at level INFO
		 */
		static i32 i( const char* fmt, ... );

		/**
		 * Hack for tolua
		 */
		static i32 i( const string& text ) { return i( text.c_str() ); };

		/**
		 * Static helper method to print a message at level WARN
		 */
		static i32 w( const char* fmt, ... );

		/**
		 * Hack for tolua
		 */
		static i32 w( const string& text ) { return w( text.c_str() ); };

		/**
		 * Static helper method to print a message at level ERROR
		 */
		static i32 e( const char* fmt, ... );

		/**
		 * Hack for tolua
		 */
		static i32 e( const string& text ) { return e( text.c_str() ); };
		

	private:

		/**
		 * Change the color of the console
		 */
		static i32 setConsoleColor( char* buffer, LogLevel level );
		static i32 setConsoleColor( char* buffer, i32 color );


};

} // namespace cr

#endif // !LOG_MANAGER_H_INCLUDED
