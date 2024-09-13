# Build and run:
#   docker build -t clion/ubuntu/cpp-env:1.0 -f Dockerfile .

FROM ubuntu:latest

ARG USERNAME=vscode
ARG UID=1000
ARG GID=${UID}

RUN apt-get update \
    && apt-get install -y sudo \
    && groupadd --gid ${GID} ${USERNAME} \
    && useradd --uid ${UID} --gid ${GID} -m ${USERNAME} -s /bin/bash \
    && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/${USERNAME} \
    && chmod 0440 /etc/sudoers.d/${USERNAME}

# Install dependencies
RUN DEBIAN_FRONTEND="noninteractive" apt-get update \
    && apt-get -y upgrade \
    && apt-get -y install \
        build-essential \
        gcc \
        g++ \
        gdb \
        clang \
        make \
        ninja-build \
        cmake \
        autoconf \
        automake \
        libtool \
        valgrind \
        locales-all \
        dos2unix \
        rsync \
        tar \
        wget \
        bison \
        flex \
        libgmp3-dev \
        libmpc-dev \
        libmpfr-dev \
        texinfo \
        libisl-dev \
        grub-common \
        xorriso \
        mtools \
        bc \
        libncurses-dev \
        libelf-dev \
        libssl-dev \
        git \
        gawk \
        openssl \
        libudev-dev \
        libpci-dev \
        libiberty-dev \
        llvm \
        cpio \
        qemu-system \
        nasm \
    && apt-get clean

# Download source code
USER ${USERNAME}
ENV HOME=/home/${USERNAME}
RUN mkdir ~/src \
    && cd ~/src \
    && wget https://ftp.gnu.org/gnu/binutils/binutils-2.43.1.tar.gz \
    && tar -xvf binutils-2.43.1.tar.gz \
    && wget https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz \
    && tar -xvf gcc-14.2.0.tar.gz

# Set up environment variables \
ENV PATH="/opt/cross/bin:${PATH}"
ENV PREFIX="/opt/cross"
ENV TARGET="i686-elf"

# Compile binutils \
RUN cd ${HOME}/src \
    && mkdir build-binutils \
    && cd build-binutils \
    && ../binutils-2.43.1/configure --target="${TARGET}" --prefix="${PREFIX}" --with-sysroot --disable-nls --disable-werror \
    && make \
    && sudo make install

# Compile GCC \
RUN cd ${HOME}/src \
    && mkdir build-gcc \
    && cd build-gcc \
    && ../gcc-14.2.0/configure --target="${TARGET}" --prefix="${PREFIX}" --disable-nls --enable-languages=c,c++ --without-headers \
    && make all-gcc \
    && make all-target-libgcc \
    && sudo make install-gcc \
    && sudo make install-target-libgcc

CMD [ "/bin/bash" ]
