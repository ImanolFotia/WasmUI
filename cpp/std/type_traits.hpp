#pragma once

#include <stddef.h>
#include <stdint.h>

namespace std {
template <class T, T v> struct integral_constant {
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant; // using injected-class-name
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept {
    return value;
  } // since c++14
};

template <class T> struct remove_cv {
  typedef T type;
};
template <class T> struct remove_cv<const T> {
  typedef T type;
};
template <class T> struct remove_cv<volatile T> {
  typedef T type;
};
template <class T> struct remove_cv<const volatile T> {
  typedef T type;
};

template <class T> struct remove_const {
  typedef T type;
};
template <class T> struct remove_const<const T> {
  typedef T type;
};

template <class T> struct remove_volatile {
  typedef T type;
};

template <bool B> using bool_constant = integral_constant<bool, B>;

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <class T, class U> struct is_same : false_type {};

template <class T> struct is_same<T, T> : true_type {};
/*
template <class T>
    struct is_integral
    : bool_constant <
      (requires(T t, T* p,
               void (*f)(T))  // T* parameter excludes reference types
{
  reinterpret_cast<T>(t); // Exclude class types
  f(0);                   // Exclude enumeration types
  p + t; // Exclude everything not yet excluded but integral types
})>{};
*/

template <class T>
struct is_integral
    : integral_constant<
          bool,
          // Note: standard floating-point types
          is_same<uint8_t, typename remove_cv<T>::type>::value ||
          is_same<uint16_t, typename remove_cv<T>::type>::value ||
          is_same<uint32_t, typename remove_cv<T>::type>::value ||
          is_same<uint64_t, typename remove_cv<T>::type>::value ||
          is_same<int8_t, typename remove_cv<T>::type>::value ||
          is_same<int16_t, typename remove_cv<T>::type>::value ||
          is_same<int32_t, typename remove_cv<T>::type>::value ||
          is_same<int64_t, typename remove_cv<T>::type>::value ||
          is_same<size_t, typename remove_cv<T>::type>::value ||
          is_same<unsigned long, typename remove_cv<T>::type>::value ||
          is_same<long, typename remove_cv<T>::type>::value ||
          is_same<int, typename remove_cv<T>::type>::value ||
          is_same<unsigned int, typename remove_cv<T>::type>::value ||
          is_same<int, typename remove_cv<T>::type>::value> {};
 // namespace std

template <class T>
struct is_floating_point
    : integral_constant<
          bool,
          // Note: standard floating-point types
          is_same<float, typename remove_cv<T>::type>::value ||
              is_same<double, typename remove_cv<T>::type>::value ||
              is_same<long double, typename remove_cv<T>::type>::value> {};
} // namespace std