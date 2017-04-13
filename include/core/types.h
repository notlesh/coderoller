#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <memory>
#include <string>

using std::unique_ptr;
using std::string;

namespace cr {

// type defs
typedef signed char i8;
typedef unsigned char ui8;
typedef signed short int i16;
typedef unsigned short int ui16;
typedef signed int i32;
typedef unsigned int ui32;
typedef signed long long i64;
typedef unsigned long long ui64;
typedef float f32;
typedef double f64;

} // namespace cr

#endif // !TYPES_H_INCLUDED
