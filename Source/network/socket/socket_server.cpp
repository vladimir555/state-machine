/*
 * socket_server.cpp
 *
 *  Created on: 15.12.2011
 *      Author: volodja
 */


#include "socket_server.h"


namespace network {


LOG_IMPLEMENT(SocketServer);


//сделать логирование адреса входящего запроса
void 
SocketServer::open() 
throw(SocketServer::Exception) {
    try {
        if (is_open) {
            LOG_DEBUG("Socket already open");
            return;
        }

        LOG_DEBUG("Socket open port " << port);
        /* First call to socket() function */
        socket_fd_listen = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd_listen < 0)
            throw Exception(EXCEPTION_ERROR_CONNECT, "Socket listen error");

        struct sockaddr_in server_host;

        /* Initialize socket structure */
        bzero((char *) &server_host, sizeof(server_host));
        server_host.sin_family      = AF_INET;
        server_host.sin_addr.s_addr = INADDR_ANY;
        server_host.sin_port        = htons(port);

        LOG_DEBUG("Socket bind port " << port);
        /* Now bind the host address using bind() call.*/
        //TODO: process bind result
//        auto bind_result =
          bind(socket_fd_listen, (struct sockaddr*) &server_host, sizeof(server_host));
//        if (bind_result < 0)
//            throw Exception(EXCEPTION_ERROR_CONNECT, "Socket binding error");

        is_open = true;
    } catch (const exception &e) {
        throw Exception(EXCEPTION_ERROR_CONNECT, e.what());
    } catch (...) {
        throw Exception(EXCEPTION_ERROR_CONNECT, "Unknown exception");
    }
}


void 
SocketServer::close() 
throw(SocketServer::Exception) {
    try {
        if (!is_open) {
            LOG_DEBUG("Socket already closed");
            return;
        }

        LOG_DEBUG("Socket close port " << port);
        if (shutdown(socket_fd_listen, SHUT_RDWR) == -1)
            throw Exception(EXCEPTION_ERROR_DISCONNECT, "Error close socket");

        is_open = false;
    } catch (const exception &e) {
        throw Exception(EXCEPTION_ERROR_DISCONNECT, e.what());
    } catch (...) {
        throw Exception(EXCEPTION_ERROR_DISCONNECT, "Unknown exception");
    }
}


void 
SocketServer::waitExchange(RequestHandler &request_handler) 
throw(SocketServer::Exception) {
    string request;
    string answer;

    try {
        if(!is_open)
            throw Exception(EXCEPTION_ERROR_CONNECT, "Socket is not open");

        /*
         * Now start listening for the clients, here process will
         * go in sleep mode and will wait for the incoming connection
         */
        LOG_DEBUG("Socket listening, wait incoming connection");
        //TODO: количество свяей в очереди
        if (listen(socket_fd_listen, 1) < 0)
            throw Exception(EXCEPTION_ERROR_EXCHANGE, "Socket listen error");

        struct sockaddr_in  client_host;
        socklen_t           socket_client_size = sizeof(client_host);
        char                buffer[256];
        int                 count;


        LOG_DEBUG("Socket accept incoming connection");
        /* Accept actual connection from the client */
        int socket_fd_accept = accept(socket_fd_listen, (struct sockaddr*)&client_host, &socket_client_size);
        if (socket_fd_accept < 0)
            throw Exception(EXCEPTION_ERROR_EXCHANGE, "Socket accept error");

        /* If connection is established then start communicating */
        bzero(buffer, 256);
        //count = read(socket_fd_accept, buffer, sizeof(buffer));
        count = recv(socket_fd_accept, buffer, sizeof(buffer), 0);
        if (count < 0)
            throw Exception(EXCEPTION_ERROR_EXCHANGE, "Socket read error");
        else
            request = buffer;
        LOG_DEBUG("Socket request '" << request << "'");

        answer = request_handler.handleRequest(request);

        LOG_DEBUG("Socket answer  '" << answer  << "'");
        /* Write a response to the client */
        //count = write(socket_fd_accept, send.c_str(), send.size());
        count = send(socket_fd_accept, answer.c_str(), answer.size(), 0);
        if (count < 0)
            throw Exception(EXCEPTION_ERROR_EXCHANGE, "Socket write error");

    } catch (const exception &e) {
        throw Exception(EXCEPTION_ERROR_CONNECT, e.what());
    } catch (...) {
        throw Exception(EXCEPTION_ERROR_CONNECT, "Unknown exception");
    }
}


SocketServer::SocketServer(utility::word_t port, int request_connect_queue_size)
:
    is_open                 (false),
    port                    (port)
//    request_connect_queue   (request_connect_queue_size)
{}


SocketServer::~SocketServer() {
    if (is_open)
        close();
}


} /* namespace network */
