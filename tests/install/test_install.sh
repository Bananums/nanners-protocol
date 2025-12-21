#!/usr/bin/env sh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/../.." && pwd)"
CONSUMER_SRC="${ROOT_DIR}/tests/install/consumer"

BUILD_DIR="$(mktemp -d)"
INSTALL_DIR="$(mktemp -d)"
CONSUMER_BUILD_DIR="$(mktemp -d)"

echo "Build dir:          ${BUILD_DIR}"
echo "Install dir:        ${INSTALL_DIR}"
echo "Consumer build dir: ${CONSUMER_BUILD_DIR}"

# 1) Configure + build library (+ tests if you want)
cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=ON \
  -DBUILD_TESTING=ON
cmake --build "${BUILD_DIR}"

# 2) Install into a clean prefix
cmake --install "${BUILD_DIR}" --prefix "${INSTALL_DIR}"

# 3) Build consumer against the installed package
cmake -S "${CONSUMER_SRC}" -B "${CONSUMER_BUILD_DIR}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="${INSTALL_DIR}" \
  -DCMAKE_FIND_USE_PACKAGE_REGISTRY=OFF \
  -DCMAKE_FIND_USE_SYSTEM_PACKAGE_REGISTRY=OFF
cmake --build "${CONSUMER_BUILD_DIR}"

# 4) Run consumer using installed shared library
LIBDIR="${INSTALL_DIR}/lib"
[ -d "${INSTALL_DIR}/lib64" ] && LIBDIR="${INSTALL_DIR}/lib64"

LD_LIBRARY_PATH="${LIBDIR}" "${CONSUMER_BUILD_DIR}/consumer"
