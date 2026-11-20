#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b55}"

make -C "${root}" _build/eve_research_stage_b55
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b55" "${output}"

compress_csv() {
    local path="$1"
    if [[ -f "${path}" ]]; then
        gzip -n -f "${path}"
    fi
}

for name in natural-trajectories subthreshold-relaxation-events \
    stability-checkpoints confusion-matrices subthreshold-inventory \
    subthreshold-inputs subthreshold-causal-copies \
    subthreshold-state-effects; do
    compress_csv "${output}/${name}.csv"
done

reference="${output}/b54-reproduction"
for name in natural-trajectories recovery-events causal-copies \
    temporal-integration; do
    compress_csv "${reference}/${name}.csv"
done

for name in loss-selection component-interventions combination-interventions \
    weight-interactions probe-sequences membrane-neuron-interventions \
    minimal-membrane-cases; do
    compress_csv "${reference}/b53-reproduction/${name}.csv"
done

for name in loss-events transient-state-differences factorial-ablations \
    inhibitory-eligibility inhibitory-followup sensor-candidate-overlap \
    probe-controls; do
    compress_csv "${reference}/b53-reproduction/b52-reproduction/${name}.csv"
done

for name in natural-trajectories eligibility-events natural-weight-changes \
    weight-transitions validated-repetitions ability-events \
    threshold-causal-copies; do
    compress_csv "${reference}/b53-reproduction/b52-reproduction/b51-reproduction/${name}.csv"
done

for name in evaluation-trials event-chains eligibility-inventory \
    prospective-events weight-changes single-weight-ablations; do
    compress_csv "${reference}/b53-reproduction/b52-reproduction/b51-reproduction/b50-reproduction/${name}.csv"
done
