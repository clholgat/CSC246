#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sock.h"

/*--------------------------------------------------------------------------*/
/*  SOCK.C								    */
/*    C routines to support client and server TCP sockets		    */
/*									    */
/*- Modification History: --------------------------------------------------*/
/*  When:	Who:			Comments:			    */
/*									    */
/*  13-Dec-10	Christopher G. Healey	Initial implementation		    */
/*--------------------------------------------------------------------------*/

/* Module global defines */

#define  PORT  3550			/* Port to communicate over */


int client_connect( char *host )

/*  Routine for a client to connect to a server socket, returns client
 *  socket descriptor, or -1 on error
 *
 *  host:  Hostname to connect to
 */
{
  int                 client_sock;	/* Client socket */
  struct hostent     *he;		/* Server information */
  int                 rc;		/* Return code */
  struct sockaddr_in  server;		/* Server address information */


  /* Query information about the server */

  he = gethostbyname( host );
  if ( he == NULL ) {
    printf( "Error requesting server information\n" );
    return -11;
  }

  /* Try to create a socket */

  client_sock = socket( AF_INET, SOCK_STREAM, 0 );
  if ( client_sock == -1 ) {
    printf( "Error creating client socket\n" );
    return -1;
  }

  server.sin_family = AF_INET;		/* ARPA internet protocol */
  server.sin_port = htons( PORT );	/* Convert port to network format */
					/* Copy server IP address */
  server.sin_addr = *((struct in_addr *) he->h_addr);
  bzero( &server.sin_zero, 8 );		/* Zero sin_zero field */

  /* Attempt to connect to the server through the socket descriptor */

  rc = connect(
    client_sock, (struct sockaddr *) &server, sizeof( struct sockaddr ) );
  if ( rc == -1 ) {
    printf( "Error connecting to server\n" );
    return -1;
  }

  return client_sock;			/* Return client socket descriptor */
}					/* End routine client_connect */


int client_waiting( int sock, char *IP, int IP_n )

/*  This routine returns a client socket descriptor if a client is
 *  waiting to connect, -1 otherwise
 *
 *  sock:  Socket descriptor
 *  IP:    Client IP address (NULL to ignore IP address)
 *  IP_n:  Length of IP address buffer
 */
{
  struct sockaddr_in  client;		/* Client address information */
  int                 client_sock;	/* Client socket */
  int                 flag;		/* Socket control flags */
  int                 rc;		/* Return code */
  int                 sin_size;		/* Size of socket address struct */


  /* Temporarily put socket into non-blocking mode */

  flag = fcntl( sock, F_GETFL );	/* Set socket non-blocking */
  flag |= O_NONBLOCK;
  fcntl( sock, F_SETFL, flag );

  rc = listen( sock, 1 );		/* Listen for a client */
  if ( rc == -1 ) {
    printf( "Error listening over server socket\n" );
    return -1;
  }

  /* Accept connection request from client, will return socket
   * descriptor if client exists, or -1 if no client is waiting */

  sin_size = sizeof( struct sockaddr_in );
  client_sock = accept( sock, (struct sockaddr *) &client, &sin_size  );

  if ( client_sock != -1 ) {		/* Client exists? */

  /* Apparently, if you accept a connection over a non-blocking
   * socket, the accepted socket is also non-blocking, so we need to
   * set the client socket back to blocking */

    flag = fcntl( client_sock, F_GETFL );
    flag &= ~O_NONBLOCK;
    fcntl( client_sock, F_SETFL, flag );

    if ( IP != NULL && IP_n > 0 ) {	/* Grab client's IP address */
      strncpy( IP, inet_ntoa( client.sin_addr ), IP_n - 1 );
      IP[ IP_n - 1 ] = '\0';
    }
  }

  flag ^= O_NONBLOCK;			/* Set sock back to blocking */
  fcntl( sock, F_SETFL, flag );

  return client_sock;			/* Return client socket descriptor */
}					/* End routine client_waiting */


int probe_socket( int sock )

/*  This routine probes a socket to see if it has data available to read
 *
 *  sock:   Socket to receive message
 */
{
  int             n;			/* Number of ready sockets */
  fd_set          sock_list;		/* Socket descriptor(s) to probe */
  struct timeval  tm_out;		/* Timeout duration */


  FD_ZERO( &sock_list );		/* Initialize socket descriptor list */
  FD_SET( sock, &sock_list );

  tm_out.tv_sec = 1;			/* Wait 1 second until timeout */
  tm_out.tv_usec = 0;

  n = select( 1, &sock_list, NULL, NULL, &tm_out );

  if ( n <= 0 ) {			/* Return 0 if probe times out */
    return 0;
  } else {				/* Return 1 if probe succeeds */
    return 1;
  }
}					/* End routine probe_socket */


int recv_msg( char *buf, int buf_n, int sock )

/*  This routine waits for a message sent along an existing socket,
 *  returns the length of the message
 *
 *  buf:    Message buffer
 *  buf_n:  Buffer size
 *  sock:   Socket to receive message
 */
{
  int  byte_n;				/* Message size */
  int  len;				/* Message length */


  if ( buf_n < 1 ) {
    printf( "Message buffer size is 0\n" );
    return 0;
  }

  /* Wait for message over socket */

  byte_n = recv( sock, (char *) &len, sizeof( int ), 0 );
  byte_n = recv( sock, buf, len, 0 );

  if ( byte_n == -1 ) {			/* Error on receive? */
    byte_n = 0;
    printf( "Error receiving message\n" );
  }

  buf[ byte_n ] = '\0';
  return byte_n;
}					/* End routine recv_msg */


void send_msg( char *buf, int sock )

/*  This routine sends a message sent along an existing socket
 *
 *  buf:    Message buffer
 *  sock:   Socket to send message
 */
{
  int  len;				/* Message length */


  len = strlen( buf );

  /* Send message length, then message */

  send( sock, (char *) &len, sizeof( int ), 0 );
  send( sock, buf, strlen( buf ), 0 );
}					/* End routine send_msg */


int server_connect()

/*  Routine for a server to create a socket to connect to client(s),
 *  returns server socket descriptor, or -1 on error
 */
{
  int                 rc;		/* Return code */
  struct sockaddr_in  server;		/* Server address information */
  int                 server_sock;	/* Server socket */


  /* First, try to create a TCP stream socket */

  server_sock = socket( AF_INET, SOCK_STREAM, 0 );
  if ( server_sock == -1 ) {
    printf( "Error creating server socket\n" );
    return -1;
  }

  server.sin_family = AF_INET;		/* ARPA internet protocol */
  server.sin_port = htons( PORT );	/* Convert port to network format */
  server.sin_addr.s_addr = INADDR_ANY;	/* Use current machine's IP addr */
  bzero( &server.sin_zero, 8 );		/* Zero sin_zero field */

  /* Attempt to bind to the port through the socket descriptor */

  rc =
    bind( server_sock, (struct sockaddr *) &server, sizeof( struct sockaddr ) );
  if ( rc == -1 ) {
    printf( "Error binding to server socket\n" );
    return -1;
  }

  return server_sock;			/* Return server socket descriptor */
}					/* End routine server_connect */


void terminate_socket( int sock )

/*  This routine closes an active socket
 *
 *  sock:  Socket to close
 */
{
  close( sock );
}					/* End routine terminate_socket */


int wait_for_client( int sock, char *IP, int IP_n )

/*  This routine listens along an active socket for a client
 *  connection, returns client socket descriptor, or -1 on error
 *
 *  sock:  Socket descriptor
 *  IP:    Client IP address (NULL to ignore IP address)
 *  IP_n:  Length of IP address buffer
 */
{
  struct sockaddr_in  client;		/* Client address information */
  int                 client_sock;	/* Client socket */
  int                 rc;		/* Return code */
  int                 sin_size;		/* Size of socket address struct */


  rc = listen( sock, 1 );		/* Listen for a client */
  if ( rc == -1 ) {
    printf( "Error listening over server socket\n" );
    return -1;
  }

  /* Accept connection request from client */

  sin_size = sizeof( struct sockaddr_in );
  client_sock = accept( sock, (struct sockaddr *) &client, &sin_size  );

  if ( client_sock == -1 ) {
    printf( "Error accepting client connection request\n" );
    return -1;
  }

  if ( IP != NULL && IP_n > 0 ) {	/* Grab client's IP address */
    strncpy( IP, inet_ntoa( client.sin_addr ), IP_n - 1 );
    IP[ IP_n - 1 ] = '\0';
  }

  return client_sock;			/* Return client socket descriptor */
}					/* End routine wait_for_client */
