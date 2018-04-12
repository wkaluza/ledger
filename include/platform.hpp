#ifndef PLATFORM_HPP
#define PLATFORM_HPP
namespace fetch {
namespace platform {

constexpr bool has_avx() {
#ifdef __AVX__
  return true;
#else
  return false;
#endif
}

constexpr bool has_avx2() {
#ifdef __AVX2__
  return true;
#else
  return false;
#endif

}

constexpr bool has_sse() {
#ifdef __SSE__
  return true;
#else
  return false;
#endif
};


constexpr bool has_sse2() {
#ifdef __SSE2__
  return true;
#else
  return false;
#endif
};

constexpr bool has_sse3() {
#ifdef __SSE3__
  return true;
#else
  return false;
#endif
};

constexpr bool has_sse41() {
#ifdef __SSE4_1__
  return true;
#else
  return false;
#endif
};
  
constexpr bool has_sse42() {
#ifdef __SSE4_2__
  return true;
#else
  return false;
#endif
};
  
};
};

#endif
