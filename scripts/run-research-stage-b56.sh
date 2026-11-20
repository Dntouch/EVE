#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b56}"

make -C "${root}" _build/eve_research_stage_b56
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b56" "${output}"

compress_csv() {
    local path="$1"
    if [[ -f "${path}" ]]; then
        gzip -n -f "${path}"
    fi
}

for name in learning-biographies checkpoint-evaluations stability-checkpoints \
    causal-weight-interventions causal-transient-interventions \
    factorial-interventions paired-continuations timing-seed-results \
    retention-reversal; do
    compress_csv "${output}/${name}.csv"
done
