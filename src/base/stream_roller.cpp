#include "base/stream_roller.h"

#include "core/aver.h"

#include "cr_constants.h"

using namespace roller;
using std::list;
using std::pair;
using std::unique_ptr;

namespace cr {

// listStreamContents
list<pair<i32, i64>> listStreamContents( void* pointer ) {
	Log::i( "Listing stream contents (%p)...", pointer );

	ui8* buffer = (ui8*)pointer;

	// do some validation
	if ( buffer[0] != 0x73 || buffer[1] != 0x72 ) {
		throw RollerException( "Not a valid StreamRoller stream (invalid begin transmission designation)" );
	}

	if ( (buffer[3] != (ui8)CR_MAJOR_VERSION)
			|| (buffer[4] != (ui8)CR_MINOR_VERSION)
			|| (buffer[5] != (ui8)CR_PATCH_VERSION) ) {

		throw RollerException( "Stream is of a different version" );
	}

	ui32* numberAddress = (ui32*)(buffer + 14);
	ui32 numberOfObjects = *numberAddress;

	Log::i( "  - number of objects: %d", numberOfObjects );

	i64 objectOffset = 18;
	
	list<pair<i32, i64>> objects;
	for ( ui32 i=0; i<numberOfObjects; i++ ) {

		i64* objectSizeAddress = (i64*)(buffer + objectOffset);
		i64 objectSize = *objectSizeAddress;

		i32* objectDefinitionHashAddress = (i32*)(buffer + objectOffset + sizeof(i32));
		i32 objectDefinitionHash = *objectDefinitionHashAddress;

		Log::i( " - object: %d : %ld", objectDefinitionHash, objectSize );

		objects.push_back( pair<i32, i64>( objectDefinitionHash, objectSize ));
	}

	return objects;
}

// createStream
pair<i64, unique_ptr<ui8[]>> createStreamContents( const list<Serializable*> objects ) {

	i64 size = 2 + 4 + 8 + 4 + 2; // all fields other than objects themselves
	for ( Serializable* object : objects ) {
		size += object->getSerializedSize() + 8 + 4; // 8 bytes for size, 4 bytes for hash
	}

	ui8* pointer = new ui8[size];
	unique_ptr<ui8[]> buffer( pointer );

	// fill in buffer...

	// beginning designation
	buffer[0] = 0x73;
	buffer[1] = 0x72;

	// CR version
	buffer[2] = 0x00;
	buffer[3] = (ui8)CR_MAJOR_VERSION;
	buffer[4] = (ui8)CR_MINOR_VERSION;
	buffer[5] = (ui8)CR_PATCH_VERSION;

	// TODO: git hash
	// byte[6 - 13]

	// number of objects
	ui32* numberAddress = (ui32*)(pointer + 14);
	*numberAddress = objects.size();

	i64 objectOffset = 18;

	for ( Serializable* object : objects ) {

		i64 objectSize = object->getSerializedSize();
		i32 objectDefinitionHash = object->getClassHash();

		i64* objectSizeAddress = (i64*)(pointer + objectOffset);
		*objectSizeAddress = objectSize;

		i32* objectDefinitionHashAddress = (i32*)(pointer + objectOffset + sizeof(i64));
		*objectDefinitionHashAddress = objectDefinitionHash;

		i64 written = object->serialize( (void*)(pointer + objectOffset + sizeof(i64) + sizeof(i32)) );

		if ( written != objectSize ) {
			throw RollerException( 
					"Serialized object did not write exactly its size (size=%ld, written=%ld)",
					objectSize,
					written );
		}

		objectOffset += (objectSize + sizeof(i64) + sizeof(i32));

	}

	// end designation
	buffer[objectOffset] = 0x73;
	buffer[objectOffset + 1] = 0x72;

	return pair<i64, unique_ptr<ui8[]>>( size, std::move(buffer) );

}

}
