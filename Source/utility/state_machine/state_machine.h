/*
 * state_machine.h
 *
 *  Created on: 29.11.2011
 *      Author: volodja
 */


//#pragma interface //"utility/state_machine/state_machine.h"


#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_


#include <vector>
#include "utility/logger.h"
#include "utility/exception/exception.h"
#include "utility/convertion.h"


#include <iostream>


using namespace std;


namespace utility {
//namespace state_machine {


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
    typedef vector<pair_state_function_t>       vector_state_function_t;
    /**
     *  указатель на класс с методами - обработчиками контекста
     */
    StateHandler                               *handler;
    /**
     *  список контекст(состояние) - обработчик
     */
    vector_state_function_t                     vector_state_function;
    /**
     *
     */
    function_t                                  handler_exit;
    /**
     *  конструктор по умолчанию для конечного автомата запрещен
     */
                                                StateMachine() {};
public:
    /**
     *  конструктор конечного автомата
     *  @param  handler     объект класса с методами обработчиками состояний
     */
                                                StateMachine(StateHandler &handler, function_t handler_exit, const int limit_step_count)
    :
        limit_step_count    (limit_step_count),
        handler             (&handler),
        handler_exit        (handler_exit)
    {}
    /**
     *  добавить обработчик для контекста
     *  @param  state       уникальный контекст
     *  @param  function    обработчик
     */
    void                                        addStateHandler(const State &state, const function_t function) throw(Exception) {
        pair_state_function_t pair(state, function);
        /*
         *  проверка на уникальность добавляемых данных по первому значению (состоянию машины) в паре
         */
        for (size_t i = 0; i < vector_state_function.size(); i++) {
            if (pair.first.equivalent(vector_state_function[i].first))
                throw Exception(CODE_ERROR_NOT_UNIQUE_STATE, "Error add not unique state to the state machine");
        }
        /*
         *  в список обработчиков контекста добавляем обработчик с уникальными атрибутами контекста
         */
        vector_state_function.push_back(pair);
    }


    /**
     *  запустить конечный автомат
     *  @param  state_container   контейнер с текущим контекстом
     */
    void                                        processing(State &state) throw(Exception) {
        int         step_count  = 0;
        function_t  function;

//        do {
//            /*
//             *  поиск и выполнение обработчика для текущего состояния
//             */
//            function = NULL;
//            for (size_t i = 0; i < vector_state_function.size(); i++) {
//                if (state.equivalent(vector_state_function[i].first)) {
//                    function = vector_state_function[i].second;
//                    break;
//                }
//            }
//            /*
//             *  если не было найдено обработчика для текущего контекста то делаем исключение
//             */
//            if  (function == NULL)
//                throw Exception(CODE_ERROR_HANDLER_NOT_FOUND, "StateHandler not found for current state");
//            else {
//                if (step_count < limit_step_count) {
//                    step_count++;
//                    (handler->*function)(state);
//                } else
//                    throw Exception(CODE_ERROR_OVERFLOW_LIMIT_STEP_COUNT, "Overflow step limit count " + Convert::numberToString(step_count));
//            }
//
//        } while (function != handler_exit);


        do {
            /*
             *  поиск и выполнение обработчика для текущего состояния
             */
            function = NULL;
            for (size_t i = 0; i < vector_state_function.size(); i++) {
                if (state.equivalent(vector_state_function[i].first)) {
                    if (step_count < limit_step_count) {
                        step_count++;
                        function = vector_state_function[i].second;
                        (handler->*vector_state_function[i].second)(state);
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
};


//} /* namespace state_machine */
} /* namespace utility */


#endif /* STATE_MACHINE_H_ */
