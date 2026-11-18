#!/usr/bin/env sh
set -eu

destination="${1:-research/results/b43}"
make _build/eve_research_stage_b43
./_build/eve_research_stage_b43 "$destination"

