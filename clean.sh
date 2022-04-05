#!/bin/bash

CURR_DIR="$(readlink -f $(dirname "$0"))"
KERNEL_DIR="${CURR_DIR}/linux-kernel-dcp/"
DISTRO="${CURR_DIR}/jammy"

rm *.deb

cd ${KERNEL_DIR}
LANG=C fakeroot debian/rules binary-headers binary-generic binary-perarch