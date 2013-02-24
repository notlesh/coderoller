#ifndef __CR_META_H_
#define __CR_META_H_

#include <string>
#include <list>

#include "core/file.h"

#include "cr_constants.h"

using namespace roller;
using std::string;
using std::list;

namespace cr {

/**
 * Struct representing a field
 */
struct Field {

	string _name;
	DataType _dataType;
	string _defaultValue;

};

/**
 * Struct representing a class.
 */
struct Class {

	string _name;
	list<Field> _fields;

};

/**
 * A package contains a list of classes to be generated.
 */
struct Package {
	list<Class> _classes;
};

/**
 * GenConfig contains the info needed to generate a package.
 */
struct GenConfig {

	Package _package;
	list<Language> _languages;
	File _outputDir;
};

};

#endif
