#!/usr/bin/env bash

# file: NxCoreApi003CategoryMessage/qb.sh - quick build

set -euo pipefail

rm -rf build && mkdir build
cmake -S . -B build -G Ninja -DCMAKE_VERBOSE_MAKEFILE=ON
cmake --build build --parallel

echo ""
echo "Build finished successfully."
echo ""
echo "To install system-wide (requires sudo):"
echo "  cd build && sudo cmake --install . && sudo ldconfig && cd .."
echo ""
echo "Or just run locally from build/apps/nxcore-cat-message/nxcore-cat-message"
echo ""
# Optional: clean old files only if you really want (careful!)
# sudo rm -f /usr/local/bin/nxcore-cat-message /usr/local/lib/libNxCoreApi003CategoryMessage.a