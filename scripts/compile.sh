#!/usr/bin/bash
#
# This script is used by Meson test system to test that certain
# files do not compile.
#
# shellcheck disable=SC2016

function extract_compiler_path_from_meson_object() {
    local meson_compiler_object="$1"

    echo "${meson_compiler_object}" | grep --color=never -Po ' `\K[^`]*' | awk '{print $NF}'
}

function compile_file() {
    local source_file="$1"
    local expect_failure="$2"
    local compiler="$3"
    shift
    shift
    shift
    local includes=("$@")
    local arguments=()

    for include in "${includes[@]}"; do
        arguments+=("-I${include}")
    done

    if [[ "$expect_failure" == "1" ]]; then
        if "${compiler}" "${source_file}" "${arguments[@]}" 2> /dev/null > /dev/null; then
            echo "expected '${source_file}' to not build, but it did"
            exit 1
        fi
    else
        if ! "${compiler}" "${source_file}" "${arguments[@]}" 2> /dev/null > /dev/null; then
            echo "expected '${source_file}' to build, but it did not"
            exit 1
        fi
    fi
}

ARG_SOURCE_FILE=""
ARG_COMPILER=""
ARG_INCLUDES=()
ARG_EXPECT_FAILURE="0"

while [[ $# -gt 0 ]]; do
  case $1 in
    --source-file)
      ARG_SOURCE_FILE="$2"
      shift
      shift
      ;;
    --expect-failure)
      ARG_EXPECT_FAILURE="1"
      shift
      ;;
    --meson-compiler-object)
      ARG_COMPILER=$(extract_compiler_path_from_meson_object "$2")
      shift
      shift
      ;;
    --include)
      ARG_INCLUDES+=("$2")
      shift
      shift
      ;;
    *)
      echo "Unexpected option: $1"
      exit 1
      ;;
  esac
done

compile_file "${ARG_SOURCE_FILE}" "${ARG_EXPECT_FAILURE}" "${ARG_COMPILER}" "${ARG_INCLUDES[@]}"