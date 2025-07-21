#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

typedef long time_t;

typedef struct timespec {
        time_t second;
        long nanosecond;
} timespec;

#endif // INCLUDE_TIME_H_
