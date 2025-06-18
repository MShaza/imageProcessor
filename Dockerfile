FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install base packages
RUN apt-get update && apt-get install -y \
    wget \
    g++ \
    make \
    libboost-all-dev \
    libssl-dev \
    libopencv-dev \
    git \
    curl \
    unzip \
    tar \
    ca-certificates

# Install CMake 3.28.3 using tar.gz (safe method)
RUN wget https://github.com/Kitware/CMake/releases/download/v3.30.4/cmake-3.30.4-linux-aarch64.tar.gz && \
    tar -xzf cmake-3.30.4-linux-aarch64.tar.gz && \
    mv cmake-3.30.4-linux-aarch64 /opt/cmake && \
    ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake && \
    rm cmake-3.30.4-linux-aarch64.tar.gz

# Confirm version
RUN cmake --version

WORKDIR /app
COPY . .

RUN rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make

EXPOSE 8080

CMD ["./build/image_service"]
