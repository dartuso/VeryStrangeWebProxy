//
// Daniel Artuso - 30029826
//
//

#include <iostream>
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>     // for memset()
#include <unistd.h>     // for close()
#include <regex>
#include <netdb.h>

using namespace std;

const int BufferSize = 100000;
const int MaxPending = 1;

int main(int argc, char* argv[]){
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <Listening Port>" << endl;
        exit(1);
    }p

    //TODO: setup
    int serverSock;                  // server socket descriptor
    int clientSock;                  // client socket descriptor
    struct sockaddr_in serverAddr;   // address of the server
    struct sockaddr_in clientAddr;   // address of the client
    char inBuffer[BufferSize];       // Buffer for the message from the server
    int bytesRecv, bytes;            // Number of bytes received
    int bytesSent;                   // Number of bytes sent

    // Create a TCP socket
    // * AF_INET: using address family "Internet Protocol address"
    // * SOCK_STREAM: Provides sequenced, reliable, bidirectional, connection-mode byte streams.
    // * IPPROTO_TCP: TCP protocol
    if ((serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        cout << "socket() failed" << endl;
        exit(1);
    }

    // Free up the port before binding
    // * sock: the socket just created
    // * SOL_SOCKET: set the protocol level at the socket level
    // * SO_REUSEADDR: allow reuse of local addresses
    // * &yes: set SO_REUSEADDR on a socket to true (1)
    // * sizeof(int): size of the value pointed by "yes"
    int yes = 1;
    if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
    {
        cout << "setsockopt() failed" << endl;
        exit(1);
    }

    // Initialize the server information
    // Note that we can't choose a port less than 1023 if we are not privileged users (root)
    memset(&serverAddr, 0, sizeof(serverAddr));         // Zero out the structure
    serverAddr.sin_family = AF_INET;                    // Use Internet address family
    serverAddr.sin_port = htons(atoi(argv[1]));         // Server port number
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface

    // Bind to the local address
    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout << "bind() failed" << endl;
        exit(1);
    }

    // Listen for connection requests
    if (listen(serverSock, MaxPending) < 0)
    {
        cout << "listen() failed" << endl;
        exit(1);
    }

    // set the size of the client address structure
    unsigned int size = sizeof(clientAddr);

    // Waiting for connection requests
    if ((clientSock = accept(serverSock, (struct sockaddr *) &clientAddr, &size)) < 0)
    {
        cout << "accept() failed" << endl;
        exit(1);
    }

    // The server will be blocked until a client is connected to it.
    cout << "Accepted a connection from " << inet_ntoa(clientAddr.sin_addr) << endl;

    // Start communication with the client (terminate when receive a "terminate" command)
    while (strncmp(inBuffer, "terminate", 9) != 0)
    {
        // Clear the buffers
        memset(&inBuffer, 0, BufferSize);

        // Receive the message from client
        bytesRecv = recv(clientSock, (char *) &inBuffer, BufferSize, 0);
        // Check for connection close (0) or errors (< 0)
        if (bytesRecv <= 0)
        {
            cout << "recv() failed, or the connection is closed. " << endl;
            exit(1);
        }

        //TODO: pass requests to web
        // Create a TCP socket
        int  socket_info; // A socket descriptor

        // * AF_INET: using address family "Internet Protocol address"
        // * SOCK_STREAM: Provides sequenced, reliable, bidirectional, connection-mode byte streams.
        // * IPPROTO_TCP: TCP protocol
        socket_info = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        // Initialize the server information
        struct sockaddr_in server;   // Address of the server
        server.sin_addr.s_addr = inet_addr("172.217.14.195"); // Server IP //TODO: GET THIS FROM CLIENT
        server.sin_family = AF_INET; // Use Internet address family
        server.sin_port = htons(80); // Server port number

        // Connect to the server
        // * sock: the socket for this connection
        // * serverAddr: the server address
        // * sizeof(*): the size of the server address
        if (connect(socket_info, (struct sockaddr *)&server, sizeof(server)) < 0){
            printf("Fail in connecting.\n");
            exit(0);
        }
        printf("Connected.\n");

        // Send the message to the server
        char *message = "GET / HTTP/1.1\r\n\r\n";
        if (send(socket_info, message, strlen(message), 0)<0){
            printf("Fail in message sending\n");
            exit(0);
        }
        printf("Message sent.\n");



        bytesSent = send(clientSock, (char *) &inBuffer, bytesRecv, 0);

        if (bytesSent < 0 || bytesSent != bytesRecv)
        {
            cout << "error in sending" << endl;
            exit(1);
        }

    }

    // Close the connection with the client
    close(clientSock);

    // Close the server socket
    close(serverSock);
    //TODO: connect to client

    //TODO: get server request from client

    //TODO: send request from client to web

    //TODO: receive content from server

    //TODO: modify content for assignment

    //TODO: send modified content to client




    return 0;
}

hostent * getHostname(char * hostName){
    struct hostent *hp = gethostbyname(hostName);

    if (hp == NULL) { // if convert was failed
        fprintf(stderr,"gethostbyname() failed\n");
    } else {
        printf("%s = ", hp->h_name);
        unsigned int i=0;
        while ( hp -> h_addr_list[i] != NULL) {
            printf( "%s ", inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[i])));
            i++;
        }
        printf("\n");
    }
    return hp;
}
