#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <android/log.h>
#include <sys/time.h>
#include <dlfcn.h>
#include "sample.h"

#define SAMPLE_TAG "bytehook_tag"

#pragma clang optimize off
void sample_test_strlen(int benchmark)
{
    if(!benchmark) __android_log_print(ANDROID_LOG_INFO, SAMPLE_TAG, "sample pre strlen");
    size_t len = strlen(benchmark ? "1" : "bytehook manual test");
    if(!benchmark) __android_log_print(ANDROID_LOG_INFO, SAMPLE_TAG, "sample post strlen = %zu", len);
}
#pragma clang optimize on
