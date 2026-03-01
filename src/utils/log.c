/*
 * Utilitários para mostra de relatórios mais coloridos
 */

#include "log.h"
#include <stdio.h>

void tfs_report_error(const char *msg) {
    tfs_log_err("%s\n", msg);
    // In the future, this can be wired to Sentry or another centralized reporting system
}
