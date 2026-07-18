#ifndef ENDE_PLATFORM_H
#define ENDE_PLATFORM_H

#ifdef __linux__

#define ENDE_LINUX
#define ENDE_UNIX

#define PRETTY_FUNC __PRETTY_FUNCTION__
#define FORCEINLINE __attribute__((always_inline))
#define NOINLINE __attribute__((noinline))

#elif defined __APPLE__

#define ENDE_APPLE
#define ENDE_UNIX

#define PRETTY_FUNC __PRETTY_FUNCTION__
#define FORCEINLINE __attribute__((always_inline))
#define NOINLINE __attribute__((noinline))

#elif defined _WIN32

#define ENDE_WIN

#define PRETTY_FUNC __FUNCTION__
#define FORCEINLINE __forceinline
#define NOINLINE __declspec(noinline)

#else

#error "unsupported system"

#endif

#ifdef __clang__
#define ENDE_CLANG
#elif (defined __GNUC__ || defined __GNUG__) && !(defined __clang__ || defined __INTEL_COMPILER)
#define ENDE_GNU
#elif defined __ICC
#define ENDE_INTEL
#elif defined __MSC_VER
#define ENDE_MSCV
#endif

#ifdef __SSE3__
#define ENDE_SSE3
#elif defined __SSE2__
#define ENDE_SSE2
#elif defined __SSE__
#define ENDE_SSE
#elif defined __AVX2__
#define ENDE_AVX2
#elif defined __AVX__
#define ENDE_AVX
#else
#define ENDE_SCALAR
#endif

#endif // ENDE_PLATFORM_H
