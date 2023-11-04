#include <cmath>
#include <cstring>
#include <lean/lean.h>
#include <string.h>

extern "C" size_t lean_hovercraft_format64(double x, char *buffer);

static inline lean_obj_res prepare_for_modfication(lean_obj_arg o, size_t extra) {
    size_t sz  = lean_string_size(o);
    size_t cap = lean_string_capacity(o);
    size_t len = lean_string_len(o);
    if (LEAN_UNLIKELY(!lean_is_exclusive(o))) {
        lean_obj_arg r = lean_alloc_string(sz, (sz + extra) * 2, len);
        ::memcpy(const_cast<char*>(lean_string_cstr(r)), lean_string_cstr(o), sz - 1);
        lean_dec_ref(o);
        return r;
    } else {
        if (sz + extra > cap) {
          lean_obj_res new_o = lean_alloc_string(sz, cap + sz + extra, lean_string_len(o));
          ::memcpy(const_cast<char*>(lean_string_cstr(new_o)), lean_string_cstr(o), sz);
          lean_free_object(o);
          return new_o;
        } else {
          return o;
        }
    }
}

extern "C" LEAN_EXPORT lean_obj_res lean_hovercraft_float_to_string_finite(double x) {
    lean_obj_res r = lean_alloc_string(0, 25, 0);
    size_t consumed = lean_hovercraft_format64(x, const_cast<char*>(lean_string_cstr(r)));
    lean_to_string(r)->m_size = consumed + 1;
    lean_to_string(r)->m_length = consumed;
    const_cast<char*>(lean_string_cstr(r))[consumed] = 0;
    return r;
}

#define LEAN_HOVERCRAFT_MK_CONSTANT_STRING(name, value) \
  static struct { \
    lean_string_object m_string; \
    char data[sizeof(value)]; \
  } const_##name##_str = { \
    .m_string = { \
      .m_header = { \
        .m_rc = 0, \
        .m_cs_sz = 0, \
        .m_other = 0, \
        .m_tag = LeanString, \
      }, \
      .m_size = sizeof(value), \
      .m_capacity = sizeof(value), \
      .m_length = sizeof(value) - 1, \
    }, \
    .data = value, \
  }

LEAN_HOVERCRAFT_MK_CONSTANT_STRING(nan, "NaN");
LEAN_HOVERCRAFT_MK_CONSTANT_STRING(inf, "inf");
LEAN_HOVERCRAFT_MK_CONSTANT_STRING(neginf, "-inf");

extern "C" LEAN_EXPORT lean_obj_res lean_hovercraft_float_to_string_infinite(double x) {
    if (std::isfinite(x)) {
        return lean_hovercraft_float_to_string_finite(x);
    } else {
        if (std::isnan(x)) {
            return &const_nan_str.m_string.m_header;
        } else if (std::signbit(x)) {
            return &const_neginf_str.m_string.m_header;
        } else {
            return &const_inf_str.m_string.m_header;
        }
    }
}

extern "C" LEAN_EXPORT lean_obj_res lean_hovercraft_string_push_float_finite(lean_obj_arg s, double x) {
    size_t sz  = lean_string_size(s);
    size_t len = lean_string_len(s);
    lean_obj_res r = prepare_for_modfication(s, 24);
    size_t consumed = lean_hovercraft_format64(x, const_cast<char*>(lean_string_cstr(r)) + sz - 1);
    lean_to_string(r)->m_size = sz + consumed;
    lean_to_string(r)->m_length += consumed;
    const_cast<char*>(lean_string_cstr(r))[sz + consumed - 1] = 0;
    return r;
}

extern "C" LEAN_EXPORT lean_obj_res lean_hovercraft_string_push_float_infinite(lean_obj_arg s, double x) {
    if (std::isfinite(x)) {
        return lean_hovercraft_string_push_float_finite(s, x);
    } else {
        auto copy = [](lean_obj_res s, size_t extra, const char * data) LEAN_ALWAYS_INLINE {
          auto sz = lean_string_size(s);
          auto r = prepare_for_modfication(s, extra);
          for (size_t i = 0; i <= extra; i++) {
            const_cast<char*>(lean_string_cstr(r))[sz + i - 1] = data[i];
          }
          lean_to_string(r)->m_size += extra;
          lean_to_string(r)->m_length += extra;
          return r;
        };
        if (std::isnan(x)) {
          return copy(s, 3, "NaN");
        } else if (std::signbit(x)) {
          return copy(s, 4, "-inf");
        } else {
          return copy(s, 3, "inf");
        }
    }
}

