#ifndef LYNX_BASE_COMPILER_SPECIFIC_H_
#define LYNX_BASE_COMPILER_SPECIFIC_H_

#if !defined(UNLIKELY)
#if defined(COMPILER_GCC)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define UNLIKELY(x) (x)
#endif
#endif

#endif