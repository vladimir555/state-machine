/*
 * socket_client.cpp
 *
 *  Created on: 14.12.2011
 *      Author: volodja
 */


#include "socket_client.h"


namespace network {


LOG_IMPLEMENT(SocketClient);


void 
SocketClient::open(const string &host_, const utility::word_t port_)   
throw(SocketClient::Exception) {
    try {
        if (is_open) {
            LOG_DEBUG("Socket already open");
            return;
        }


        host    = host_;
        port    = port_;

        LOG_DEBUG("Socket open, connect to " << host << ":" << port);

        struct  sockaddr_in     socket_address;
        struct  hostent        *host_raw = gethostbyname(host.c_str());

        if (host_raw == NULL)
            throw Exception(EXCEPTION_ERROR_CONNECT, "Wrong host '" + host + "'");

        socket_fd                   = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1)
            throw Exception(EXCEPTION_ERROR_CONNECT, "Error open socket, port " + utility::Convert::numberToString(port));

        socket_address.sin_family   = AF_INET;
        socket_address.sin_port     = htons(port);
        bcopy((char*)host_raw->h_addr, (char*)&socket_address.sin_addr, host_raw->h_length);

        if (connect(socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address)) < 0)
            throw Exception(EXCEPTION_ERROR_CONNECT, "Connect error");

        is_open                     = true;
    } catch (const exception &e) {
        throw Exception(EXCEPTION_ERROR_CONNECT, e.what());
    } catch (...) {
        throw Exception(EXCEPTION_ERROR_CONNECT, "Unknown exception");
    }
}


void 
SocketClient::close() {
    try {
        if (!is_open) {
            LOG_DEBUG("Socket already closed");
            return;
        }

        LOG_DEBUG("Socket close, " << host << ":" << port);
        if (shutdown(socket_fd, SHUT_RDWR) == -1)
            throw Exception(EXCEPTION_ERROR_DISCONNECT, "Error close socket");

        is_open = false;
    } catch (const exception &e) {
        throw Exception(EXCEPTION_ERROR_DISCONNECT, e.what());
    } catch (...) {
        throw Exception(EXCEPTION_ERROR_DISCONNECT, "Unknown exception");
    }
}


string 
SocketClient::exchange(const string &message)                     	
throw(SocketClient::Exception) {
    try {
        string  response;
        char    buffer[256];

        open(host, port);

        if (!is_open)
            throw Exception(EXCEPTION_ERROR_CONNECT , "Socket is not open");

        LOG_DEBUG("Socket send '" << message << "'");
        send(socket_fd, message.c_str(), message.size(), 0);
        do {
            buffer[0] = 0;
            recv(socket_fd, buffer, sizeof(buffer), 0);
            response += buffer;
        } while (buffer[0] != 0);

        LOG_DEBUG("Socket resp '" << response << "'");

        close();

        return response;
    } catch (const exception &e) {
        throw Exception(EXCEPTION_ERROR_EXCHANGE, e.what());
    } catch (...) {
        throw Exception(EXCEPTION_ERROR_EXCHANGE, "Unknown exception");
    }
}


SocketClient::SocketClient(const string &host, const utility::word_t port)
:
    is_open (false),
    host    (host),
    port    (port)
{
}


SocketClient::~SocketClient() {
    if (is_open)
        close();
}


} /* namespace network */
