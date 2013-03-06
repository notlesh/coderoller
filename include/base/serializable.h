#ifndef __CR_BASE_SERIALIZABLE_H_
#define __CR_BASE_SERIALIZABLE_H_

#include "core/types.h"

using namespace roller;

namespace cr {

class Serializable {

public:

	/**
	 * Virtual destructor.
	 */
	virtual ~Serializable() { }

	/**
	 * Serialize this class. Caller should provide a pointer to an address where the class 
	 * should be written, and should take care to ensure that there is sufficient space
	 * allocated to do so.
	 *
	 * The number of bytes written should be returned. This can be determined ahead of time
	 * by calling getSerializedSize()
	 */
	virtual i64 serialize( void* buffer ) const = 0;

	/**
	 * Return the serialized size of the object. This should return the exact amount of 
	 * space required to serialize it.
	 */
	virtual i64 getSerializedSize() const = 0;

	/**
	 * Internalize the serialized stream. The implementation should initialize all serialized 
	 * members in such a manner as to reciprocate the serialize() function.
	 *
	 * Should return the number of bytes used from the stream to internalize.
	 */
	virtual i64 internalize( void* buffer ) = 0;

	/**
	 * Return a hash code for the class that uniquely identifies it. If the class definition changes 
	 * such that its serialization semantics change, this hash should also change.
	 */
	virtual i32 getClassHash() const = 0;
	
};

}

#endif
