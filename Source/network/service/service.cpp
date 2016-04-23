/*
 * service.cpp
 *
 *  Created on: 16.12.2011
 *      Author: volodja
 */


#include "service.h"


namespace network {


LOG_IMPLEMENT(Service);


//-----Service::State
bool 
Service::State::equivalent(Service::State &state) {
    if (state_current == state.state_current)
        return true;
    else
        return false;
}


Service::State::State()
:
    state_current(STATE_UNKNOWN)
{
}


Service::State::~State() {
}
//-----


//-----Service
void 
Service::start() {
    service_state_machine.processing(service_state);
}


Service::Service()
:
    service_state_machine   (*this, &Service::handleOnExit  , SERVICE_STATE_MACHINE_LIMIT_STEP_COUNT),
    service_socket_server   (utility::word_t(3001)          , SERVICE_SOCKET_SERVER_REQUEST_DEQUEUE_SIZE)
{
    State state;

    state.state_current = State::STATE_UNKNOWN;
    service_state_machine.addStateHandler(state, &Service::handleOnUnknownState);

    state.state_current = State::STATE_EXIT;
    service_state_machine.addStateHandler(state, &Service::handleOnExit);

    state.state_current = State::STATE_WAIT_REQUEST;
    service_state_machine.addStateHandler(state, &Service::handleOnWaitRequest);
}


Service::~Service() {
}


string 
Service::RequestHandler_::handleRequest(const string &request) {
    return request + " - processed";
}


void 
Service::handleOnUnknownState(State &state) {
    LOG_DEBUG("Unknown state");
    LOG_INFO ("Initialize service");
    service_socket_server.open();
    state.state_current = State::STATE_WAIT_REQUEST;
}


void 
Service::handleOnExit(State &state) {
    LOG_DEBUG("Exit");
    LOG_INFO ("Shut down service");

    service_socket_server.close();

    state.state_current = State::STATE_UNKNOWN;
}


void 
Service::handleOnWaitRequest(State &state) {
    LOG_INFO("Wait request");

    service_socket_server.waitExchange(service_request_handler);

    LOG_INFO("Request processed");
}
//-----


} /* namespace network */
