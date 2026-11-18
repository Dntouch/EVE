#!/usr/bin/env sh
set -eu

destination="${1:-research/results/b46}"
make _build/eve_research_stage_b46
./_build/eve_research_stage_b46 "$destination"

# Detailed raw traces are immutable experiment artifacts.  Deterministic gzip
# keeps them below hosting limits while compact result tables remain browsable.
for file in trials.csv inhibitory-events.csv weight-changes.csv; do
    if [ -f "$destination/$file" ]; then
        gzip -n -9 -f "$destination/$file"
    fi
done
