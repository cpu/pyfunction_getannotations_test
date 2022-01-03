{ nixpkgs ? import <nixpkgs> { } }:
nixpkgs.stdenv.mkDerivation rec {
  pname = "pyfunction_getannotations_test";
  version = "0.0.1";

  src = builtins.path {
    path = ./.;
    name = pname;
  };

  nativeBuildInputs = [ nixpkgs.ncurses nixpkgs.python310 nixpkgs.python39 ];
}
