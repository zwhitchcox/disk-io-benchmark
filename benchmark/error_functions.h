/* error_functions.h

   Header file for error_functions.c.
*/
#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

#ifdef DEBUG
#define errExit(...) printf("Error at %s:%d\n", __FILE__, __LINE__); _errExit(__VA_ARGS__);
#define errExitEN(...) printf("Error at %s:%d\n", __FILE__, __LINE__); _errExitEN(__VA_ARGS__);

#else
#define errExit(...) _errExit(__VA_ARGS__);
#define errExitEN(...) _errExitEN(__VA_ARGS__);
#endif




/* Error diagnostic routines */


void errMsg(const char *format, ...);

#ifdef __GNUC__

    /* This macro stops 'gcc -Wall' complaining that "control reaches
       end of non-void function" if we use the following functions to
       terminate main() or some other non-void function. */

#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

void _errExit(const char *format, ...) NORETURN ;

void err_exit(const char *format, ...) NORETURN ;

void _errExitEN(int errnum, const char *format, ...) NORETURN ;

void fatal(const char *format, ...) NORETURN ;

void usageErr(const char *format, ...) NORETURN ;

void cmdLineErr(const char *format, ...) NORETURN ;

#endif