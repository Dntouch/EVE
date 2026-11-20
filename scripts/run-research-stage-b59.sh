#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b59}"

make -C "${root}" _build/eve_tests _build/eve_research_stage_b59
"${root}/_build/eve_tests"
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b59" "${output}"

for name in network-classifications stable-experiences plasticity-events \
        integration-events nonfire-decisions eligibility-integration; do
    path="${output}/${name}.csv"
    if [[ -f "${path}" ]]; then
        gzip -n -f "${path}"
    fi
done
