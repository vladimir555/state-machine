/*
 * main.cpp
 *
 *  Created on: 01.08.2011
 *      Author: volodja
 */


//TODO: сделать в логе добавление имени потока
//TODO: сделать два отдельных логера, один для программистов другой для админов
//TODO: добавить классы персистенции иерархии классов
//TODO: сделать классы протоколов обмена с устройствами и от них наследовать реализации конкретных устройств с конечным автоматом на разных уровнях абстракции
//TODO: сделать запрос состояния устройств с выводом инфы в консоль для админов

#include "main.h"


int main() {
    network::Service service;

    service.start();

    return 0;
}