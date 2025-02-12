FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu-20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    build-essential \
    git \
    gcc \
    cmake \
    gdb \
    && rm -rf /var/lib/apt/lists/*
