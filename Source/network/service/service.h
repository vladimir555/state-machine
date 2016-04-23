/*
 * service.h
 *
 *  Created on: 16.12.2011
 *      Author: volodja
 */


#ifndef SERVICE_H_
#define SERVICE_H_


#include "utility/logger.h"
#include "utility/exception/exception.h"
#include "utility/state_machine/state_machine.h"
#include "network/socket/socket_server.h"


namespace network {


//TODO: сделать очередь и работу в потоке
class Service {
public:
    /**
     *  коды исключений для методов файловой системы
     */
    enum ExceptionCode {
        EXCEPTION_WRITE_ERROR,
        EXCEPTION_READ_ERROR,
        EXCEPTION_UNKNOWN_ERROR
    };
    typedef utility::Exception<ExceptionCode> 
    Exception;

private:
    LOG_DEFINE;

    static const int   
    SERVICE_STATE_MACHINE_LIMIT_STEP_COUNT      = 5;
    
    static const int   
    SERVICE_SOCKET_SERVER_REQUEST_DEQUEUE_SIZE  = 5;

    class State {
    public:

        enum StateCode {
            STATE_UNKNOWN,
            STATE_WAIT_REQUEST,
            STATE_EXIT
        };

        StateCode   
        state_current;

        bool        
        equivalent(State &state);

        State();

       ~State();
    };
    State                                   
    service_state;

    utility::StateMachine<Service, State>   
    service_state_machine;

    class RequestHandler_: public SocketServer::RequestHandler {
        string 
        handleRequest(const string &request);
    };
    RequestHandler_                         
    service_request_handler;

    SocketServer                            
    service_socket_server;

    void                                    
    handleOnUnknownState(State &state);

    void                                    
    handleOnExit(State &state);

    void                                    
    handleOnWaitRequest(State &state);

public:

    void                                    
    start();

    Service();

    virtual ~Service();
};


} /* namespace network */
#endif /* SERVICE_H_ */
