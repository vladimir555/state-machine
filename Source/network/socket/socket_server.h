/*
 * socket_server.h
 *
 *  Created on: 15.12.2011
 *      Author: volodja
 */


#ifndef SOCKET_SERVER_H_
#define SOCKET_SERVER_H_


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <string.h>
#include "utility/logger.h"
#include "utility/exception/exception.h"
#include "utility/convertion.h"


using namespace std;


namespace network {


/**
 *
 */
class SocketServer {
private:
    bool                is_open;
    utility::word_t     port;
//    int                 request_connect_queue;
    int                 socket_fd_listen;

    LOG_DEFINE;

public:
    class RequestHandler {
    public:
        virtual string  handleRequest(const string &request) = 0;
                        RequestHandler() {};
        virtual        ~RequestHandler() {};
    };

    enum ExceptionCode {
        EXCEPTION_ERROR_CONNECT,
        EXCEPTION_ERROR_DISCONNECT,
        EXCEPTION_ERROR_EXCHANGE
    };
    typedef utility::Exception<ExceptionCode> Exception;
    /**
     *  открыть сокет
     *  @param  port    порт
     */
    void        open()                                          throw(Exception);
    /**
     *  закрыть сокет
     */
    void        close()                                         throw(Exception);
    /**
     *  ожидать сообщения через сокет
     *  @return         сообщение
     */
    void        waitExchange(RequestHandler &request_handler)   throw(Exception);
    /**
     *  конструктор
     */
                SocketServer(utility::word_t port, int request_connect_queue_size);
    /**
     *  деструктор
     */
    virtual    ~SocketServer();
};


} /* namespace network */
#endif /* SOCKET_SERVER_H_ */
