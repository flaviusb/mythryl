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
//     NEED_
//
//                -- 2011-10-28 CrT

#ifndef MYTHRYL_CONFIG_H
#define MYTHRYL_CONFIG_H 1

#include "config.h"


#define NEED_SOFTWARE_GENERATED_PERIODIC_EVENTS 1
    //
    // The Mythryl heapcleaner ("garbage collector") runs as a cooperative
    // thread with user code -- the Mythryl compiler ensures that every closed
    // loop through the code calls the garbage collector probe at least once.
    //
    // This facility takes advantage of that to allow invocation of arbitrary
    // user code on a regular basis.  This provides an alternative to (say)
    // using kernel-generated SIGVTALRM calls, which have the disadvantages
    // of higher overhead, and of interrupting "slow" system calls, which must
    // then be explicitly restarted -- something which many C library functions
    // probably do not do correctly.  

#define NEED_PTHREAD_SUPPORT 0
    //
    // Define this as TRUE (nonzero) to compile in support
    // for multiple posix threads running Mythryl code in
    // parallel in the same address space.  For background
    // see    src/A.MULTICORE-SUPPORT.OVERVIEW

// Max number of posix threads running Mythryl.
// We dedicate half a meg or so of memory to each
// one, so don't be prolifigate here:
//
#if !NEED_PTHREAD_SUPPORT
    //
    #define MAX_PTHREADS	1
#else
    #define MAX_PTHREADS	8
#endif



#define MAX_POSIX_SIGNALS      60
    //
    // This needs to be at least one more than the
    // highest valid signal number on the host system.
    //
    // This value gets used in    src/c/h/pthread-state.h
    // to size the                posix_signal_counts []
    // vector and in              src/c/main/runtime-state.c
    // and                        src/c/machine-dependent/signal-stuff.c
    // to iterate over that vector.
    // 
    // The proper value for this symbol is typically about 30
    // and doesn't change much, so just hardwiring it to 60 should be
    // reasonably safe, if not particularly elegant.
    //
    // (I'm reluctant to autoconfig it at compile time based on (say)
    // `kill -l` because we might well (say) upgrade to a kernel supporting
    // more signals and wind up getting weird memory corruption bugs
    // as a result.)
    //                                         -- 2011-10-30 CrT


#endif // MYTHRYL_CONFIG_H
