#ifndef __CR_CPP_UTIL_H_
#define __CR_CPP_UTIL_H_

#include <memory>
#include <iostream>

#include "languages/generator.h"
#include "cr_constants.h"
#include "meta.h"

using std::shared_ptr;
using std::ostream;

namespace cr {

/**
 * Return the C++ data type string for a given DataType
 */
const char* getDataTypeName( DataType type );

/**
 * Return the C++ access specifier for the given AccessPrivacy
 */
const char* getPrivacyName( AccessPrivacy privacy );

/**
 * Resolve the desired access privacy for the given field
 */
AccessPrivacy resolvePrivacy( const Package& p, const Class& c, const Field& f );

/**
 * Resolve the desired serializable for the given class
 */
bool resolveClassSerializable( const Package& p, const Class& c );

/**
 * Resolve the desired serializable for the given field
 */
bool resolveFieldSerializable( const Package& p, const Class& c, const Field& f );

};

#endif
