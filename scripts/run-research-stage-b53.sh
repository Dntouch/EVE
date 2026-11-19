#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b53}"

make -C "${root}" _build/eve_research_stage_b53
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b53" "${output}"

for name in loss-selection component-interventions combination-interventions \
    weight-interactions probe-sequences membrane-neuron-interventions \
    minimal-membrane-cases; do
    gzip -n -f "${output}/${name}.csv"
done

for name in loss-events transient-state-differences factorial-ablations \
    inhibitory-eligibility inhibitory-followup sensor-candidate-overlap \
    probe-controls; do
    gzip -n -f "${output}/b52-reproduction/${name}.csv"
done

for name in natural-trajectories eligibility-events natural-weight-changes \
    weight-transitions validated-repetitions ability-events \
    threshold-causal-copies; do
    gzip -n -f "${output}/b52-reproduction/b51-reproduction/${name}.csv"
done

for name in evaluation-trials event-chains eligibility-inventory \
    prospective-events weight-changes single-weight-ablations; do
    gzip -n -f "${output}/b52-reproduction/b51-reproduction/b50-reproduction/${name}.csv"
done
