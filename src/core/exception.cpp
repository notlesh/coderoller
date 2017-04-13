#include "core/exception.h"

#include <stdarg.h>

namespace cr {

// Constructor
CRException::CRException() :
				_what(new char[4096]) {
}

// Constructor
CRException::CRException( CRException&& other ) :
				_what( std::move( other._what )) {
}

// Constructor
CRException::CRException( const char* fmt, ... ) :
				_what(new char[4096]) {


	va_list args;
	va_start( args, fmt );

	format( fmt, args );

	va_end( args );

}

// Constructor
CRException::CRException( const char* fmt, va_list args ) :
				_what(new char[4096]) {
	format( fmt, args );
}

// Destructor
CRException::~CRException() throw() {
}

// what
const char* CRException::what() const throw() {
	return _what.get();
}

// format
void CRException::format( const char* fmt, va_list args ) throw() {
	vsprintf( _what.get(), fmt, args );
}

// format
void CRException::format( const char* fmt, ... ) throw() {
	va_list args;
	va_start( args, fmt );

	format( fmt, args );

	va_end( args );

} // namespace cr

}
