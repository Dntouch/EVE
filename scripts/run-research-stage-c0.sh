#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output_dir="${root_dir}/research/results/c0"

make -C "${root_dir}" _build/eve_research_stage_c0 _build/eve_tests
"${root_dir}/_build/eve_tests"
rm -rf "${output_dir}"
mkdir -p "${output_dir}"
"${root_dir}/_build/eve_research_stage_c0" "${output_dir}"

for file in autonomous-trials.csv natural-growth-connections.csv \
    recruitment-stages.csv task-learning-curves.csv control-groups.csv \
    causal-interventions.csv; do
    gzip -n -9 "${output_dir}/${file}"
done
