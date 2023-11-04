def FloatFormat.scientific     : USize := (1 : USize) <<< 0
def FloatFormat.fixed          : USize := (1 : USize) <<< 2
def FloatFormat.hex            : USize := (1 : USize) <<< 3
def FloatFormat.no_infnan      : USize := (1 : USize) <<< 4
def FloatFormat.json_fmt       : USize := (1 : USize) <<< 5
def FloatFormat.fortran_fmt    : USize := (1 : USize) <<< 6
def FloatFormat.json           : USize :=
  FloatFormat.json_fmt ||| FloatFormat.scientific ||| FloatFormat.fixed ||| FloatFormat.no_infnan
def FloatFormat.json_or_infnan : USize :=
  FloatFormat.json_fmt ||| FloatFormat.scientific ||| FloatFormat.fixed
def FloatFormat.fortran        : USize :=
  FloatFormat.fortran_fmt ||| FloatFormat.scientific ||| FloatFormat.fixed
def FloatFormat.general        : USize :=
  FloatFormat.scientific ||| FloatFormat.fixed

@[extern "lean_hovercraft_string_to_float"]
opaque String.toFloat (s : String) (format : USize := FloatFormat.general) : Float

@[extern "lean_hovercraft_string_to_float_delimited"]
opaque String.toFloatArray (s : String) (format : USize := FloatFormat.general) : FloatArray
