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
inline QString stateToString(int value) {
    State state = intToState(value);
    switch(state) {
    case State::Undefined:  return "Неопределено";
    case State::NotWorking: return "Не работает";
    case State::Failure:    return "Авария";
    case State::Working:    return "Работает";
    default:                return "Неопределено";
    }
}

#endif // STATES_H
