#ifndef INCLUDE_COMMON_HPP_7F6B3050_43A0_4E35_A983_A38D99685CF4
#define INCLUDE_COMMON_HPP_7F6B3050_43A0_4E35_A983_A38D99685CF4

#include <string>
#include <iosfwd>
#include <memory>
#include <thread>

#include <cxxabi.h>

#define LOG (std::cerr << __FILE__ << ':' << __LINE__ << ':' << std::this_thread::get_id() << ": ")


template <typename T>
inline auto demangle(T = T()) {
  auto deleter = [](char* p) { free(p); };
  return std::string(std::unique_ptr<char, decltype (deleter)>(abi::__cxa_demangle(typeid (T).name(), 0, 0, nullptr),
							       deleter).get());
}

template <typename T, typename D>
inline auto ptr(T* ptr, D del) {
  auto deleter = [del](T* p) {
    LOG << "auto deletion: " << p << ':' << demangle<T*>(p) << std::endl;
    del(p);
  };
  return std::unique_ptr<T, decltype (deleter)>(ptr, deleter);
}


template <typename First>
inline auto concat(First first) {
  return std::string(first);
}
template <typename First, typename... Rest>
inline auto concat(First first, Rest... rest) {
  return first + concat(rest...);
}

#endif/*INCLUDE_COMMON_HPP_7F6B3050_43A0_4E35_A983_A38D99685CF4*/
