#include <string.h>

#include "core/types.h"
#include "core/log.h"
#include "core/file.h"

#include "meta.h"
#include "cr_constants.h"
#include "cr_globals.h"
#include "generate.h"
#include "parse/parse.h"

using namespace cr;

void printUsage();
i32 parseCL( i32 argc, char** argv );

// main
i32 main( i32 argc, char** argv ) {

	try {

		i32 error = parseCL( argc, argv );
		if ( error ) {

			// print usage case
			if (  error == -1 ) {
				return 0;
			}

			return error;
		}

		shared_ptr<GenConfig> config = parseXML( argv[1] );
		config->_outputDir = g_outputDir;
		generateAll( config );

		Log::i( "coderoller exiting 0" );

		return 0;

	} catch ( const exception& e ) {
		Log::i( "std::exception caught in main: %s", e.what() );
		return 1;
	} catch ( ... ) {
		Log::i( "unrecognized exception caught in main" );
	}
}

// parseCL
i32 parseCL( i32 argc, char** argv ) {

	// --help -h --usage
	if ( argc == 2 
			&& ((strcmp( argv[1], "--help" ) == 0) 
			|| (strcmp( argv[1], "-h" ) == 0)
			|| (strcmp( argv[1], "--usage" ) == 0))) {
		printUsage();
		return -1;
	}

	if ( argc < 3 ) {
		printUsage();
		return 1;
	}

	g_inputFile = File( argv[1] );
	if ( ! g_inputFile.exists() && ! g_inputFile.isDir() ) {
		throw CRException( "Invalid input file %s", argv[1] );
	}

	g_outputDir = File( argv[2] );
	if ( ! g_outputDir.exists() && ! g_outputDir.isDir() ) {
		throw CRException( "Invalid output directory %s", argv[2] );
	}

	return 0;
}

// printUsage
void printUsage() {

	Log::i( "coderoller code generator (c) Stephen Shelton" );
	Log::i( "  version %d.%d", CR_MAJOR_VERSION, CR_MINOR_VERSION );
	Log::i( "  usage: coderoller [input file] [output base dir]" );
}
