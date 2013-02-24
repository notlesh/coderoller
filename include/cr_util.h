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

};

#endif
