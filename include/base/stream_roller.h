#ifndef __CR_BASE_STREAM_ROLLER_H_
#define __CR_BASE_STREAM_ROLLER_H_

#include <memory>
#include <list>
#include <utility>

#include "core/types.h"

#include "base/serializable.h"

using namespace roller;
using std::list;
using std::pair;
using std::unique_ptr;

namespace cr {

/**
 * The StreamRoller protocol is a relatively simple protocol for serializing and deserializing data. This
 * could be used for sending and receiving serialized objects over a network or to and from a binary file
 * as well.
 *
 * The protocol is visually described in doc/encoding. A transmission contains some information about the
 * version of StreamRoller used as well as the number of objects it contains. It also has some simple 2-byte
 * designations to denote the beginning and end (and help identify the data as a StreamRoller stream.)
 *
 * The stream is broken up as follows:
 * 
 * - 2 bytes 0x7372 ("sr") to designate the beginning of a StreamRoller stream
 * - 4 bytes to designate the version of StreamRoller
 * - 8 bytes to designate the git hash of CodeRoller (first 4 and last 4 of the hash)
 * - 4 bytes to designate the number of objects in the stream
 * - Any number of objects, broken up as:
 *   - 8 bytes to designate the size of the object
 *   - 4 bytes to designate the object definition hash
 *   - The serialized object
 * - 2 bytes 0x7372 ("sr") to designate the end of the StreamRoller stream
 *
 *
 * The rest of this file contains utilities to help pull data out of this stream and create a stream.
 *
 */

/**
 * List the stream contents. This takes a pointer to the beginning of a StreamRoller transmission stream and
 * will return an ordered list of the object definition hashes in the stream paired with the offset into the
 * stream at which the object can be found.
 *
 * @return list of pairs of stream contents. The pairs are [object definition hash] [offset to object]. 
 * The offset will point directly to the size of the object (see above).
 */
list<pair<i32, i64>> listStreamContents( void* pointer );

/** 
 * Create a data stream. This will return a new buffer (inside a unique_ptr) with the contents of the stream.
 *
 * The returned pair has the size of the buffer and the buffer itself. The size will be the size of the 
 * actual Stream, regardless of the allocated buffer size.
 */
pair<i64, unique_ptr<ui8[]>> createStreamContents( const list<Serializable*> objects );

}

#endif
