#ifndef GL_LOG_H
#define GL_LOG_H
#include "stdafx.h"

/* Define function prototypes */
bool restart_gl_log(void);
bool gl_log(const char* message, ...);
bool gl_log_err(const char* message, ...);
void log_gl_params(void);
const char* GL_type_to_string(GLenum type);

#endif // #if_n_def gl_log_h