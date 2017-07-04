#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#define LOG_LEVEL_DEBUG     0
#define LOG_LEVEL_INFO      1
#define LOG_LEVEL_ERROR     2

#ifdef __cplusplus
extern "C"
{
#endif

extern void debug_init(int level, const char *tag, const char *logdir);

extern void debug_printf(int level, const char* function, const char *format, ...);

#ifdef NDEBUG

#define LOGD(...)                   do{}while(0)
#define LOGI(...)                   do{}while(0)
#define LOGE(...)                   debug_printf(LOG_LEVEL_ERROR, __FUNCTION__, ##__VA_ARGS__)

#else

#define LOGD(...)           debug_printf(LOG_LEVEL_DEBUG, __FUNCTION__, ##__VA_ARGS__)
#define LOGI(...)           debug_printf(LOG_LEVEL_INFO,  __FUNCTION__, ##__VA_ARGS__)
#define LOGE(...)           debug_printf(LOG_LEVEL_ERROR, __FUNCTION__, ##__VA_ARGS__)

#endif  //NDEBUG

#define ASSERT(expr, msg)  do{ \
    if(!(expr)) { \
        LOGE("ASSERT FAILE"); \
        abort(); \
    } }while(0)

#ifdef __cplusplus
}
#endif

#endif //DEBUG_H
