FROM ubuntu:24.04

ARG CATCH2_VERSION=3.7.1
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
		xz-utils && \
	apt-get autoremove -y && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*

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
		libclang-rt-${CLANG_VERSION}-dev \
		lld-${CLANG_VERSION} \
		lldb-${CLANG_VERSION} \
		llvm-${CLANG_VERSION} \
		# CMake
		cmake \
		ninja-build \
		make \
		# GCC
		g++-${GCC_VERSION} \
		# Code analysis
		cppcheck \
		gcovr && \
	pip install --break-system-packages cpplint pre-commit && \
	# pre-commit install --hook-type commit-msg && \
	apt-get autoremove -y && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*
ENV PATH="$PATH:/usr/lib/llvm-${CLANG_VERSION}/bin/"

#
# Include-what-you-use
#

RUN apt-get update && apt-get install -y --no-install-recommends libclang-${CLANG_VERSION}-dev llvm-${CLANG_VERSION}-dev libsqlite3-dev && \
	wget -qO - https://github.com/include-what-you-use/include-what-you-use/archive/refs/tags/${INCLUDE_WHAT_YOU_USE_VERSION}.tar.gz | tar xz -C /tmp && \
	CC=clang CXX=clang++ cmake -S /tmp/include-what-you-use-${INCLUDE_WHAT_YOU_USE_VERSION} -B /tmp/include-what-you-use-${INCLUDE_WHAT_YOU_USE_VERSION}/build && \
	cmake --build /tmp/include-what-you-use-${INCLUDE_WHAT_YOU_USE_VERSION}/build --target install && \
	apt-get purge -y libclang-${CLANG_VERSION}-dev llvm-${CLANG_VERSION}-dev libsqlite3-dev gnupg2 && \
	apt-get autoremove -y && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*

#
# Catch2
#

RUN wget -qO - https://github.com/catchorg/Catch2/archive/refs/tags/v${CATCH2_VERSION}.tar.gz | tar xz -C /tmp && \
	cmake -S /tmp/Catch2-${CATCH2_VERSION} -B /tmp/Catch2-${CATCH2_VERSION}/build && \
	cmake --build /tmp/Catch2-${CATCH2_VERSION}/build --target install

#
# Setup environment
#

RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-${GCC_VERSION} 20 && \
	update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-${GCC_VERSION} 20 && \
	update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-${GCC_VERSION} 20 && \
	update-alternatives --install /usr/bin/python python /usr/bin/python3 10
