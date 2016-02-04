/*
 * convertation.cpp
 *
 *  Created on: 08.12.2011
 *      Author: volodja
 */


#include "convertation.h"


namespace utility {


string  Convert::numberToString(int64_t number, int base, size_t count_symbols) throw(Convert::Exception) {
    if (base < 2  ||  base > 16)
        throw Exception(EXCEPTION_CONVERT_BAD_NUMBER, "Wrong number base, must be 2..16");
    if (count_symbols > 256)
        throw Exception(EXCEPTION_CONVERT_BAD_INDEX,  "Wrong number count symbols, must be 0..256");
    if (number < 0)
        throw Exception(EXCEPTION_CONVERT_BAD_NUMBER, "Wrong number, must be 0..");

    string  number_str;
    string  result;
    int     dig_int;

    if (number != 0)
        while (number > 0) {
            dig_int = number % base;
            number  = number / base;

            if (dig_int > 9)
                number_str += 'A' + dig_int - 10;
            else
                number_str += '0' + dig_int;
        }
    else
        number_str = '0';

    for (size_t i = number_str.size(); i < count_symbols; i++)
        result += "0";

    for (size_t i = 0; i < number_str.size(); i++)
        result += number_str[number_str.size() - i - 1];

    return result;
}


int64_t Convert::stringToNumber(const string &number, int base) throw(Convert::Exception) {
    if (base < 2  ||  base > 16)
        throw Exception(EXCEPTION_CONVERT_BAD_INDEX, "Wrong number base, must be 2..16");

    int     result;
    size_t  i;
    bool    is_number = false;

    i       = 0;
    result  = 0;
    while(i < number.size()) {
        if (number[i] >= '0'  &&  number[i] <= '9') {
            result = result * base + number[i] - '0';
            is_number  = true;
        } else
        if (number[i] >= 'A'  &&  number[i] <= ('A' - 10 + base)) {
            result = result * base + number[i] - 'A' + 10;
            is_number  = true;
        } else
            throw Exception(EXCEPTION_CONVERT_BAD_STRING, "Wrong number string '" + number + "'");
        i++;
    }

    if (!is_number)
        throw Exception(EXCEPTION_CONVERT_BAD_STRING, "Wrong number string '" + number + "'");

    return result;
}


string 	Convert::byteVectorToString(const vector<byte_t> &v) {
    if (v.empty())
        return "";
    else {
        string result;

        for (unsigned int i = 0; i < v.size() - 1; i++)
            result += "0x" + numberToString(v[i], 16, 2) + " ";
        result += "0x" + numberToString(v[v.size() - 1], 16, 2);

        return result;
    }
}


string 	Convert::charVectorToString(const vector<byte_t> &v) {
    string result;

    result.assign(v.begin(), v.end());

    return result;
}


time_t  Convert::stringToDateTime(const string &date_time_string) throw(Convert::Exception) {
    if (date_time_string.size() != 10  &&  date_time_string.size() != 19)
        throw Exception(EXCEPTION_CONVERT_BAD_STRING, "Wrong date string '" + date_time_string + "'");

    tm      tm_;
    time_t  t    = 0;

    /*
     *  пример:
     *  date=2010-01-01
     */
    tm_.tm_year = stringToNumber(date_time_string.substr( 0, 4), 10) - 1900;
    tm_.tm_mon  = stringToNumber(date_time_string.substr( 5, 2), 10) - 1;
    tm_.tm_mday = stringToNumber(date_time_string.substr( 8, 2), 10);
    if (date_time_string.size() == 19) {
        tm_.tm_sec  = stringToNumber(date_time_string.substr(18, 2), 10);
        tm_.tm_min  = stringToNumber(date_time_string.substr(15, 2), 10);
        tm_.tm_hour = stringToNumber(date_time_string.substr(11, 2), 10);
    } else {
        tm_.tm_sec  = 0;
        tm_.tm_min  = 0;
        tm_.tm_hour = 0;
    }

    try {
        /* get current timeinfo and modify it to the user's choice */
        //time(&t);
        tm_ = *localtime(&t);
        /* call mktime: timeinfo->tm_wday will be set */
        mktime(&tm_);
    } catch(...) {
        throw Exception(EXCEPTION_CONVERT_BAD_DATE_TIME, "Wrong date time");
    }

    return t;
}


string  Convert::dateTimeToString(const time_t &date_time) {
    string          result;
    struct tm      *tm_;

    try {
        tm_                 =  localtime(&date_time);
        result              =  numberToString((tm_->tm_year + 1900), 10, 4)    + "-"
                            +  numberToString( tm_->tm_mon  + 1    , 10, 2)    + "-"
                            +  numberToString( tm_->tm_mday        , 10, 2)    + " "

                            +  numberToString( tm_->tm_hour        , 10, 2)    + ":"
                            +  numberToString( tm_->tm_min         , 10, 2)    + ":"
                            +  numberToString( tm_->tm_sec         , 10, 2);
    } catch(...) {
        throw Exception(EXCEPTION_CONVERT_BAD_DATE_TIME, "Wrong date time");
    }

    return result;
}


/**
 *  конвертировать длинную строку в список строк с переносом по словам, где длина каждой строки не больше max_string_length
 *  @param  max_string_length    максимальная длина каждой строки
 */
text_t  Convert::stringToFixedWideText(string &str_source, const size_t max_string_length) {
    if (max_string_length > 256)
        throw Exception(EXCEPTION_CONVERT_BAD_INDEX, "Wrong max string length " + numberToString(max_string_length));

    string word;
    string line;
    text_t result;

    for (size_t i = 0; i < str_source.size(); i++) {
        if (str_source[i] == ' '  &&  !word.empty()) {
            if (word.size() + line.size() > max_string_length) {
                result.push_back(line);
                line.clear();
            }
            line += word + " ";
            word.clear();

        } else
            word += str_source[i];
    }

    if (!line.empty())
        result.push_back(line);

    return result;
}


text_t  Convert::textToFixedWideText(const text_t &text_source, const size_t max_string_length) {
    if (max_string_length > 256)
        throw Exception(EXCEPTION_CONVERT_BAD_INDEX, "Wrong max string length " + numberToString(max_string_length));

    string str;

    for(size_t i = 0; i < text_source.size(); i++)
        str += text_source[i] + " ";

    return stringToFixedWideText(str, max_string_length);
}


}
