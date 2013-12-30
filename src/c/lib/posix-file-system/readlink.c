// readlink.c


#include "../../mythryl-config.h"

#include "system-dependent-unix-stuff.h"

#include <stdio.h>
#include <string.h>

#if HAVE_UNISTD_H
    #include <unistd.h>
#endif

#if HAVE_SYS_STAT_H
    #include <sys/stat.h>
#endif

#if HAVE_LIMITS_H
    #include <limits.h>
#endif

#if HAVE_SYS_PARAM_H
    #include <sys/param.h>
#endif

#include "runtime-base.h"
#include "runtime-values.h"
#include "make-strings-and-vectors-etc.h"
#include "raise-error.h"
#include "cfun-proto-list.h"



// One of the library bindings exported via
//     src/c/lib/posix-file-system/cfun-list.h
// and thence
//     src/c/lib/posix-file-system/libmythryl-posix-file-system.c



Val   _lib7_P_FileSys_readlink   (Task* task,  Val arg)   {
    //========================
    //
    // Mythryl type:  String -> String
    //
    // Read the value of a symbolic link.
    //
    // The following implementation assumes that the system readlink
    // fills the given buffer as much as possible, without nul-termination,
    // and returns the number of bytes copied. If the buffer is not large
    // enough, the return value will be at least the buffer size. In that
    // case, we find out how big the link really is, allot a buffer to
    // hold it, and redo the readlink.
    //
    // Note that the above semantics are not those of POSIX, which requires
    // null-termination on success, and only fills the buffer up to at most 
    // the penultimate byte even on failure.
    //
    // Should this be written to avoid the extra copy, using heap memory?
    //
    // This fn gets bound as   readlink   in:
    //
    //     src/lib/std/src/psx/posix-file.pkg
    //     src/lib/std/src/psx/posix-file-system-64.pkg

									    ENTER_MYTHRYL_CALLABLE_C_FN(__func__);

    struct stat  sbuf;
    int          len;
    int          iresult;

    char* heap_path = HEAP_STRING_AS_C_STRING( arg );

    char  buf[MAXPATHLEN];

    // We cannot reference anything on the Mythryl
    // heap between RELEASE_MYTHRYL_HEAP and RECOVER_MYTHRYL_HEAP
    // because garbage collection might be moving
    // it around, so copy heap_path into C storage: 
    //
    Mythryl_Heap_Value_Buffer  path_buf;
    //
    {   char* c_path
	    = 
	    buffer_mythryl_heap_value( &path_buf, (void*) heap_path, strlen( heap_path ) +1 );		// '+1' for terminal NUL on string.

	RELEASE_MYTHRYL_HEAP( task->hostthread, __func__, NULL );
	    //
	    len = readlink(c_path, buf, MAXPATHLEN);
	    //
	RECOVER_MYTHRYL_HEAP( task->hostthread, __func__ );

	unbuffer_mythryl_heap_value( &path_buf );
    }

    if (len < 0)   return RAISE_SYSERR__MAY_HEAPCLEAN(task, len, NULL);

    if (len < MAXPATHLEN) {
        //
	buf[len] = '\0';
	Val result = make_ascii_string_from_c_string__may_heapclean (task, buf, NULL);
									    EXIT_MYTHRYL_CALLABLE_C_FN(__func__);
	return result;
    }


    // Buffer not big enough.

    // Determine how big the link text is and allot a buffer.

    {   char* c_path
	    = 
	    buffer_mythryl_heap_value( &path_buf, (void*) heap_path, strlen( heap_path ) +1 );		// '+1' for terminal NUL on string.

	RELEASE_MYTHRYL_HEAP( task->hostthread, __func__, NULL );
	    //
	    iresult = lstat (c_path, &sbuf);
	    //
	RECOVER_MYTHRYL_HEAP( task->hostthread, __func__ );

	unbuffer_mythryl_heap_value( &path_buf );
    }

    if (iresult < 0) {
	Val result = RAISE_SYSERR__MAY_HEAPCLEAN(task, iresult, NULL);
									    EXIT_MYTHRYL_CALLABLE_C_FN(__func__);
	return result;
    }

    int nlen = sbuf.st_size + 1;

    char* nbuf = MALLOC(nlen);

    if (nbuf == 0)   return RAISE_ERROR__MAY_HEAPCLEAN(task, "out of malloc memory", NULL);

    // Try the readlink again. Give up on error or if len is still bigger
    // than the buffer size.
    //
    {   char* c_path
	    = 
	    buffer_mythryl_heap_value( &path_buf, (void*) heap_path, strlen( heap_path ) +1 );		// '+1' for terminal NUL on string.

	RELEASE_MYTHRYL_HEAP( task->hostthread, __func__, NULL );
	    //
	    len = readlink(c_path, buf, len);
	    //
	RECOVER_MYTHRYL_HEAP( task->hostthread, __func__ );

	unbuffer_mythryl_heap_value( &path_buf );
    }

    Val result;

    if      (len < 0)		result = RAISE_SYSERR__MAY_HEAPCLEAN(task, len, NULL);
    else if (len >= nlen)	result = RAISE_ERROR__MAY_HEAPCLEAN(task, "readlink failure", NULL);
    else {
	nbuf[len] = '\0';
	result = make_ascii_string_from_c_string__may_heapclean (task, nbuf, NULL);
	FREE (nbuf);
    }
									    EXIT_MYTHRYL_CALLABLE_C_FN(__func__);
    return result;
}


// COPYRIGHT (c) 1995 by AT&T Bell Laboratories.
// Subsequent changes by Jeff Prothero Copyright (c) 2010-2014,
// released per terms of SMLNJ-COPYRIGHT.

