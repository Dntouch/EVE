#!/usr/bin/env sh
set -eu

make _build/eve_research_stage_b31
exec _build/eve_research_stage_b31 "${1:-research/results}"
