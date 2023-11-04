#include <cmath>
#include <lean/lean.h>
#include "../fast_float/include/fast_float/fast_float.h"

extern "C" double lean_hovercraft_string_to_float(b_lean_obj_arg s, size_t format) {
  size_t sz = lean_string_size(s);
  const char *str = lean_string_cstr(s);
  double value;
  auto result = fast_float::from_chars(str, str + sz - 1, value, static_cast<fast_float::chars_format>(format));
  if (result.ec != std::errc()) {
    return NAN;
  }
  return value; 
}

extern "C" lean_obj_res lean_hovercraft_string_to_float_delimited(b_lean_obj_arg s, size_t format) {
  size_t sz = lean_string_size(s);
  char const*str = lean_string_cstr(s);

  double value;
  lean_obj_res array = lean_alloc_sarray(sizeof(double), 0, 64);
  while (str < str + sz - 1) {
    auto result = fast_float::from_chars(str, str + sz - 1, value, static_cast<fast_float::chars_format>(format));
    if (result.ec != std::errc()) {
      break;
    }
    array = lean_float_array_push(array, value);
    str = result.ptr + 1;
  }
  return array;
}