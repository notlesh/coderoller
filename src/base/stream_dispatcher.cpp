#include "base/stream_dispatcher.h"

#include "core/log.h"

#include "base/stream_roller.h"

using namespace roller;
using std::map;
using std::function;

namespace cr {

// Constructor
StreamObjectDispatcher::StreamObjectDispatcher() :
				_toleranceType(MissingHandlerTolerance::THROW) {
}

// Destructor
StreamObjectDispatcher::~StreamObjectDispatcher() {
}

// setHandler
void StreamObjectDispatcher::setHandler( i32 classHash, const HandlerFunction& handler ) {
	_handlers[classHash] = handler;
}

// clearHandler
void StreamObjectDispatcher::clearHandler( i32 classHash ) {
	_handlers.erase( classHash );
}

// setMissingHandlerTolerance
void StreamObjectDispatcher::setMissingHandlerTolerance( const MissingHandlerTolerance& type ) {
	_toleranceType = type;
}

// getMissingHandlerTolerance
const MissingHandlerTolerance& StreamObjectDispatcher::getMissingHandlerTolerance() const {
	return _toleranceType;
}

// handleStream
void StreamObjectDispatcher::handleStream( void* ptr ) {

	auto objectList = listStreamContents( ptr );

	for ( auto objectItem : objectList ) {

		i32 classHash = objectItem.first;
		auto itr = _handlers.find( classHash );

		if ( itr == _handlers.end() ) {

			switch ( _toleranceType ) {
				case MissingHandlerTolerance::THROW:
					throw RollerException( "Missing stream handler for object type %u", classHash );

				case MissingHandlerTolerance::WARN:
					Log::w( "Missing stream handler for object type %u (ignoring object)", classHash );
					break;

				case MissingHandlerTolerance::SILENT:
					Log::f( "Silently ignoring handler for object type %u", classHash );
					break;
			}

			continue;
		}

		auto handler = itr->second;
		handler( classHash, ((ui8*)ptr) + objectItem.second );
	}
}

}

