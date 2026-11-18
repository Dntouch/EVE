#!/usr/bin/env bash
set -euo pipefail

repository_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd "$repository_root"

output_directory=${1:-research/results}
variant=${2:-}

make -j"$(getconf _NPROCESSORS_ONLN)" test \
    _build/eve_experiment_003 \
    _build/eve_experiment_003_benchmark

if [[ -n "$variant" ]]; then
    _build/eve_experiment_003 "$output_directory" "$variant"
else
    _build/eve_experiment_003 "$output_directory"
    _build/eve_experiment_003_benchmark \
        "$output_directory/experiment-003-performance.csv"
    awk -f scripts/summarize_experiment_003.awk \
        "$output_directory/experiment-003-training.csv" \
        > "$output_directory/experiment-003-summary.csv"
    awk -f scripts/summarize_experiment_003_recognition.awk \
        "$output_directory/experiment-003-recognition.csv" \
        > "$output_directory/experiment-003-recognition-summary.csv"
fi
