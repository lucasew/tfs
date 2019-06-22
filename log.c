#ifndef _TFS_LOG
#define _TFS_LOG
#include <stdio.h>
#define _tfs_log_prefix(boldness, color, prefix) \
    printf("\033[%i;%im%c: \033[0m", boldness, color, prefix);

#define tfs_log_err(...) \
    _tfs_log_prefix(0, 31, 'E'); \
    printf(__VA_ARGS__); 

#define tfs_log_warn(...) \
    _tfs_log_prefix(0, 33, 'W'); \
    printf(__VA_ARGS__); 

#define tfs_log_info(...) \
    _tfs_log_prefix(0, 34, 'I'); \
    printf(__VA_ARGS__); 


#endif
