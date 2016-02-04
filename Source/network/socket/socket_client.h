/*
 * socket_client.h
 *
 *  Created on: 14.12.2011
 *      Author: volodja
 */


#ifndef SOCKET_CLIENT_H_
#define SOCKET_CLIENT_H_


#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include "utility/type.h"
#include "utility/convertion.h"
#include "utility/exception/exception.h"
#include "utility/logger.h"


using namespace std;


namespace network {


/**
 *  сетевой сокет клиен, отправляет и принимает строковые данные
 */
class SocketClient {
public:
    enum ExceptionCode {
        EXCEPTION_ERROR_CONNECT,
        EXCEPTION_ERROR_DISCONNECT,
        EXCEPTION_ERROR_EXCHANGE
    };
    typedef utility::Exception<ExceptionCode> Exception;

private:
    LOG_DEFINE;

    bool                is_open;
    string              host;
    utility::word_t     port;
    int                 socket_fd;

    /**
     *  открыть соединение с сервером
     *  @param  host    адрес хоста
     *  @param  port    порт
     */
    void                open(const string &host, const utility::word_t port)    throw(Exception);
    /**
     *  закрыть соединение с сервером
     */
    void                close();

public:
    /**
     *  обмен с сервером, посылает строку message и получает строковый ответ
     */
    string              exchange(const string &message)                         throw(Exception);
    /**
     *  конструктор
     */
                        SocketClient(const string &host, const utility::word_t port);
    /**
     *  деструктор
     */
    virtual            ~SocketClient();
};


} /* namespace network */
#endif /* SOCKET_CLIENT_H_ */
