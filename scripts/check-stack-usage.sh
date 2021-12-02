#!/usr/bin/bash

SCRIPTS_DIRECTORY=$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)

function check_stack_usage() {
    local callgraph_file="$1"
    local function_name="$2"
    local min_stack_usage="$3"
    local max_stack_usage="$4"
    local stack_usage

    stack_usage=$("${SCRIPTS_DIRECTORY}/estimate-stack-usage.py" "${callgraph_file}" --function "${function_name}" --non-verbose)

    if [[ stack_usage -lt min_stack_usage ]]; then
        echo "Expected stack usage to be higher or equal to ${min_stack_usage}, got ${stack_usage}"
        exit 1
    fi

    if [[ stack_usage -gt max_stack_usage ]]; then
        echo "Expected stack usage to be lower or equal to ${max_stack_usage}, got ${stack_usage}"
        exit 1
    fi
}

ARG_CALLGRAPH_FILE=""
ARG_FUNCTION_NAME=""
ARG_EXPECTED_STACK_USAGE_MIN="0"
ARG_EXPECTED_STACK_USAGE_MAX="0"

while [[ $# -gt 0 ]]; do
    case $1 in
        --callgraph-file)
            ARG_CALLGRAPH_FILE="$2"
            shift
            shift
            ;;
        --function-name)
            ARG_FUNCTION_NAME="$2"
            shift
            shift
            ;;
        --min-stack-usage)
            ARG_EXPECTED_STACK_USAGE_MIN="$2"
            shift
            shift
            ;;
        --max-stack-usage)
            ARG_EXPECTED_STACK_USAGE_MAX="$2"
            shift
            shift
            ;;
        *)
            echo "Unexpected option: $1"
            exit 1
            ;;
    esac
done

check_stack_usage "${ARG_CALLGRAPH_FILE}" "${ARG_FUNCTION_NAME}" "${ARG_EXPECTED_STACK_USAGE_MIN}" "${ARG_EXPECTED_STACK_USAGE_MAX}"