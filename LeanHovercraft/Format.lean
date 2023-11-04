
@[extern "lean_hovercraft_float_to_string_finite"]
opaque Float.toStringFinite : Float → String

@[extern "lean_hovercraft_float_to_string_infinite"]
opaque Float.toStringInfinite : Float → String

@[extern "lean_hovercraft_string_push_float_finite"]
opaque String.pushFloatFinite : String → Float → String

@[extern "lean_hovercraft_string_push_float_infinite"]
opaque String.pushFloatInfinite : String → Float → String
