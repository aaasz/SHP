#ifndef FSL_IF_H
#define FSL_IF_H

/* FSL assignments */
#if !defined(IMPULSE_FIRST_FSL)

#define p_test_function_one_s_out() "rfsl0"
#define p_test_function_one_s_in() "rfsl1"
#define p_config() "rfsl2"
#else

#if IMPULSE_FIRST_FSL == 0

#define p_test_function_one_s_out() "rfsl0"
#define p_test_function_one_s_in() "rfsl1"
#define p_config() "rfsl2"
#endif

#if IMPULSE_FIRST_FSL == 1

#define p_test_function_one_s_out() "rfsl1"
#define p_test_function_one_s_in() "rfsl2"
#define p_config() "rfsl3"
#endif

#if IMPULSE_FIRST_FSL == 2

#define p_test_function_one_s_out() "rfsl2"
#define p_test_function_one_s_in() "rfsl3"
#define p_config() "rfsl4"
#endif

#if IMPULSE_FIRST_FSL == 3

#define p_test_function_one_s_out() "rfsl3"
#define p_test_function_one_s_in() "rfsl4"
#define p_config() "rfsl5"
#endif

#if IMPULSE_FIRST_FSL == 4

#define p_test_function_one_s_out() "rfsl4"
#define p_test_function_one_s_in() "rfsl5"
#define p_config() "rfsl6"
#endif

#if IMPULSE_FIRST_FSL == 5

#define p_test_function_one_s_out() "rfsl5"
#define p_test_function_one_s_in() "rfsl6"
#define p_config() "rfsl7"
#endif

#if IMPULSE_FIRST_FSL == 6

#define p_test_function_one_s_out() "rfsl6"
#define p_test_function_one_s_in() "rfsl7"
#define p_config() "rfsl8"
#endif

#if IMPULSE_FIRST_FSL == 7

#define p_test_function_one_s_out() "rfsl7"
#define p_test_function_one_s_in() "rfsl8"
#define p_config() "rfsl9"
#endif

#endif


#endif
