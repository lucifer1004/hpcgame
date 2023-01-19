#ifndef MACROS_H
#define MACROS_H

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define AITISA_API_EXPORT __declspec(dllexport)
  #define AITISA_API_IMPORT __declspec(dllimport)
  #define AITISA_API_INTERNAL
#else
  #if __GNUC__ >= 4
    #define AITISA_API_EXPORT __attribute__ ((visibility ("default")))
    #define AITISA_API_IMPORT __attribute__ ((visibility ("default")))
    #define AITISA_API_INTERNAL __attribute__ ((visibility ("hidden")))
  #else
    #define AITISA_API_EXPORT
    #define AITISA_API_IMPORT
    #define AITISA_API_INTERNAL
  #endif
#endif

// Now we use the generic helper definitions above to define AITISA_API_PUBLIC
// and AITISA_API_LOCAL. AITISA_API_PUBLIC is used for the public API symbols.
// It either DLL imports or DLL exports (or does nothing for static build)
// AITISA_API_LOCAL is used for non-api symbols.

#ifdef AITISA_API_SHARED_LIBS  // defined if AITISA_API is compiled as a shared
                               // library
#ifdef AITISA_API_SHARED_LIBS_EXPORTS  // defined if we are building the
                                       // AITISA_API shared library (instead of
                                       // using it)
#define AITISA_API_PUBLIC AITISA_API_EXPORT
#else
#define AITISA_API_PUBLIC AITISA_API_IMPORT
#endif  // AITISA_API_SHARED_LIB_EXPORTS
#define AITISA_API_LOCAL AITISA_API_INTERNAL
#else  // AITISA_API_SHARED_LIB is not defined: this means AITISA_API is a
       // static lib.
#define AITISA_API_PUBLIC
#define AITISA_API_LOCAL
#endif // AITISA_API_DLL

#endif