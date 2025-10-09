#!/bin/sh

# ref-url: https://code.visualstudio.com/docs/remote/faq
# section: can-i-run-vs-code-server-on-older-linux-distributions
# ref-url: https://github.com/naitaku/old-linux-vscode/blob/main/Dockerfile

os_version=$(grep VERSION_CODENAME /etc/os-release | cut -d= -f2)

if [ "$(id -u)" != 0 ]; then
    echo "This script must be run as root"
    exit 1
fi

pwd=$(pwd)
echo "Current directory: $pwd"
vsc_helper_dir="/opt/vsc_helper"
patchelf_dir="$vsc_helper_dir/bin"
lib_dir="$vsc_helper_dir/lib"

export CT_ALLOW_BUILD_AS_ROOT=y
export CT_ALLOW_BUILD_AS_ROOT_SURE=y

update() {
    echo ""
    echo "====================================================="
    echo "           update and install depend-tools           "
    echo "====================================================="

    apt-get update && apt-get install -y \
        gcc g++ gperf bison flex texinfo help2man make libncurses5-dev \
        python3-dev autoconf automake libtool libtool-bin gawk wget bzip2 \
        xz-utils unzip patch rsync meson ninja-build
}

crosstool_ng() {
    echo ""
    echo "====================================================="
    echo "           download and build crosstool-ng           "
    echo "====================================================="

    # shellcheck disable=SC3043
    local url_prefix="http://crosstool-ng.org/download/crosstool-ng"
    # shellcheck disable=SC3043
    local file="crosstool-ng-1.26.0.tar.bz2"
    # shellcheck disable=SC3043
    local out_dir="$pwd/crosstool-ng-1.26.0/out"

    if [ ! -f $file ]; then
        wget $url_prefix/$file -O $file && tar -xaf $file
    fi
    if [ ! -x "$out_dir"/bin/ct-ng ]; then
        cd "$out_dir"/.. && ./configure --prefix="$out_dir" && \
            make && make install && cd "$pwd" || exit
    fi
    export PATH="$PATH":"${out_dir}"/bin
}

build_sysroot() {
    echo ""
    echo "====================================================="
    echo "            build vscode runtimes sysroot            "
    echo "====================================================="

    # shellcheck disable=SC3043
    local url_prefix1="https://raw.githubusercontent.com/"
    # shellcheck disable=SC3043
    local url_prefix2="microsoft/vscode-linux-build-agent/refs/heads/main/"
    # shellcheck disable=SC3043
    if [ "$os_version" = "xenial" ] || [ "$os_version" = "stretch" ]; then
        local file="x86_64-gcc-8.5.0-glibc-2.28.config"
    else
        local file="x86_64-gcc-10.5.0-glibc-2.28.config"
    fi

    # shellcheck disable=SC3043
    local toolchain_dir="$pwd/toolchain"
    mkdir -p "$toolchain_dir"/downloads
    wget ${url_prefix1}${url_prefix2}${file} -O "$toolchain_dir"/.config

    # shellcheck disable=SC3043
    local out_dir="$toolchain_dir"/x86_64-linux-gnu/x86_64-linux-gnu/sysroot
    if [ ! -d "$out_dir" ]; then
        cd "$toolchain_dir" && ct-ng build && cd "$pwd" || exit
    fi

    url_prefix1="https://github.com/NixOS/patchelf/releases/download/0.18.0/"
    file="patchelf-0.18.0-x86_64.tar.gz"
    if [ ! -f "$toolchain_dir"/downloads/${file} ];  then
        wget ${url_prefix1}${file} -O "$toolchain_dir"/downloads/${file}
    fi
    tar -xaf "$toolchain_dir"/downloads/${file} -C "$out_dir"
    rm -rf $vsc_helper_dir && mv "$out_dir" "$vsc_helper_dir"
}

config_env() {
    echo ""
    echo "====================================================="
    echo "          config vscode runtime environment          "
    echo "====================================================="
    {
        echo "VSCODE_SERVER_PATCHELF_PATH=$patchelf_dir/patchelf"
        echo "VSCODE_SERVER_CUSTOM_GLIBC_LINKER=$lib_dir/ld-2.28.so"
        echo "VSCODE_SERVER_CUSTOM_GLIBC_PATH=$lib_dir"
    } >> /etc/environment
}

update
crosstool_ng
build_sysroot
config_env
