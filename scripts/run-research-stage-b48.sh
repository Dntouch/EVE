#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b48}"

make -C "${root}" "${root}/_build/eve_research_stage_b48" 2>/dev/null || \
    make -C "${root}" _build/eve_research_stage_b48
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b48" "${output}"

for name in training-episodes negative-events evaluation-trials; do
    gzip -n -f "${output}/${name}.csv"
done
