/*
 * 441 Assignment 1
 *
 * Daniel Artuso - 30029826
 * daniel.artuso1@ucalgary.ca
 * CPSC 441 - Assignment 1
 *
 *
 * Based on code from:
 * Tutorial 3 - HTTP Proxy
 * Example: Basic Proxy (client <-> proxy <-> server)
 * Created: Jan. 27, 2020
 * Author: Rachel Mclean
 *
 * Also used for reference for multithreading:
 * http://www.linuxhowtos.org/C_C++/socket.htm
 *
 *
 * Purpose:
 * This is a web proxy that receives requests from the client and pass them to the desired web server
 * The server will reply with the content which the proxy modifies
 *
 * Details:
 * Command line - EXECUTABLE portnumber (optional, default is 8080)
 *
 * From client connect on serverIp:port
 *
 *
 */

/* standard libraries*/
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

/* libraries for socket programming */
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>


/*h_addr address*/
#include <netdb.h>

/*clean exit*/
#include <csignal>
#include <string>
#include <regex>

/* port numbers */
#define HTTP_PORT 80
#define DEFAULT_PORT 8080

/* string sizes */
#define MESSAGE_SIZE 2048

static const std::string trollUrl = "\"http://pages.cpsc.ucalgary.ca/~carey/CPSC441/trollface.jpg\"";
//const std::string otherTroll = R"("./trollface.jpg")";
//const std::string japanStr = "Japan";
static const std::string germanyStr = "Germany";
static const std::string trollyStr = "Trolly";
static const std::string lengthHeader = "Content-Length: ";

//socket descriptors (global for use in cleanExit)
int listen_socket, data_socket, web_socket;

using namespace std;

//Functions
/*
 * cleanExit
 * Function for exiting with ports closed
 * Pass signal for program to return with
 */
void cleanExit(int signal);

/*
 * modifyWeb
 *
 * Given a packet of data modifies it according to assignment specs:
 * Trolly to Floppy
 * Italy to Germany
 * Images with Floppy in the URL are replaced with a troll face image
 *
 * Returns modified packet in string format
 */
string modifyWeb(char *input);

/*
 * find my IP
 * Utility to list ip to make it easier for client to find it to connect.
 * Source courtesy of: https://stackoverflow.com/questions/212528/get-the-ip-address-of-the-machine
 */
void findMyIP();

void receiveRequest();

void receiveData();

int main(int argc, char *argv[]) {
    int proxyPort = DEFAULT_PORT, processId;

    findMyIP();

    if (argc == 2) {
        try {
            proxyPort = stoi(argv[1]);
            printf("Running on port %d\n",proxyPort);
        } catch (...){
            printf("Error: usage EXE portNumber\nUsing default port 8080\n");
            proxyPort = DEFAULT_PORT;
        }
    } else {
        printf("Defaulted to port 8080, port can be passed as argument.\n");
    }

    /* to handle ungraceful exits */
    signal(SIGTERM, cleanExit);
    signal(SIGINT, cleanExit);
    signal(SIGCHLD,SIG_IGN);

    //initialize proxy address
    printf("Initializing proxy address...\n");
    struct sockaddr_in proxy_addr{};
    proxy_addr.sin_family = AF_INET;
    proxy_addr.sin_port = htons(proxyPort);
    proxy_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //create listening socket
    printf("Creating listen_socket...\n");
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket < 0) {
        perror("socket() call failed");
        exit(-1);
    }

    //bind listening socket
    printf("Binding listen_socket...\n");
    if (bind(listen_socket, (struct sockaddr *) &proxy_addr, sizeof(struct sockaddr_in)) < 0) {
        perror("bind() call failed");
        exit(-1);
    }

    //listen for client connection requests
    printf("Listening on listen_socket...\n");
    if (listen(listen_socket, 20) < 0) {
        perror("listen() call failed...\n");
        exit(-1);
    }

    //infinite while loop for listening
    while (true) {
        printf("Accepting connections...\n");

        //accept client connection request
        data_socket = accept(listen_socket, nullptr, nullptr);
        if (data_socket < 0) {
            perror("accept() call failed\n");
            exit(-1);
        }

        //Forking
        processId = fork();
        if (processId < 0) {
            printf("Error during fork");
        }

        if (processId == 0) { //if new process then process otherwise
            close(listen_socket); //only main process is still listening (non blocking)
            // while loop to receive client requests
            receiveRequest();
            exit(0);
        } else {
            close(data_socket);
        }
    }
}

void receiveRequest() {
    char client_request[MESSAGE_SIZE], server_request[MESSAGE_SIZE], url[MESSAGE_SIZE];
    char host[MESSAGE_SIZE], path[MESSAGE_SIZE];
    int i;
    while (recv(data_socket, client_request, MESSAGE_SIZE, 0) > 0) {
        printf("%s\n", client_request);

        //copy to server_request to preserve contents (client_request will be damaged in strtok())
        strcpy(server_request, client_request);

        //tokenize to get a line at a time
        char *line = strtok(client_request, "\r\n");

        //extract url
        sscanf(line, "GET http://%s", url);

        //separate host from path
        for (i = 0; i < strlen(url); i++) {
            if (url[i] == '/') {
                strncpy(host, url, i);
                host[i] = '\0';
                break;
            }
        }
        bzero(path, MESSAGE_SIZE);
        strcat(path, &url[i]);

        //initialize server address
        printf("Initializing server address...\n");
        struct sockaddr_in server_addr{};
        struct hostent *server;
        server = gethostbyname(host);

        bzero((char *) &server_addr, sizeof(struct sockaddr_in));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(HTTP_PORT);
        bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);

        //create web_socket to communicate with web_server
        web_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (web_socket < 0) {
            perror("socket() call failed\n");
        }

        //send connection request to web server
        if (connect(web_socket, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) < 0) {
            perror("connect() call failed\n");
        }

        //send http request to web server
        if (send(web_socket, server_request, MESSAGE_SIZE, 0) < 0) {
            perror("send(0 call failed\n");
        }


        //receive http response from server
        receiveData();
    }//while recv() from client
}

void receiveData() {
    char server_response[10 * MESSAGE_SIZE], client_response[10 * MESSAGE_SIZE];
    int serverBytes;
    regex regText("Content-Type: text");

    while ((serverBytes = recv(web_socket, server_response, MESSAGE_SIZE, 0)) > 0) {

        bcopy(server_response, client_response, serverBytes);

        //Detect if modifiable or regular
        if (regex_search(client_response, regText)) {
//            printf("%s\n", server_response);
            printf("Recieved: %d\n", serverBytes);

            string modified_response = modifyWeb(client_response);
            printf("Printing modified...\n");

//            printf("%s\n", modified_response.c_str());

            //send http response to client
            int sent = send(data_socket, modified_response.c_str(), modified_response.length(), 0);
            if (sent < 0) {
                perror("send() call failed...\n");
            }
            modified_response.clear();
            printf("Sent: %d\n", sent);
        } else {
            if (send(data_socket, client_response, serverBytes, 0) < 0) {
                perror("send() call failed...\n");
            }
        }
        bzero(client_response, MESSAGE_SIZE * 10);
        bzero(server_response, MESSAGE_SIZE * 10);
    }//while recv() from server
}

void cleanExit(int signal) {
    close(listen_socket);
    close(data_socket);
    close(web_socket);
    exit(signal);
}

string modifyWeb(char *input) {
    string inputStr(input);
    string outputStr;
    // Regex expressions
    regex regflopp("Floppy");
    regex regfloppUrl("\".*Floppy.*[jpg|png|gif|jpeg]\"");
    regex regItaly("Italy(?!\\.[j|p|g])");
    regex regContentLength("Content-Length: \\d*\r\n");
    //Perform regex replacement on each
    outputStr = regex_replace(inputStr, regfloppUrl, trollUrl); //Image url
    outputStr = regex_replace(outputStr, regflopp, trollyStr); //Floppy to Trolly
    outputStr = regex_replace(outputStr, regItaly, germanyStr); //Italy to Germany
    //Calculate new length
    string content = outputStr.substr(outputStr.find("\r\n\r\n") + 4); //Get packet content from below header
    int contentSize = content.length();
    string newHeader = lengthHeader + to_string(contentSize) + "\r\n";
    printf("%s\n", newHeader.c_str());
    outputStr = regex_replace(outputStr, regContentLength, newHeader);

    return outputStr;
}

void findMyIP() {
    struct ifaddrs * ifAddrStruct=nullptr;
    struct ifaddrs * ifa=nullptr;
    void * tmpAddrPtr=nullptr;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct!=nullptr) freeifaddrs(ifAddrStruct);
}
