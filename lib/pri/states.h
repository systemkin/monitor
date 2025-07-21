#ifndef STATES_H
#define STATES_H

#include <QString>

enum class State {
    Undefined = 0,
    NotWorking = 1,
    Failure = 2,
    Working = 3
};
inline State intToState(int value) {
    return static_cast<State>(value);
}
inline int stateToInt(State value) {
    return static_cast<int>(value);
}
inline QString stateToString(State value) {
    switch(value) {
    case State::Undefined:  return "Неизвестно";
    case State::NotWorking: return "Не работает";
    case State::Failure:    return "Авария";
    case State::Working:    return "Работает";
    default:                return "Неизвестно";
    }
}

#endif // STATES_H
