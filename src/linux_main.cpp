#include "core/types.h"

#include "meta.h"
#include "cr_constants.h"
#include "generate.h"

using namespace roller;
using namespace cr;

// main
i32 main( i32 argc, char** argv ) {

	// XXX: testing
	Class c;
	c._name = "TestClass";

	Field f1;
	f1._name = "temp";
	f1._dataType = DataType::F32;
	f1._defaultValue = "-1.0";

	Field f2;
	f2._name = "time";
	f2._dataType = DataType::I64;
	f2._defaultValue = "-1";

	c._fields.push_back( f1 );
	c._fields.push_back( f2 );

	Package p;
	p._classes.push_back( c );

	GenConfig config;
	config._package = p;
	config._languages.push_back( Language::CPP );

	generateAll( config );

	return 0;
}
