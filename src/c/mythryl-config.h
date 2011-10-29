// mythryl-config.h
//
// The architectural idea here is to have
//
//     config.h
//
// (which is autogenerated by the GNU autoconfig toolset)
// record which resources are available on the build machine,
// and then record in this file manual policy decisions such
// as whether to take advantage of a given resource.
//
// Thus, we #include config.h here, and then #include (only)
//
//     mythryl-config.h
//
// in all our C source files.
//
// By convention symbols defined in config.h start with the prefix
//
//     HAVE_
//
// whereas symbols defined in mythryl-config.h start with the prefix
//
//     WANT_
//
//                -- 2011-10-28 CrT

#ifndef MYTHRYL_CONFIG_H
#define MYTHRYL_CONFIG_H 1

#include "config.h"


#endif // MYTHRYL_CONFIG_H
