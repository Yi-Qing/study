FROM debian:bookworm

# 修改为BFSU镜像源
COPY common/other/distro/bookworm/debian.sources /etc/apt/sources.list.d/debian.sources

# 安装基础工具和HTTPS支持
RUN apt update && apt install -y apt-transport-https ca-certificates

# 安装常用工具
RUN sed -i 's/http/https/g' /etc/apt/sources.list.d/debian.sources
RUN apt update && apt upgrade -y && apt install --install-recommends -y \
    openssh-server bash-completion git bear curl wget bsdmainutils \
    locales shellcheck netcat-openbsd vim file pv clangd-19
RUN ln -sf /usr/bin/clangd-19 /usr/bin/clangd

# 安装开发工具链和SSH
RUN apt install --install-recommends -y \
    xz-utils netpbm make gcc g++ flex bison libncurses-dev libssl-dev \
    bzip2 cpio unzip rsync lzop kmod fakeroot automake \
    libtool device-tree-compiler bc lsof repo

RUN apt install --install-recommends -y \
    lz4 libgmp-dev libmpc-dev expect expect-dev scons

RUN ln -sf /usr/bin/python3 /usr/bin/python
RUN rm -rf /var/lib/apt/lists/*

# 安装编译依赖的 python2 tar 包
COPY common/other/distro/bookworm/python2.tgz /tmp/python2.tgz
RUN tar -xzf /tmp/python2.tgz -C /usr/local/ && \
    rm -rf /tmp/python2.tgz

# 配置vim
RUN sed -i 's/^\(\s*\)set mouse=a$/\1set mouse=/' /usr/share/vim/vim90/defaults.vim
COPY common/vim/yank /usr/bin/osc52_yank
COPY common/vim/vimrc.local /etc/vim/vimrc.local

# 配置locale
RUN echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen && \
    echo "zh_CN.UTF-8 UTF-8" >> /etc/locale.gen && \
    locale-gen

# 配置SSH
RUN mkdir -p /var/run/sshd && \
    mkdir -p /root/.ssh && \
    chmod 700 /root/.ssh
COPY common/ssh/authorized_keys /root/.ssh/authorized_keys
COPY common/ssh/config /root/.ssh/config
# 配置SSH（强制密钥登录）
RUN sed -i 's/#PermitTTY yes/PermitTTY yes/' /etc/ssh/sshd_config
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin without-password/' /etc/ssh/sshd_config && \
    sed -i 's/#PasswordAuthentication yes/PasswordAuthentication no/' /etc/ssh/sshd_config && \
    sed -i 's/#PubkeyAuthentication yes/PubkeyAuthentication yes/' /etc/ssh/sshd_config

# 配置GIT
RUN mkdir -p /root/.config/git
COPY common/git/config /root/.config/git/config

# 配置BASH
COPY common/bash/bashrc /root/.bashrc
COPY common/bash/profile /root/.profile
COPY common/bash/bash_aliases /root/.bash_aliases
RUN echo "alias cw='cd /workspace/rk3562'" > /root/.bash_work
RUN echo "export TAR_OPTIONS='--no-same-owner'" >> /root/.bash_work
RUN sed -i '/#if ! shopt -oq posix;/,/#fi/ s/^\s*#//g' /etc/bash.bashrc

# 创建BASH相关目录结构
RUN mkdir -p /root/.cache

# 创建工作目录
RUN mkdir -p /workspace/rk3562
WORKDIR /workspace/rk3562

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]
