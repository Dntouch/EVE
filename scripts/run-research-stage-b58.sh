#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b58}"

make -C "${root}" _build/eve_tests _build/eve_research_stage_b58
"${root}/_build/eve_tests"
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b58" "${output}"

for name in network-classifications stable-experiences plasticity-events; do
    path="${output}/${name}.csv"
    if [[ -f "${path}" ]]; then
        gzip -n -f "${path}"
    fi
done
