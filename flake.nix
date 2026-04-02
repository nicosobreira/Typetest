{
  description = "Multi-platform dev shell with safe platform-specific tools";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
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
        inherit (pkgs) lib stdenv;
      in {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = with pkgs;
            [
              git
              cmake
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
