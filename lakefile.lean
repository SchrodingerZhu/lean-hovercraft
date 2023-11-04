import Lake
open Lake DSL

package «lean-hovercraft» where
  -- add package configuration options here

lean_lib «LeanHovercraft» where
  -- add library configuration options here

@[default_target]
lean_exe «lean-hovercraft» where
  root := `Main
  -- Enables the use of the Lean interpreter by the executable (e.g.,
  -- `runFrontend`) at the expense of increased binary size on Linux.
  -- Remove this line if you do not need such functionality.
  supportInterpreter := true


extern_lib liblean_ryu pkg := do
  let name := nameToStaticLib "lean_ryu"
  let libFile := pkg.buildDir / "lib" / name
  let cargoFile ← inputFile <| pkg.dir / "lean_ryu" / "Cargo.toml"
  let libSrc ← inputFile <| pkg.dir / "lean_ryu" / "src" / "lib.rs"
  let currentDir ← IO.currentDir
  let absTargetDir := currentDir / pkg.buildDir / "rust"
  let absOutDir := currentDir / pkg.buildDir / "lib"
  buildFileAfterDepArray libFile #[cargoFile, libSrc] (fun _ => proc {
    cmd := "cargo",
    args := #["build", "--release", "-Zunstable-options", "--target-dir", absTargetDir.toString, "--out-dir", absOutDir.toString],
    cwd := pkg.dir / "lean_ryu"
  } true) (pure BuildTrace.nil)

def flags : SchedulerM (Array String) := do
  let extraFlags ← IO.getEnv "LEAN_HOVERCRAFT_FLAGS"
  let extraFlags := extraFlags.getD "" |>.trim |>.splitOn " " |>.filter (·.length > 0)
  return #[
    "-I", (← getLeanIncludeDir).toString,
    "-fPIC", "-std=gnu++17", "-O3", "-fvisibility=hidden", "-ffreestanding"
  ] ++ extraFlags

def objectFile (pkg : Package) (name : String) : SchedulerM (BuildJob FilePath) := do
  let oFile := pkg.buildDir / "cxx" / (name ++ ".o")
  let srcJob ← inputFile <| pkg.dir / "cxx" / (name ++ ".cpp")
  let compiler := match ← IO.getEnv "CXX" with
    | some cxx => cxx
    | none => "c++"
  buildO (name ++ ".cpp") oFile srcJob #[] (← flags) compiler

target format.o pkg : FilePath := objectFile pkg "format"
target parser.o pkg : FilePath := objectFile pkg "parser"
extern_lib liblean_hovercraft.a pkg := do
  let name := nameToStaticLib "lean_hovercraft"
  let formatO ← fetch <| pkg.target ``format.o
  let parserO ← fetch <| pkg.target ``parser.o
  buildStaticLib (pkg.nativeLibDir / name) #[formatO, parserO]
