#ifndef _HELPERS_H_
#define _HELPERS_H_

/* Compilers must hate me :) */

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* Add more options here */
#define INFO "info"
#define ERROR "error"
#define WARNING "warn"

#define log_INFO std::cout << ANSI_COLOR_GREEN << INFO << ANSI_COLOR_RESET
#define log_ERROR std::cerr << ANSI_COLOR_RED << ERROR << ANSI_COLOR_RESET
#define log_WARNING std::cout << ANSI_COLOR_YELLOW << WARNING << ANSI_COLOR_RESET

#define meslog(type) log_##type meta_log
#define meta_log << ":" << __FILE__ << ":" << __func__ << ":" << __LINE__ << " \t"

#endif