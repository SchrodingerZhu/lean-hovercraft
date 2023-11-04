import «LeanHovercraft»

def main : IO Unit := do
  println! 3.1415926.toStringFinite
  println! (1.0/0.0).toStringInfinite
  println! (1.0/0.0).toString
  println! ("hello: ".pushFloatFinite 3.141592653511111111111111111111E111)
  println! ("hello: ".pushFloatInfinite (-1.0/0.0))
