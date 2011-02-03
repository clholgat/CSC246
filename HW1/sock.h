/*  SOCK.H
 *    Prototypes for SOCK.C
 */

#ifndef _sock_h
#define _sock_h

#ifdef __cplusplus
  extern "C" {
#endif

int   client_connect(			/* Create client socket */
        char *IP			/*  IP address to connect to */
      );
int   client_waiting(			/* Check for waiting clients */
        int sock, 			/*  Socket to wait over */
        char *IP,			/*  Buffer for client IP address */
        int IP_n			/*  Buffer size */
      );
int   probe_socket(			/* Probe socket for data to read */
        int sock 			/*  Socket to probe */
      );
int   recv_msg(				/* Wait for message along socket */
        char *buf,			/*  Message buffer */
        int buf_n,			/*  Message buffer size */
        int sock 			/*  Socket to wait over */
      );
void  send_msg(				/* Send message along socket */
        char *buf,			/*  Message buffer */
        int sock 			/*  Socket to send over */
      );
int   server_connect();			/* Create server socket */
void  terminate_socket(			/* Close an active socket */
        int sock			/*  Socket to close */
      );
int   wait_for_client(			/* Wait for client connection */
        int sock, 			/*  Socket to wait over */
        char *IP,			/*  Buffer for client IP address */
        int IP_n			/*  Buffer size */
      );

#ifdef __cplusplus
  }
#endif

#endif
