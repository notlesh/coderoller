#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED

#include <memory>
#include <stdexcept>
#include <string>

namespace cr {

using std::exception;
using std::string;
using std::unique_ptr;

/**
 * Base CRException class
 */
class CRException : public exception {

	public:

		/**
		 * Empty constructor
		 */
		CRException();

		/**
		 * Copy constructor
		 */
		CRException( const CRException& other ) = delete;

		/**
		 * Move constructor
		 */
		CRException( CRException&& other );

		/**
		 * Corstructor. Accepts printf style varargs.
		 */
		CRException( const char* fmt, ... );

		/**
		 * Constructor
		 */
		CRException( const char* fmt, va_list args );

		/**
		 * Virtual destructor
		 */
		virtual ~CRException() throw();

		/**
		 * What?!
		 */
		virtual const char* what() const throw();

		/**
		 * Formats the exception with the given fmt and va_list (delegates to sprintf)
		 */
		virtual void format( const char* fmt, va_list args ) throw();

		/**
		 * Formats the exception with the given fmt and elipses
		 */
		virtual void format( const char* fmt, ... ) throw();

	protected:

		unique_ptr<char[]> _what;
};

} // namespace cr

#endif // !EXCEPTION_H_INCLUDED
