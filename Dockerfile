FROM ubuntu:latest

ENV DEBIAN_FRONTEND noninteractive

RUN apt update &&       \
    apt upgrade -y &&   \
    apt install clang nasm make qemu-system-i386 grub-common xorriso gcc -y