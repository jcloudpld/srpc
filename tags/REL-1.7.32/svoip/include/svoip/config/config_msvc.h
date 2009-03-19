#ifndef SVOIP_CONFIG_MSVC_H
#define SVOIP_CONFIG_MSVC_H

// MSVC configuration

#ifdef SVOIP_DLL
#   ifdef SVOIP_EXPORTS
#      define SVOIP_API __declspec(dllexport)
#   else
#      define SVOIP_API __declspec(dllimport)
#   endif
// struct 'XXX' needs to have dll-interface to be used by clients of class 'XX'
#   pragma warning (disable: 4251)

//non dll-interface class 'XXX' used as base for dll-interface class 'XXX'
#   pragma warning (disable: 4275)
#else
#   define SVOIP_API
#endif // SVOIP_DLL

/// inline함수로 이뤄진 클래스를 export할 때 사용
#define SVOIP_API_INLINE

// 4511: copy constructor could not be generated
// 4512: assignment operator could not be generated
#pragma warning (disable: 4511 4512)

#endif // SVOIP_CONFIG_MSVC_H
