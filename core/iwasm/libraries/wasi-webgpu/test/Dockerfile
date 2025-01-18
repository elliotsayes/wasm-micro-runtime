# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

FROM ubuntu:22.04 AS base

ARG WASM_TARGET="wasm32-unknown-unknown"

ENV DEBIAN_FRONTEND=noninteractive

# hadolint ignore=DL3008,DL3009
RUN apt-get update \
  && apt-get install -y --no-install-recommends \
       ca-certificates cmake build-essential git wget curl \
       libvulkan-dev vulkan-tools mesa-vulkan-drivers \
       pkg-config python3-pip file

WORKDIR /usr/local/share/ca-certificates/cacert.org
RUN wget -qP /usr/local/share/ca-certificates/cacert.org http://www.cacert.org/certs/root.crt http://www.cacert.org/certs/class3.crt \
  && update-ca-certificates

# Install Rust toolchain
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"
ENV RUSTUP_HOME=/root/.rustup
ENV CARGO_HOME=/root/.cargo

RUN rustup target add ${WASM_TARGET}

WORKDIR /home/wamr
COPY . .
RUN git config --global http.sslCAinfo /etc/ssl/certs/ca-certificates.crt

WORKDIR /home/wamr/product-mini/platforms/linux
RUN rm -rf build \
  && cmake -S . -B build \
       -DWAMR_BUILD_WASI_WEBGPU=1 \
  && cmake --build build -j "$(grep -c ^processor /proc/cpuinfo)"

WORKDIR /home/wamr/core/iwasm/libraries/wasi-webgpu/test/compute
RUN cargo build --target ${WASM_TARGET}

# WORKDIR /home/wamr/core/iwasm/libraries/wasi-webgpu/test/triangle
# RUN cargo build --target ${WASM_TARGET}

FROM ubuntu:22.04

ARG WASM_TARGET="wasm32-unknown-unknown"

RUN apt-get update \
  && apt-get install -y --no-install-recommends \
       libvulkan1 mesa-vulkan-drivers \
       ca-certificates \
       coreutils \
       file \
  && rm -rf /var/lib/apt/lists/*

COPY --from=base /home/wamr/product-mini/platforms/linux/build/iwasm /usr/bin/
COPY --from=base /home/wamr/product-mini/platforms/linux/build/lib*.a /usr/lib/
COPY --from=base /home/wamr/core/iwasm/libraries/wasi-webgpu/test/compute/target/${WASM_TARGET}/debug/compute.wasm /app/compute.wasm
# COPY --from=base /home/wamr/core/iwasm/libraries/wasi-webgpu/test/triangle/target/${WASM_TARGET}/debug/triangle.wasm /app/triangle.wasm

ENV LD_LIBRARY_PATH=/usr/lib

WORKDIR /app
