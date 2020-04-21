#ifndef _HELPERS_H_
#define _HELPERS_H_

/* Compilers must hate me :) */

#define INFO "info"
#define ERROR "error"
#define WARNING "warning"

#define log(type) log_##type meta_log

#define log_INFO std::cout << INFO 
#define log_ERROR std::cerr << ERROR 
#define log_WARNING std::cout << WARNING 

#define meta_log << ":" << __FILE__ << ":" << __func__ << ":" << __LINE__ << " \t"

#endif