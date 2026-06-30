#!/usr/bin/env bash
# Architecture guardrail: enforce the dependency direction ui -> io -> model.
# The model layer is the pure, framework-free domain and must NOT depend on Qt,
# nor on the io or ui layers. Run via `cmake --build build --target check`.
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
status=0

model_violations=$(grep -rnE '#include[[:space:]]+(<Q|"src/(ui|io)/)' \
  "$root/src/model" || true)
if [ -n "$model_violations" ]; then
  echo "Layer violation: src/model must not depend on Qt, io, or ui:"
  echo "$model_violations"
  status=1
fi

# io may use model + Qt, but must not depend on the ui layer.
io_violations=$(grep -rnE '#include[[:space:]]+"src/ui/' "$root/src/io" || true)
if [ -n "$io_violations" ]; then
  echo "Layer violation: src/io must not depend on ui:"
  echo "$io_violations"
  status=1
fi

if [ "$status" -eq 0 ]; then
  echo "Layer check passed: model is framework-free; io does not depend on ui."
fi
exit "$status"
