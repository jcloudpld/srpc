#ifndef SRPC_CONFIG_MSVC_H
#define SRPC_CONFIG_MSVC_H

#ifdef _MSC_VER
#  pragma once
#endif

// MSVC configuration

#ifdef SRPC_DLL
#   ifdef SRPC_EXPORTS
#      define SRPC_API __declspec(dllexport)
#   else
#      define SRPC_API __declspec(dllimport)
#   endif
// struct 'XXX' needs to have dll-interface to be used by clients of class 'XX'
#   pragma warning (disable: 4251)

//non dll-interface class 'XXX' used as base for dll-interface class 'XXX'
#   pragma warning (disable: 4275)
#else
#   define SRPC_API
#endif // SRPC_DLL

/// inline함수로 이뤄진 클래스를 export할 때 사용
#define SRPC_API_INLINE

// 4511: copy constructor could not be generated
// 4512: assignment operator could not be generated
#pragma warning (disable: 4511 4512)

#endif // SRPC_CONFIG_MSVC_H
