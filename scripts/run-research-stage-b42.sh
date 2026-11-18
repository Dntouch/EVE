#!/usr/bin/env sh
set -eu

destination="${1:-research/results/b42}"
make _build/eve_research_stage_b42
./_build/eve_research_stage_b42 "$destination"
