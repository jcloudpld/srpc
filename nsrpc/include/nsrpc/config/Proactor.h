#ifndef NSRPC_TPROACTOR_H
#define NSRPC_TPROACTOR_H

#if defined (NSRPC_USE_TPROACTOR)

#  define NSRPC_Proactor TRB_Proactor
#  define NSRPC_Asynch_Read_Stream TRB_Asynch_Read_Stream
#  define NSRPC_Asynch_Write_Stream TRB_Asynch_Write_Stream
#  define NSRPC_Asynch_Acceptor TRB_Asynch_Acceptor
#  define NSRPC_Service_Handler TRB_Service_Handler
#  define NSRPC_Asynch_Accept TRB_Asynch_Accept
#  define NSRPC_Asynch_Connector TRB_Asynch_Connector
#  define NSRPC_Asynch_Connect TRB_Asynch_Connect
#  define NSRPC_Handler TRB_Handler
#  define NSRPC_Asynch_Read_Dgram TRB_Asynch_Read_Dgram
#  define NSRPC_Asynch_Write_Dgram TRB_Asynch_Write_Dgram

#  define NSRPC_Proactor_Impl TRB_Proactor_Impl
#  define NSRPC_WIN32_Proactor TRB_WIN32_Proactor
#  define NSRPC_POSIX_AIOCB_Proactor TRB_POSIX_AIOCB_Proactor
#  define NSRPC_POSIX_CB_Proactor TRB_POSIX_CB_Proactor
#  define NSRPC_POSIX_SIG_Proactor TRB_POSIX_SIG_Proactor
#  define NSRPC_SUN_Proactor TRB_SUN_Proactor

#else

#  define NSRPC_Proactor ACE_Proactor
#  define NSRPC_Asynch_Read_Stream ACE_Asynch_Read_Stream
#  define NSRPC_Asynch_Write_Stream ACE_Asynch_Write_Stream
#  define NSRPC_Asynch_Acceptor ACE_Asynch_Acceptor
#  define NSRPC_Service_Handler ACE_Service_Handler
#  define NSRPC_Asynch_Accept ACE_Asynch_Accept
#  define NSRPC_Asynch_Connector ACE_Asynch_Connector
#  define NSRPC_Asynch_Connect ACE_Asynch_Connect
#  define NSRPC_Handler ACE_Handler
#  define NSRPC_Asynch_Read_Dgram ACE_Asynch_Read_Dgram
#  define NSRPC_Asynch_Write_Dgram ACE_Asynch_Write_Dgram

#  define NSRPC_Proactor_Impl ACE_Proactor_Impl
#  define NSRPC_WIN32_Proactor ACE_WIN32_Proactor
#  define NSRPC_POSIX_AIOCB_Proactor ACE_POSIX_AIOCB_Proactor
#  define NSRPC_POSIX_CB_Proactor ACE_POSIX_CB_Proactor
#  define NSRPC_POSIX_SIG_Proactor ACE_POSIX_SIG_Proactor
#  define NSRPC_SUN_Proactor ACE_SUN_Proactor

#endif

#endif // NSRPC_TPROACTOR_H
