#ifndef NSRPC_CONFIG_MSVC_H
#define NSRPC_CONFIG_MSVC_H

// MSVC configuration

#ifdef NSRPC_DLL
#   ifdef NSRPC_EXPORTS
#      define NSRPC_API __declspec(dllexport)
#   else
#      define NSRPC_API __declspec(dllimport)
#   endif
// struct 'XXX' needs to have dll-interface to be used by clients of class 'XX'
#   pragma warning (disable: 4251)

//non dll-interface class 'XXX' used as base for dll-interface class 'XXX'
#   pragma warning (disable: 4275)
#else
#   define NSRPC_API
#endif // NSRPC_DLL

#endif // NSRPC_CONFIG_MSVC_H
