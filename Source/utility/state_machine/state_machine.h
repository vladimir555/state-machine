/*
 * state_machine.h
 *
 *  Created on: 29.11.2011
 *      Author: volodja
 */





#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_


#include <list>
#include "utility/logger.h"
#include "utility/exception/exception.h"
#include "utility/convertion.h"


#include <iostream>


using namespace std;


namespace utility {


/**
 *  конечный автомат
 *  @param  State       класс состояния, содержит состояние автомата
 *  @param  StateHandler     класс обработчика состояния, изменяет состояние
 */
template<typename StateHandler, typename State>
class StateMachine {
public:
    /**
     *  коды исключений конечного автомата
     */
    enum ExceptionCode {
        CODE_ERROR_UNKNOWN,
        CODE_ERROR_NOT_UNIQUE_STATE,
        CODE_ERROR_HANDLER_NOT_FOUND,
        CODE_ERROR_OVERFLOW_LIMIT_STEP_COUNT
    };
    /**
     *  класс исключений конечного автомата
     */
    typedef utility::Exception<ExceptionCode>   Exception;

private:
    /**
     *  максимальное количество шагов конечного автомата
     */
    const int                                   limit_step_count;
    /**
     *  тип функции обработчика, изменяет изменяет контекст
     */
    typedef void                                (StateHandler::*function_t)(State &state);
    /**
     *  пара: состояние - обработчик
     */
    typedef pair<State, function_t>             pair_state_function_t;
    /**
     *  тип список контекст(состояние) - обработчик
     */
    typedef list<pair_state_function_t>       state_function_list_t;
    /**
     *  указатель на класс с методами - обработчиками контекста
     */
    StateHandler                               *handler;
    /**
     *  список контекст(состояние) - обработчик
     */
    state_function_list_t                       state_function_list;
    /**
     *
     */
    function_t                                  handler_exit;
    /**
     *  конструктор по умолчанию для конечного автомата запрещен
     */
                                                StateMachine();
public:
    /**
     *  конструктор конечного автомата
     *  @param  handler     объект класса с методами обработчиками состояний
     */
                                                StateMachine(StateHandler &handler, function_t handler_exit, const int limit_step_count);
    /**
     *  добавить обработчик для контекста
     *  @param  state       уникальный контекст
     *  @param  function    обработчик
     */
    void                                        addStateHandler(const State &state, const function_t function) throw(Exception);
    /**
     *  запустить конечный автомат
     *  @param  state       контейнер с текущим состоянием
     */
    void                                        processing(State &state) throw(Exception);
};


template<typename StateHandler, typename State>
StateMachine<StateHandler, State>::StateMachine(StateHandler &handler, function_t handler_exit, const int limit_step_count)
:
    limit_step_count    (limit_step_count),
    handler             (&handler),
    handler_exit        (handler_exit)
{}


template<typename StateHandler, typename State>
void StateMachine<StateHandler, State>::addStateHandler(const State &state, const function_t function) throw(StateMachine<StateHandler, State>::Exception) {
    pair_state_function_t pair(state, function);
    /*
     *  проверка на уникальность добавляемых данных по первому значению (состоянию машины) в паре
     */
    for (auto i: state_function_list) {
        if (pair.first.equivalent(i.first))
            throw Exception(CODE_ERROR_NOT_UNIQUE_STATE, "Error add not unique state to the state machine");
    }
    /*
     *  в список обработчиков контекста добавляем обработчик с уникальными атрибутами контекста
     */
    state_function_list.push_back(pair);
}


template<typename StateHandler, typename State>
void StateMachine<StateHandler, State>::processing(State &state) throw(StateMachine<StateHandler, State>::Exception) {
    int         step_count  = 0;
    function_t  function;

    do {
        /*
         *  поиск и выполнение обработчика для текущего состояния
         */
        function = NULL;
//        for (size_t i = 0; i < state_function_list.size(); i++) {
        for (auto i: state_function_list) {
            if (state.equivalent(i.first)) {
                if (step_count < limit_step_count) {
                    step_count++;
                    function = i.second;
                    (handler->*i.second)(state);
                    break;
                } else
                    throw Exception(CODE_ERROR_OVERFLOW_LIMIT_STEP_COUNT, "Overflow step limit count " + Convert::numberToString(step_count));
            }
        }
        /*
         *  если не было найдено обработчика для текущего контекста то делаем исключение
         */
        if  (function == NULL)
            throw Exception(CODE_ERROR_HANDLER_NOT_FOUND, "Handler not found for current state");

    } while (function != handler_exit);
}


} /* namespace utility */


#endif /* STATE_MACHINE_H_ */
