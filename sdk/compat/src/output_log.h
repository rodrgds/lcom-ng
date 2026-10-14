#ifndef LCOM_NG_COMPAT_OUTPUT_LOG_H
#define LCOM_NG_COMPAT_OUTPUT_LOG_H

/* Restore stdout, drain its pending log bytes, and return any I/O error. */
int lcom_compat_output_log_stop(void);

#endif
