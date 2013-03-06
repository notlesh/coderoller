#ifndef __CR_BASE_STREAM_DISPATCHER_H_
#define __CR_BASE_STREAM_DISPATCHER_H_

#include <map>
#include <functional>

#include "core/types.h"

using namespace roller;
using std::map;
using std::function;

namespace cr {

// a standalone function matching this typedef would have the given signature:
// void handle( i32 classHash, void* ptr );
typedef function<void(i32, void*)> HandlerFunction;

/**
 * Enum to represent the different tolerance types for missing handlers. This is relevant when a
 * stream contains an object that has no handler type. The default is to throw.
 */
enum class MissingHandlerTolerance {
	THROW,
	WARN,
	SILENT
};

/**
 * The StreamObjectDispatcher class is a helper class to take messages and dispatch them to handlers. 
 * The workflow goes like this:
 *
 * 1) create a StreamObjectDispatcher
 * 2) configure the handlers for different object types (using their class hash to identify them)
 * 3) send stream contents to the class via handleStream() function
 *
 * The class will take the stream and dispatch each object in it to the provided handler. The class can be
 * configured (via setMissingHandlerTolerance()) to fail hard when an object is received that doesn't have
 * a handler, to print a warning message, or to silintly ignore it.
 *
 * The handlers should take responsibility of the object's type. This can be conveniently done with the 
 * Stream Roller utility function inflate(), which takes as a template parameter the object type and as a
 * function parameter the pointer to where the object's serialization stream is..
 */
class StreamObjectDispatcher {

public:

	/**
	 * Constructor
	 */
	StreamObjectDispatcher();

	/**
	 * Destructor
	 */
	virtual ~StreamObjectDispatcher();

	/**
	 * Sets the handler for an object.
	 *
	 * @param classHash is the class hash of the object type the handler is provided for.
	 * @param handler is a callable that will handle the object type
	 */
	void setHandler( i32 classHash, const HandlerFunction& handler );

	/**
	 * Clears the handler for a given object type
	 *
	 * @param classHash is the class hash of the object type for which any handler will be cleared
	 */
	void clearHandler( i32 classHash );

	/**
	 * Set the missing handler tolerance type. The default is to throw an exception.
	 *
	 * @param type is the type of tolerance
	 */
	void setMissingHandlerTolerance( const MissingHandlerTolerance& type );

	/**
	 * Returns the current missing handler tolerance type.
	 *
	 * @return the current missing handler tolerance type.
	 */
	const MissingHandlerTolerance& getMissingHandlerTolerance() const;

	/**
	 * Handle a stream. This should be called to begin stream evaluation and delegation.
	 *
	 * @param ptr is the pointer to the stream contents
	 */
	void handleStream( void* ptr );

private:

	map<i32, HandlerFunction> _handlers;
	MissingHandlerTolerance _toleranceType;
	
};

}

#endif
