#!/usr/bin/env bash

set -eou pipefail

function binary_path()
{
	local preset="$1"

	echo "build/${preset}/App/typetest"
}

function run_workflow()
{
	local workflow="$1"

	cmake --workflow --preset "$workflow"
}

function run_debug()
{
	run_workflow "dev"

	local binary
	binary=$(binary_path "debug")

	if [[ -n "${TMUX-}" ]]; then
		local port
		port=$(shuf -i 2000-65000 -n 1)

		echo "Starting gdbserver on port ${port}..."

		tmux split-window -h -p 50 \
			"exec gdbserver :${port} '${binary}'"

		# Small delay so gdbserver is listening
		sleep 0.2

		tmux select-pane -t left

		gdb --tui -q \
			-ex "file ${binary}" \
			-ex "target remote :${port}"
	else
		gdb --tui -q "${binary}"
	fi
}

function link_compile_commands()
{
    local preset="$1"

    local build="./build"

    local link="${build}/compile_commands.json"
    if [[ ! -f "${link}" ]]; then
        echo "Linking compile_commands ..."

        local src="./${preset}/compile_commands.json"
        (cd "${build}" && ln -s "${src}" .)
    fi
}

function run()
{
	run_workflow "dev"

    link_compile_commands "debug"

	exec "$(binary_path "debug")"
}

MODE="${1-}"

case "$MODE" in
	debug | d)
		echo -e "\tDebug\n"
		run_debug
		;;
	build | b)
		echo -e "\tBuild\n"
		run_workflow "dev"
		;;
	run | r)
		run
		;;
	*)
		run
		;;
esac
