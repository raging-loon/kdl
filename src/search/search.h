#ifndef SEARCH_SEARCH_H_
#define SEARCH_SEARCH_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#ifndef bool
#define bool int
#endif // bool

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // FALSE

typedef bool (*KDL_FILE_SEARCH_FN)(void);

#ifdef __cplusplus
} // extern "C"
#endif //__cplusplus

#endif // SEARCH_SEARCH_H_