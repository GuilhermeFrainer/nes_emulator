#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>

#define assert_eq_u8(actual, expected) \
    if (actual == expected) { successful_tests++; } \
    else { printf("%s failed at %s at line %i.\n", __FUNCTION__,  __FILE__, __LINE__); failed_tests++; }

#define end_tests() printf("Testing over:\nSucceeded: %i\nFailed: %i\n", successful_tests, failed_tests);

#endif