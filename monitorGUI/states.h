#ifndef STATES_H
#define STATES_H

#include <QString>

inline QString stateToString(int state) {
    switch(state) {
    case 0:    return "Неопределено";
    case 1: return "Не работает";
    case 2: return "Авария";
    case 3:    return "Работает";
    default:         return "Неопределено";
    }
}

#endif // STATES_H
