#ifndef __CR_PARSE_H_
#define __CR_PARSE_H_

#include <memory>

#include <roller/core/exception.h>
#include "tinyxml2.h"

#include "meta.h"

using namespace roller;
using namespace tinyxml2;
using std::shared_ptr;

namespace cr {

/**
 * Parse XML document
 */
shared_ptr<GenConfig> parseXML( const char* filename );

/**
 * Parse a class XML element
 */
Class parseClass( XMLElement* classElement );

/**
 * Parse a field XML element
 */
Field parseField( XMLElement* fieldElement, const Class& c );

}

#endif
