#include "languages/cpp/cpp_generator.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>

#include "languages/cpp/cpp_util.h"
#include "languages/cpp/cpp_class_generator.h"
#include "cr_constants.h"
#include "cr_util.h"

using std::unique_ptr;
using std::shared_ptr;
using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::endl;

namespace cr {

// Constructor
CPPGenerator::CPPGenerator() {
}

// Destructor
CPPGenerator::~CPPGenerator() {
}

// generate
void CPPGenerator::generate( shared_ptr<GenConfig> config ) {

	for ( Class c : config->_package._classes ) {

		char hFilePath[128]; 
		snprintf( hFilePath, 128, "%s.gen.h", c._name.c_str() );

		char cppFilePath[128]; 
		snprintf( cppFilePath, 128, "%s.gen.cpp", c._name.c_str() );

		CPPClassGenerator generator;
		generator.generate( 
				config, 
				c,
				"include/",
				hFilePath,
				"src/",
				cppFilePath );

	}
}

};
