/*
 * exception.h
 *
 *  Created on: 24.11.2011
 *      Author: volodja
 */


#ifndef EXCEPTION_H_
#define EXCEPTION_H_


#include <exception>
#include <string>


using namespace std;


namespace utility {


using namespace std;


/**
 *
 */
template<typename ExceptionCode>
class Exception: public exception {
public:
    /**
     *  получить код исключения
     *  @return         код исключения
     */
    ExceptionCode           getCode() const {
        return code;
    }
    /**
     *  конструктор исключения
     *  @param  code    код исключения
     *  @param  message дополнительная информация исключения
     */
                            Exception(ExceptionCode code, const string &message) throw ()
    :

        message (message),
        code    (code)
    {
    }
    /**
     *  деструктор
     */
    virtual                ~Exception() throw () {
    }
    /**
     *  получить текст сообщения исключения
     */
    virtual const char*     what() const throw () {
        return message.c_str();
    }
protected:
    /**
     *  дополнительная информация
     */
    string                  message;
    /**
     *  код исключения
     */
    ExceptionCode           code;
};


/**
 *
 */
template<typename CodeAction, typename CodeException>
class ExceptionAction {
private:
    /**
     *
     */
    CodeAction              code_action;
    /**
     *
     */
    CodeException           code_exception;
public:
    /**
     *
     */
    CodeAction              getCodeAction() {
        return code_action;
    }
    /**
     *
     */
    CodeException           getCodeException() {
        return code_exception;
    }
    /**
     *
     */
                            ExceptionAction(CodeAction code_action, CodeException code_exception)
    :
        code_action     (code_action),
        code_exception  (code_exception)
    {
    }
    /**
     *
     */
                           ~ExceptionAction() {
    }
};

}


#endif /* EXCEPTION_H_ */
