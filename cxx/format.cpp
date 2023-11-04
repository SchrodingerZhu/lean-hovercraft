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

extern "C" LEAN_EXPORT lean_obj_res lean_hovercraft_float_to_string_infinite(double x) {
    if (std::isfinite(x)) {
        return lean_hovercraft_float_to_string_finite(x);
    } else {
        if (std::isnan(x)) {
            return lean_mk_string("NaN");
        } else if (std::signbit(x)) {
            return lean_mk_string("-inf");
        } else {
            return lean_mk_string("inf");
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
        const char * data;
        auto sz = lean_string_size(s);
        if (std::isnan(x)) {
          data = "NaN";
        } else if (std::signbit(x)) {
          data = "-inf";
        } else {
          data = "inf";
        };
        auto extra = strlen(data);
        auto r = prepare_for_modfication(s, extra);
        memcpy(const_cast<char*>(lean_string_cstr(r)) + sz - 1, data, extra + 1);
        lean_to_string(r)->m_size += extra;
        lean_to_string(r)->m_length += extra;
        return r;
    }
}

