#ifndef __CR_CR_UTIL_H_
#define __CR_CR_UTIL_H_

#include "cr_constants.h"

namespace cr {

/**
 * Returns the DataType from the given input
 */
DataType toDataType( const char* str );

/**
 * Returns the Language from the given input
 */
Language toLanguage( const char* str );

/**
 * Returns the AccessPrivacy from the given input
 */
AccessPrivacy toAccessPrivacy( const char* str );

/**
 * Returns the SerializableSpecification from the given input
 */
SerializableSpecification toSerializableSpecification( const char* str );

/**
 * Capitalize a letter
 */
char capitalize( char c );

};

#endif
