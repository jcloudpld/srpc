#ifndef NSRPC_LOGGER_H
#define NSRPC_LOGGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#if defined (_MSC_VER)
#  pragma warning ( push )
#  pragma warning ( disable: 4127 4244 4267 4311 4312 4996)
#endif
#include <ace/Log_Msg.h>
#if defined (_MSC_VER)
#  pragma warning ( pop )
#endif

/** @addtogroup utility
* @{
*/

// = Log-related Macros (excerpts from ACE)

#if defined (ACE_NLOGGING)
    // = Debug
#   define NSRPC_LOG_DEBUG(A) { A; }
#   define NSRPC_LOG_DEBUG2(A,B) { A, B; }
#   define NSRPC_LOG_DEBUG3(A,B,C) { A, B, C; }
#   define NSRPC_LOG_DEBUG4(A,B,C,D) { A, B, C, D; }
#   define NSRPC_LOG_DEBUG5(A,B,C,D,E) { A, B, C, D, E; }
#   define NSRPC_LOG_DEBUG6(A,B,C,D,E,F) { A, B, C, D, E, F; }
#   define NSRPC_LOG_DEBUG7(A,B,C,D,E,F,G) { A, B, C, D, E, F, G; }
#   define NSRPC_LOG_DEBUG8(A,B,C,D,E,F,G,H) { A, B, C, D, E, F, G, H; }
#   define NSRPC_LOG_DEBUG9(A,B,C,D,E,F,G,H,I) { A, B, C, D, E, F, G, H, I; }
#   define NSRPC_LOG_DEBUG10(A,B,C,D,E,F,G,H,I,J) \
    { A, B, C, D, E, F, G, H, I, J; }
    // = Info
#   define NSRPC_LOG_INFO(A) { A; }
#   define NSRPC_LOG_INFO2(A,B) { A, B; }
#   define NSRPC_LOG_INFO3(A,B,C) { A, B, C; }
#   define NSRPC_LOG_INFO4(A,B,C,D) { A, B, C, D; }
#   define NSRPC_LOG_INFO5(A,B,C,D,E) { A, B, C, D, E; }
#   define NSRPC_LOG_INFO6(A,B,C,D,E,F) { A, B, C, D, E, F; }
#   define NSRPC_LOG_INFO7(A,B,C,D,E,F,G) { A, B, C, D, E, F, G; }
    // = Warning
#   define NSRPC_LOG_WARNING(A) { A; }
#   define NSRPC_LOG_WARNING2(A,B) { A, B; }
#   define NSRPC_LOG_WARNING3(A,B,C) { A, B, C; }
#   define NSRPC_LOG_WARNING4(A,B,C,D) { A, B, C, D; }
#   define NSRPC_LOG_WARNING5(A,B,C,D,E) { A, B, C, D, E; }
    // = Error
#   define NSRPC_LOG_ERROR(A) { A; }
#   define NSRPC_LOG_ERROR2(A,B) { A, B; }
#   define NSRPC_LOG_ERROR3(A,B,C) { A, B, C; }
#   define NSRPC_LOG_ERROR4(A,B,C,D) { A, B, C, D; }
#   define NSRPC_LOG_ERROR5(A,B,C,D,E) { A, B, C, D, E; }
#   define NSRPC_LOG_ERROR6(A,B,C,D,E,F) { A, B, C, D, E, F; }
#   define NSRPC_LOG_ERROR7(A,B,C,D,E,F,G) { A, B, C, D, E, F, G; }
#   define NSRPC_LOG_ERROR8(A,B,C,D,E,F,G,H) { A, B, C, D, E, F, G, H; }

#else // defined (ACE_NLOGGING)
    // = Debug
#   define NSRPC_LOG_DEBUG(A) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n")); \
    }
#   define NSRPC_LOG_DEBUG2(A,B) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n"),B); \
    }
#   define NSRPC_LOG_DEBUG3(A,B,C) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n"),B,C); \
    }
#   define NSRPC_LOG_DEBUG4(A,B,C,D) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n"),B,C,D); \
    }
#   define NSRPC_LOG_DEBUG5(A,B,C,D,E) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n"),B,C,D,E); \
    }
#   define NSRPC_LOG_DEBUG6(A,B,C,D,E,F) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F); \
    }
#   define NSRPC_LOG_DEBUG7(A,B,C,D,E,F,G) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F,G); \
    }
#   define NSRPC_LOG_DEBUG8(A,B,C,D,E,F,G,H) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F,G,H); \
    }
#   define NSRPC_LOG_DEBUG9(A,B,C,D,E,F,G,H,I) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F,G,H,I); \
    }
#   define NSRPC_LOG_DEBUG10(A,B,C,D,E,F,G,H,I,J) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, \
        ACE_TEXT("DEBUG|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F,G,H,I,J); \
    }
    // = Info
#   define NSRPC_LOG_INFO(A) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, \
        ACE_TEXT("INFO|%D|%t|") A ACE_TEXT("\n")); \
    }
#   define NSRPC_LOG_INFO2(A,B) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, \
        ACE_TEXT("INFO|%D|%t|") A ACE_TEXT("\n"),B); \
    }
#   define NSRPC_LOG_INFO3(A,B,C) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, \
        ACE_TEXT("INFO|%D|%t|") A ACE_TEXT("\n"),B,C); \
    }
#   define NSRPC_LOG_INFO4(A,B,C,D) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, \
        ACE_TEXT("INFO|%D|%t|") A ACE_TEXT("\n"),B,C,D); \
    }
#   define NSRPC_LOG_INFO5(A,B,C,D,E) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, \
        ACE_TEXT("INFO|%D|%t|") A ACE_TEXT("\n"),B,C,D,E); \
    }
#   define NSRPC_LOG_INFO6(A,B,C,D,E,F) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, \
        ACE_TEXT("INFO|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F); \
    }
#   define NSRPC_LOG_INFO7(A,B,C,D,E,F,G) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, \
        ACE_TEXT("INFO|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F,G); \
    }
    // = Warning
#   define NSRPC_LOG_WARNING(A) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, \
        ACE_TEXT("WARNING|%D|%t|") A ACE_TEXT("\n")); \
    }

#   define NSRPC_LOG_WARNING2(A,B) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, \
        ACE_TEXT("WARNING|%D|%t|") A ACE_TEXT("\n"),B); \
    }

#   define NSRPC_LOG_WARNING3(A,B,C) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, \
        ACE_TEXT("WARNING|%D|%t|") A ACE_TEXT("\n"),B,C); \
    }

#   define NSRPC_LOG_WARNING4(A,B,C,D) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, \
        ACE_TEXT("WARNING|%D|%t|") A ACE_TEXT("\n"),B,C,D); \
    }

#   define NSRPC_LOG_WARNING5(A,B,C,D,E) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, \
        ACE_TEXT("WARNING|%D|%t|") A ACE_TEXT("\n"),B,C,D,E); \
    }
    // = Error
#   define NSRPC_LOG_ERROR(A) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, \
        ACE_TEXT("ERROR|%D|%t|") A ACE_TEXT("\n")); \
    }
#   define NSRPC_LOG_ERROR2(A,B) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, \
        ACE_TEXT("ERROR|%D|%t|") A ACE_TEXT("\n"),B); \
    }
#   define NSRPC_LOG_ERROR3(A,B,C) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, \
        ACE_TEXT("ERROR|%D|%t|") A ACE_TEXT("\n"),B,C); \
    }
#   define NSRPC_LOG_ERROR4(A,B,C,D) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, \
        ACE_TEXT("ERROR|%D|%t|") A ACE_TEXT("\n"),B,C,D); \
    }
#   define NSRPC_LOG_ERROR5(A,B,C,D,E) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, \
        ACE_TEXT("ERROR|%D|%t|") A ACE_TEXT("\n"),B,C,D,E); \
    }
#   define NSRPC_LOG_ERROR6(A,B,C,D,E,F) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, \
        ACE_TEXT("ERROR|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F); \
    }
#   define NSRPC_LOG_ERROR7(A,B,C,D,E,F,G) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, \
        ACE_TEXT("ERROR|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F,G); \
    }
#   define NSRPC_LOG_ERROR8(A,B,C,D,E,F,G,H) \
    { \
    const int __ace_error = ACE_Log_Msg::last_error_adapter(); \
    ACE_Log_Msg* ace___ = ACE_Log_Msg::instance(); \
    ace___->conditional_set(__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, \
        ACE_TEXT("ERROR|%D|%t|") A ACE_TEXT("\n"),B,C,D,E,F,G,H); \
    }
#endif

/** @} */ // addtogroup utility

#endif // NSRPC_LOGGER_H
