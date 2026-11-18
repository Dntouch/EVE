#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b50}"

make -C "${root}" "${root}/_build/eve_research_stage_b50" 2>/dev/null || \
    make -C "${root}" _build/eve_research_stage_b50
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b50" "${output}"

for name in evaluation-trials event-chains eligibility-inventory prospective-events weight-changes single-weight-ablations; do
    gzip -n -f "${output}/${name}.csv"
done
