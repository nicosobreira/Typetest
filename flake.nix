{
  description = "C/C++ Clang Shell";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = import nixpkgs {inherit system;};

        toolchain = pkgs.llvmPackages_latest;
      in {
        devShells.default =
          pkgs.mkShell.override {
            stdenv = toolchain.stdenv;
          } {
            nativeBuildInputs = with pkgs;
              [
                cmake
                ninja

                clang-tools
                cmake-language-server
                valgrind
              ]
              ++ lib.optionals stdenv.isLinux [
                perf
                hotspot
              ];

            buildInputs = with pkgs; [
              ncurses
            ];
          };
      }
    );
}
