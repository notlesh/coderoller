#ifndef LOG_MANAGER_CPP_INCLUDED
#define LOG_MANAGER_CPP_INCLUDED

#include <stdarg.h>
#include <string>
#include <stdexcept>

#include "core/log.h"

#ifdef _WIN32
# include <windows.h>
#endif

#include <stdlib.h>

#define ROLLER_LOG_BUFFER_SIZE 4096
#define FLAG_FINE_TEXT "[FINE] "
#define FLAG_WARN_TEXT "[WARN] "
#define FLAG_ERROR_TEXT "[ERROR] "
#define FLAG_INFO_TEXT "[INFO] "

namespace cr {

using std::string;
using std::runtime_error;

LogLevelMode s_logLevelMode = LOG_LEVEL_MODE_NONE;
LogLevel s_minLevel = LOG_LEVEL_FINE;

// setLogLevelMode
void Log::setLogLevelMode( LogLevelMode mode ) { 
	s_logLevelMode = mode;
}

// setMinLogLevel
void Log::setMinLogLevel( LogLevel level ) { 
	s_minLevel = level;
}

// log
i32 Log::log( const char* fmt, ... ) {

	va_list args;
	va_start( args, fmt );

	i32 num = log( fmt, args );

	va_end( args );

	return num;
}

// log with var args
i32 Log::vlog( const char* fmt, LogLevel level, va_list args ) {

	if ( level < s_minLevel ) {
		return 0;
	}

	// our buffer will have a few parts:
	// - prefix (console color and/or flag
	// - original message
	// - suffix (newline)
	//
	// +----------------------------------------+
	// | prefix |       message        | suffix |
	// +----------------------------------------+
	//
	// we will return the length of all parts (the amount written)
	// we will only pass the message part to listeners
	//
	// the whole thing will have to fit inside our stack-allocated buffer, so we use vsnprintf to
	// ensure we don't overflow and warn if we exceeded max size

	char buffer[ROLLER_LOG_BUFFER_SIZE];

	i32 prefixSize = 0;
	i32 messageSize = 0;
	i32 suffixSize = 0;

	// TODO: prefix size

#ifdef _WIN32
	suffixSize = 2; // \r\n
#else
	suffixSize = 1; // \n
#endif

	if ( s_logLevelMode == LOG_LEVEL_MODE_UNIX_TERMINAL ) {
		prefixSize += setConsoleColor( buffer, level );
	}

	if ( s_logLevelMode == LOG_LEVEL_MODE_FLAGS ) {

		const char* flagText = nullptr;
		switch ( level ) {
			case LOG_LEVEL_FINE:
				flagText = FLAG_FINE_TEXT;
				break;
			case LOG_LEVEL_WARN:
				flagText = FLAG_WARN_TEXT;
				break;
			case LOG_LEVEL_ERROR:
				flagText = FLAG_ERROR_TEXT;
				break;
			case LOG_LEVEL_INFO:
			default:
				flagText = FLAG_INFO_TEXT;
				break;
		}

		prefixSize += sprintf( buffer + prefixSize, "%s", flagText );
	}

	i32 available = ROLLER_LOG_BUFFER_SIZE - prefixSize - suffixSize;

	messageSize = vsnprintf( 
			buffer + prefixSize, // pointer to our buffer + what we've already written
			available,
			fmt, 
			args );

	if ( messageSize > available ) {
		Log::w( 
				"Warning: the following message is too big for the print buffer. Max print buffer size is %d", 
				ROLLER_LOG_BUFFER_SIZE );
		messageSize = available;
	}

	// add suffix (newline)
#ifdef _WIN32
	sprintf( buffer + prefixSize + messageSize, "%s", "\r\n" );
#else
	sprintf( buffer + prefixSize + messageSize, "%s", "\r" );
#endif

	// actually print
#ifdef ANDROID_NDK
	// android: use ndk libraries
	switch ( level ) {
		case LOG_LEVEL_FINE:
			__android_log_write( ANDROID_LOG_DEBUG, "roller", buffer );
			break;
		case LOG_LEVEL_WARN:
			__android_log_write( ANDROID_LOG_WARN, "roller", buffer );
			break;
		case LOG_LEVEL_ERROR:
			__android_log_write( ANDROID_LOG_ERROR, "roller", buffer );
			break;
		case LOG_LEVEL_INFO:
		default:
			__android_log_write( ANDROID_LOG_INFO, "roller", buffer );
			break;
	}
#else
	// windows and *nix: use printf
	puts( buffer );
#endif

#ifdef _WIN32
	// also print to debug console
	OutputDebugStringA( reinterpret_cast<LPCSTR>(buffer));
#endif

	i32 written = prefixSize + messageSize + suffixSize;

	// retur nthe console color if we messed with it
	/*
	 * TODO: make this work correctly (fputs() doesn't flush, but puts() adds \n)
	if ( s_logLevelMode == LOG_LEVEL_MODE_UNIX_TERMINAL ) {
		written += setConsoleColor( buffer, CONSOLE_FG_NORMAL );
		fputs( buffer, stdout );
	}
	*/
	return written;
}

// log at level FINE
i32 Log::f( const char* fmt, ... ) {

	va_list args;
	va_start( args, fmt );

	i32 num = vlog( fmt, LOG_LEVEL_FINE, args );

	va_end( args );

	return num;
}

// log at level INFO
i32 Log::i( const char* fmt, ... ) {

	va_list args;
	va_start( args, fmt );

	i32 num = vlog( fmt, LOG_LEVEL_INFO, args );

	va_end( args );

	return num;
}

// log at level WARN
i32 Log::w( const char* fmt, ... ) {

	va_list args;
	va_start( args, fmt );

	i32 num = vlog( fmt, LOG_LEVEL_WARN, args );

	va_end( args );

	return num;
}

// log at level ERROR
i32 Log::e( const char* fmt, ... ) {

	va_list args;
	va_start( args, fmt );

	i32 num = vlog( fmt, LOG_LEVEL_ERROR, args );

	va_end( args );

	return num;
}

// setConsoleColor
i32 Log::setConsoleColor( char* buffer, LogLevel level ) {
	switch ( level ) {
		case LOG_LEVEL_FINE:
			return setConsoleColor( buffer, CONSOLE_FG_CYAN );
		case LOG_LEVEL_WARN:
			return setConsoleColor( buffer, CONSOLE_FG_YELLOW );
		case LOG_LEVEL_ERROR:
			return setConsoleColor( buffer, CONSOLE_FG_RED );
		case LOG_LEVEL_INFO:
		default:
			return setConsoleColor( buffer, CONSOLE_FG_NORMAL );
	}
	return 0;
}

// setConsoleColor
i32 Log::setConsoleColor( char* buffer, i32 color ) {
	// return sprintf( buffer, "%c[%d;%d;%dm", 0x1B, 0, color, 40 );
	return sprintf( buffer, "%c[%d;%d;%dm", 0x1B, 0, color, 40 );
}

} // namespace cr

#endif // !LOG_MANAGER_CPP_INCLUDED
