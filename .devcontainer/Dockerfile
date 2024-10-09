FROM ubuntu:24.04

ARG CLANG_VERSION=18
ARG GCC_VERSION=14
ARG GCC_ARM_VERSION=13.3.rel1
ARG INCLUDE_WHAT_YOU_USE_VERSION=0.22

#
# Required tools
#

RUN apt-get update && \
	apt-get install -y --no-install-recommends \
		ca-certificates \
		git \
		gnupg2 \
		python3-minimal \
		python3-pip \
		wget \
		xz-utils


#
# Install arm-gcc toolchain
#

RUN mkdir /opt/gcc-arm-none-eabi \
 && wget -qO - "https://developer.arm.com/-/media/Files/downloads/gnu/${GCC_ARM_VERSION}/binrel/arm-gnu-toolchain-${GCC_ARM_VERSION}-$(uname -m)-arm-none-eabi.tar.xz" | tar --strip-components=1 -xJC /opt/gcc-arm-none-eabi
ENV PATH="$PATH:/opt/gcc-arm-none-eabi/bin"

#
# Toolchain
#

RUN wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor -o /usr/share/keyrings/llvm-snapshot-keyring.gpg && \
	apt-get update && \
	apt-get install -y --no-install-recommends \
		# Clang
		clang-${CLANG_VERSION} \
		clang-format-${CLANG_VERSION} \
		clang-tidy-${CLANG_VERSION} \
		clangd-${CLANG_VERSION} \
		lld-${CLANG_VERSION} \
		lldb-${CLANG_VERSION} \
		# CMake
		cmake \
		ninja-build \
		make \
		# GCC
		gcc-${GCC_VERSION} \
		libstdc++-14-dev \
		# Code analysis
		cppcheck \
		gcovr \
		# IWYU
		libclang-${CLANG_VERSION}-dev \
		llvm-${CLANG_VERSION}-dev && \
	pip install --break-system-packages cpplint
ENV PATH="$PATH:/usr/lib/llvm-${CLANG_VERSION}/bin/"

#
# Include-what-you-use
#

RUN apt-get update && apt-get install -y --no-install-recommends libclang-${CLANG_VERSION}-dev llvm-${CLANG_VERSION}-dev libsqlite3-dev && \
	wget -qO - https://github.com/include-what-you-use/include-what-you-use/archive/refs/tags/${INCLUDE_WHAT_YOU_USE_VERSION}.tar.gz | tar xz -C /tmp && \
	CC=clang CXX=clang++ cmake -S /tmp/include-what-you-use-${INCLUDE_WHAT_YOU_USE_VERSION} -B /tmp/include-what-you-use-${INCLUDE_WHAT_YOU_USE_VERSION}/build && \
	cmake --build /tmp/include-what-you-use-${INCLUDE_WHAT_YOU_USE_VERSION}/build --target install

RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-13 20 && \
	update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-13 20 && \
	update-alternatives --install /usr/bin/python python /usr/bin/python3 10
