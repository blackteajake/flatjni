#include "fjlog.h"

#include <stdarg.h>
#include <pthread.h>
#include <string.h>

#ifdef ANDROID
#include "android/log.h"
static int android_level[] = {
    ANDROID_LOG_DEBUG,
    ANDROID_LOG_INFO,
    ANDROID_LOG_ERROR
};
#endif

#define LOG_FILE_MAX_SIZE        (1024 * 1024 * 3)

static int _level = LOG_LEVEL_DEBUG;
static char _tag[32] = {"jni"};

static char _log[256] = {"jni_0.log"};
static char _logbak[256] = {"jni_1.log"};
static pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
static FILE* _fp = NULL;
static char _buf[512] = {0};

static void check_rotation() {
    if (ftell(_fp) >= LOG_FILE_MAX_SIZE) {
        fclose(_fp);
        rename(_log, _logbak);
        _fp = fopen(_log, "a");
    }
}

void debug_init(int level, const char *tag, const char *logdir)
{
    _level = level;
    snprintf(_tag, sizeof(_tag), "%s", tag);
    if (logdir != NULL && logdir[0]) {
        snprintf(_log, sizeof(_log), "%s/%s_0.log", logdir, _tag);
        snprintf(_logbak, sizeof(_logbak), "%s/%s_1.log", logdir, _tag);
    }
    _fp = fopen(_log, "a");
    if (_fp != NULL)
        check_rotation();
#ifdef ANDROID
    __android_log_print(ANDROID_LOG_ERROR, _tag, "create log file:%s %s!\n", _log, (_fp == NULL) ? "error" : "ok");
#else
    fprintf(stderr, "%s create log file:%s %s!\n", _tag, _log, (_fp == NULL) ? "error" : "ok");
#endif
}


void debug_printf(int level, const char* function, const char *format, ...)
{
    if(level < _level)
        return;

    va_list args;
    time_t t = time(NULL);
    char str[512] = {0};

    va_start(args, format);
    vsnprintf(str, sizeof(str), format, args);
#ifdef ANDROID
    __android_log_print(android_level[level], _tag, "[%s] %s\n", function, str);
#else
    fprintf(stderr, "%s [%s] %s\n", _tag, function, str);
#endif
    va_end(args);

    if(_fp != NULL)
    {
        pthread_mutex_lock(&_mutex);
        if (_fp != NULL) {
            snprintf(_buf, sizeof(_buf), "%02ld:%02ld:%02ld [%s] %s\n", (t / 3600 + 8) % 24, t % 3600 / 60, t % 3600 % 60, function, str);
            fputs(_buf, _fp);
            if (level > LOG_LEVEL_INFO)
                fflush(_fp);
            check_rotation();
        }
        pthread_mutex_unlock(&_mutex);
    }
}
