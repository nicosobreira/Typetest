{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  # Tools
  nativeBuildInputs = with pkgs; [
    git
    cmake

    clang-tools
    cmake-language-server

    # Profiling
    perf
    hotspot
    valgrind
  ];

  # Libraries
  buildInputs = with pkgs; [
    ncurses
  ];
}
