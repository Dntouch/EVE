#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b57}"

make -C "${root}" _build/eve_research_stage_b57
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b57" "${output}"

for name in local-plasticity-events modulation-counterfactuals loss-trajectories \
    natural-continuations; do
    path="${output}/${name}.csv"
    if [[ -f "${path}" ]]; then
        gzip -n -f "${path}"
    fi
done
