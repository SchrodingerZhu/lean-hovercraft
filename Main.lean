import «LeanHovercraft»

def main : IO Unit := do
  let arr := "3.1415926,3.1415926535,3.1415926e123,-3.1415926,inf,nan".toFloatArray
  for i in arr do
    println! i.toStringInfinite
  -- println! 3.1415926.toStringFinite
  -- println! (1.0/0.0).toStringInfinite
  -- println! (1.0/0.0).toString
  -- println! ("hello: ".pushFloatFinite 3.141592653511111111111111111111E111)
  -- let mut buf : String := ""
  -- for i in [0:100000] do
  --   buf := buf.pushFloatFinite $ i.toFloat * 3.14159265351111111
  -- println! buf
