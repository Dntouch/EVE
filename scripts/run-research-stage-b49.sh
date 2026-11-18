#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b49}"

make -C "${root}" "${root}/_build/eve_research_stage_b49" 2>/dev/null || \
    make -C "${root}" _build/eve_research_stage_b49
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b49" "${output}"

for name in evaluation-trials event-chains; do
    gzip -n -f "${output}/${name}.csv"
done
