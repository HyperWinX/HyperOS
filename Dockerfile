FROM ubuntu:latest

ENV DEBIAN_FRONTEND noninteractive

RUN apt update &&       \
    apt upgrade -y &&   \
    apt install clang nasm make qemu-system-x86 xorriso gcc git -y && \
    git clone https://github.com/limine-bootloader/limine.git --branch=v7.x-binary --depth=1 && \
    make -C limine