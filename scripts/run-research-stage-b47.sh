#!/usr/bin/env sh
set -eu

destination="${1:-research/results/b47}"
make _build/eve_research_stage_b47
./_build/eve_research_stage_b47 "$destination"

# Episode- and event-level diagnostics are reproducible but comparatively
# large.  Deterministic gzip preserves exact content without noisy Git blobs.
for file in learning-trajectories.csv event-traces.csv weight-changes.csv \
    evaluation-trials.csv; do
    if [ -f "$destination/$file" ]; then
        gzip -n -9 -f "$destination/$file"
    fi
done
