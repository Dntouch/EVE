#!/usr/bin/env bash
set -euo pipefail

repository_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd "$repository_root"

output_directory=${1:-research/results}
repetitions=${2:-512}
variant=${3:-}

make -j"$(getconf _NPROCESSORS_ONLN)" test _build/eve_experiment_002
if [[ -n "$variant" ]]; then
    _build/eve_experiment_002 "$output_directory" "$repetitions" "$variant"
else
    _build/eve_experiment_002 "$output_directory" "$repetitions"
fi
