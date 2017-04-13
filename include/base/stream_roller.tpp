#ifndef __CR_BASE_STREAM_ROLLER_TPP_
#define __CR_BASE_STREAM_ROLLER_TPP_

#include <memory>
#include <vector>
#include <utility>

#include <roller/core/types.h>
#include <roller/core/exception.h>

#include "base/serializable.h"

using namespace roller;
using std::vector;
using std::pair;
using std::unique_ptr;

namespace cr {

// inflate
template <typename TYPE>
TYPE inflate( void* pointer ) {
	TYPE obj;
	obj.internalize( pointer );
	return obj;
}

// getObject
template <typename TYPE>
TYPE getObject( const vector<pair<i32, i64>>& contents, void* pointer, i64 itemIndex ) {

	if ( itemIndex >= contents.size() ) {
		throw RollerException( "Index out of bounds (%lld >= %d)", itemIndex, contents.size() );
	}

	i64 offset = contents[itemIndex].second;

	return inflate<TYPE>( ((ui8*)pointer) + offset );
}

}

#endif
