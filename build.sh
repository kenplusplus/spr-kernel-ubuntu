#!/bin/bash

CURR_DIR="$(readlink -f $(dirname "$0"))"
KERNEL_DIR="${CURR_DIR}/linux-kernel-dcp/"
DISTRO="${CURR_DIR}/jammy"

sudo apt install pahole curl libcap-dev default-jdk -y

if [[ ! -d ${KERNEL_DIR}/debian ]]; then
    ln -s ${DISTRO}/debian ${KERNEL_DIR}/debian
fi

if [[ ! -d ${KERNEL_DIR}/debian.master ]]; then
    ln -s ${DISTRO}/debian.master ${KERNEL_DIR}/debian.master
fi

if [[ ! -d ${KERNEL_DIR}/ubuntu ]]; then
    ln -s ${DISTRO}/ubuntu ${KERNEL_DIR}/ubuntu
fi

if [[ ! -f ${KERNEL_DIR}/scripts/ubuntu-retpoline-extract-one ]]; then
    ln -s ${DISTRO}/ubuntu-retpoline-extract-one ${KERNEL_DIR}/scripts/ubuntu-retpoline-extract-one
fi

cd ${KERNEL_DIR}
LANG=C fakeroot debian/rules binary-headers binary-generic binary-perarch
