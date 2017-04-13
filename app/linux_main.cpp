#include <string.h>
#include <chrono>
#include <iostream>

#include "core/types.h"
#include "core/log.h"
#include "core/file.h"

#include "meta.h"
#include "cr_constants.h"
#include "cr_globals.h"
#include "generate.h"
#include "parse/parse.h"
#include "cr_util.h"

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

		typedef std::chrono::high_resolution_clock Clock;
		auto start = Clock::now();

		// parse XML
		shared_ptr<GenConfig> config = parseXML( argv[1] );

		auto now = Clock::now();
		std::chrono::nanoseconds parseTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start);
		start = now;

		config->_outputDir = g_outputDir;

		// generate code
		generateAll( config );

		now = Clock::now();
		std::chrono::nanoseconds genTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start);

		double parseTimeSec = (double)parseTime.count() / 1000000000.0;
		double genTimeSec = (double)genTime.count() / 1000000000.0;

		std::cout.precision(8);
		std::cout << "--- Stats ---" << std::endl;
		std::cout << "Parse time: " << std::fixed << parseTimeSec << " seconds" << std::endl;
		std::cout << "Gen time: " << std::fixed << genTimeSec << " seconds" << std::endl;
		std::cout << "Total time: " << std::fixed << (parseTimeSec + genTimeSec) << " seconds" << std::endl;

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
	if (! g_outputDir.exists()) {
		// our File class doesn't do too well with non-existent files, esp. when it comes to mkdir
		// g_outputDir.mkdir(true);

		// ...so just make a system call...
		char buffer[4096];
		snprintf(buffer, sizeof(buffer), "mkdir %s", argv[2]);
		system(buffer); // exec "mkdir <argv[2]>"
	}
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
