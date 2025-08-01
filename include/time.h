#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

typedef long time_t;

struct timespec {
        time_t second;
        long nanosecond;
};

typedef struct timespec timespec_t;

#endif // INCLUDE_TIME_H_
