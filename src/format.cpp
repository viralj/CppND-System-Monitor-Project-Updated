#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

// DONE: ElapsedTime is helper function that will take long int seconds and it will output formatted time
string Format::ElapsedTime(long times) {

    long _times = times;
    long hour = 0, minute = 0, _seconds = 0;
    string date;

    hour = _times / 3600;
    _times = _times % 3600;
    minute = _times / 60;
    _times = _times % 60;
    _seconds = _times;
    date = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(_seconds);

    return date;
}