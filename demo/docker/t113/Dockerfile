FROM debian:buster

# 修改为TUNA-ELTS镜像源
COPY common/other/distro/buster/elts-archive-key.gpg /etc/apt/trusted.gpg.d/freexian-archive-extended-lts.gpg
COPY common/other/distro/buster/sources.list /etc/apt/sources.list

# 安装基础工具和HTTPS支持
RUN apt update && apt install -y apt-transport-https ca-certificates

# 安装常用工具
RUN sed -i 's/http/https/g' /etc/apt/sources.list
RUN apt update && apt upgrade -y && apt install --install-recommends -y \
    openssh-server bash-completion git bear curl wget bsdmainutils \
    locales shellcheck netcat-openbsd vim file pv clangd-16
RUN ln -sf /usr/bin/clangd-16 /usr/bin/clangd

# 安装开发工具链和SSH
RUN apt install --install-recommends -y \
    xz-utils netpbm make gcc g++ flex bison libncurses-dev libssl-dev \
    bzip2 cpio unzip rsync lzop kmod fakeroot automake \
    libtool device-tree-compiler bc lsof

RUN dpkg --add-architecture i386 && apt update && \
    apt install --install-recommends -y \
    busybox libc6:i386 libgcc1:i386 libstdc++6:i386

RUN ln -sf /usr/bin/python3 /usr/bin/python
RUN rm -rf /var/lib/apt/lists/*
COPY t113/repo /usr/bin/repo
COPY t113/gen_compile_commands.py /usr/bin/gen_compile_commands.py

# 配置locale
RUN echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen && \
    echo "zh_CN.UTF-8 UTF-8" >> /etc/locale.gen && \
    locale-gen

# 配置vim
RUN sed -i 's/^\(\s*\)set mouse=a$/\1set mouse=/' /usr/share/vim/vim81/defaults.vim
COPY common/vim/yank /usr/bin/osc52_yank
COPY common/vim/vimrc.local /etc/vim/vimrc.local

# 配置 SSH
RUN mkdir -p /var/run/sshd && \
    mkdir -p /root/.ssh && \
    chmod 700 /root/.ssh
COPY common/ssh/authorized_keys /root/.ssh/authorized_keys
COPY common/ssh/config /root/.ssh/config
RUN sed -i 's/#PermitTTY yes/PermitTTY yes/' /etc/ssh/sshd_config
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin without-password/' /etc/ssh/sshd_config
RUN sed -i 's/#PasswordAuthentication yes/PasswordAuthentication no/' /etc/ssh/sshd_config
RUN sed -i 's/#PubkeyAuthentication yes/PubkeyAuthentication yes/' /etc/ssh/sshd_config

# 配置 GIT
RUN mkdir -p /root/.config/git
COPY common/git/config /root/.config/git/config
COPY common/other/distro/buster/lazygit /usr/local/bin

# 配置 BASH
COPY common/bash/bashrc /root/.bashrc
COPY common/bash/profile /root/.profile
COPY common/bash/bash_aliases /root/.bash_aliases
RUN echo "alias cw='cd /workspace/t113'" > /root/.bash_work
RUN echo "export TAR_OPTIONS='--no-same-owner'" >> /root/.bash_work
RUN sed -i '/#if ! shopt -oq posix;/,/#fi/ s/^\s*#//g' /etc/bash.bashrc
RUN echo "export REPO_URL='https://mirrors.bfsu.edu.cn/git/git-repo'" >> /root/.bash_work

# 创建 BASH 相关目录结构
RUN mkdir -p /root/.cache

# 创建工作目录
RUN mkdir -p /workspace/t113
WORKDIR /workspace/t113

# 使能 vscode remote 功能在旧的系统上
COPY common/other/distro/buster/vsc_helper.tgz /opt/vsc_helper.tgz
RUN tar --no-same-owner -xaf /opt/vsc_helper.tgz -C /opt && rm /opt/vsc_helper.tgz
RUN echo "VSCODE_SERVER_CUSTOM_GLIBC_LINKER=/opt/vsc_helper/lib/ld-2.28.so" >> /etc/environment && \
    echo "VSCODE_SERVER_PATCHELF_PATH=/opt/vsc_helper/bin/patchelf" >> /etc/environment && \
    echo "VSCODE_SERVER_CUSTOM_GLIBC_PATH=/opt/vsc_helper/lib" >> /etc/environment

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]
