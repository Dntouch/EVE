#!/usr/bin/env bash
set -euo pipefail

repository_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd "$repository_root"

output_directory=${1:-research/results}

make -j"$(getconf _NPROCESSORS_ONLN)" test _build/eve_research_stage_b1
_build/eve_research_stage_b1 "$output_directory"
