#!/usr/bin/env bash

set -eou pipefail

BINARY="./build/App/typetest"

function run_build()
{
	local preset="$1"
	local build_dir="${2-build}"

	cmake --preset "$preset"
	cmake --build "$build_dir"
}

function run_debug() {
  local preset="debug"

  # Pick a free TCP port
  local port
  port=$(shuf -i 2000-65000 -n 1)

  run_build "debug" "build"

  if [[ -n "${TMUX-}" ]]; then
    echo "Starting gdbserver on port ${port}..."

    tmux split-window -h -p 50 \
      "exec gdbserver :${port} '${BINARY}'"

    # Small delay so gdbserver is listening
    sleep 0.2

    tmux select-pane -t left

    gdb --tui -q \
      -ex "file ${BINARY}" \
      -ex "target remote :${port}"

  else
    gdb --tui -q "${BINARY}"
  fi
}

function run()
{

	run_build "debug" "build"

	exec "$BINARY"
}

MODE="${1-}"

case "$MODE" in
	profile | p)
		echo -e "\tProfile\n"
		run_profile
		;;
	debug | d)
		echo -e "\tDebug\n"
		run_debug
		;;
	build | b)
		echo -e "\tBuild\n"
		run_build "debug" "build"
		;;
	run | r)
		run
		;;
	*)
		run
		;;
esac
