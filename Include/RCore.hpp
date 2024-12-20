#pragma once

#include <iostream>

/**
 * Assertions can be disabled with `#define REFLECTION_ASSERT_DISABLE`.
 * Custom assertion handling can be defined with `#define REFLECTION_ASSERT(Expr)` and `#define REFLECTION_ASSERT_MSG(Expr, Message)`
 */

#ifdef REFLECTION_ASSERT_DISABLE
    #define REFLECTION_ASSERT(Expr) ((void)0)
#else
    #ifndef REFLECTION_ASSERT
        #define REFLECTION_ASSERT(Expr, ...) \
            do { \
                if (!(Expr)) { \
                    std::cerr << "Reflection Assertion failed: " << #Expr << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
                    std::abort(); \
                } \
            } while (0)
    #endif
    #ifndef REFLECTION_ASSERT_MSG
    #define REFLECTION_ASSERT_MSG(Expr, Message) \
                do { \
                    if (!(Expr)) { \
                        std::cerr << "Reflection Assertion failed: " << #Expr << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
                        std::cerr << " - " << Message << std::endl; \
                        std::abort(); \
                    } \
                } while (0)
    #endif
#endif