import «LeanHovercraft»

def main : IO Unit := do
  println! 3.1415926.toStringFinite
  println! (1.0/0.0).toStringInfinite
  println! (1.0/0.0).toString
  println! ("hello: ".pushFloatFinite 3.141592653511111111111111111111E111)
  let mut buf : String := ""
  for i in [0:100000] do
    buf := buf.pushFloatFinite $ i.toFloat * 3.14159265351111111
  println! buf
