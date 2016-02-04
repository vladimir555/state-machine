/*
 * convertion.h
 *
 *  Created on: 08.12.2011
 *      Author: volodja
 */


#ifndef CONVERTATION_H_
#define CONVERTATION_H_


#include <vector>
#include <string>
#include <time.h>
#include <sys/types.h>
#include "utility/type.h"
#include "utility/exception/exception.h"


namespace utility {


using namespace std;


class Convert {
public:
    /**
     *  коды исключений для методов конвертора
     */
    enum ExceptionCode {
        EXCEPTION_CONVERT_BAD_NUMBER,
        EXCEPTION_CONVERT_BAD_STRING,
        EXCEPTION_CONVERT_BAD_DATE_TIME,
        EXCEPTION_CONVERT_BAD_INDEX,
        EXCEPTION_UNKNOWN_ERROR
    };
    typedef utility::Exception<ExceptionCode> Exception;
    /**
     *  преобразовать число в строку base системы исчисления дополненное нулями до count_symbols символов
     *  @param  number              число которое надо преобразовать
     *  @param  base                система исчисления результата
     *  @param  count_symbols       количество символов в результирующей строке(дополняется нулями слева по необходимости)
     *  @return                     число number в строковом представлении в системе исчисления base
     */
    static string   numberToString(int64_t number, int base = 10, size_t count_symbols = 0) throw(Exception);
    /**
     *  преобразовать число в строковом представлении base системы исчисления в число
     *  @param  number              число в строковом представлении которое надо преобразовать
     *  @param  base                система исчисления числа number
     *  @return                     число number int представлении
     */
    static int64_t  stringToNumber(const string &number, int base = 10)                     throw(Exception);
    /**
     *  преобразовать массив байт в список 16-ричных чисел в строковом представлении
     *  @param  v                   массив байт
     *  @return                     строка чисел в шестнацетиричной системе исчисления
     */
    static string   byteVectorToString(const vector<byte_t> &v);
    /**
     *  преобразовать массив байт в строку символов
     *  @param  v                   массив байт
     *  @return                     строка чисел в шестнацетиричной системе исчисления
     */
    static string   charVectorToString(const vector<byte_t> &v);
    /**
     *  конвертировать строку в формат времени
     *  @param  date_time_string    дата и время в строковом представлении вида "2010-01-01 10:07:57"
     *                                                                           0123456789012345678
     *  @return                     время
     */
    static time_t   stringToDateTime(const string &date_time_string)                        throw(Exception);
    /**
     *  конвертировать строку в формат времени
     *  @param  date_time           дата и время
     *  @return                     дата и время в строковом представлении вида "2010-01-01 10:07:57"
     *                                                                           0123456789012345678
     */
    static string   dateTimeToString(const time_t &date_time);
    /**
     *  конвертировать длинную строку в список строк с переносом по словам, где длина каждой строки не больше max_string_length
     *  @param  str_source          исходная строка
     *  @param  max_string_length   максимальная длина каждой строки
     *  @return                     текст
     */
    static text_t   stringToFixedWideText(string &str_source, const size_t max_string_length);
    /**
     *  конвертировать текст в текст со строками длиной не больше max_string_length с переносам по словам
     *  @param  text_source         исходный текст
     *  @param  max_string_length   максимальная длина каждой строки
     *  @return                     текст
     */
    static text_t   textToFixedWideText(const text_t &text_source, const size_t max_string_length);
};


}


#endif /* CONVERTATION_H_ */
