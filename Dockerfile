FROM ubuntu
MAINTAINER nicolov

RUN apt-get update
RUN apt-get -y install build-essential cmake git

RUN mkdir /home/git; \
    cd /home/git; \
    git clone https://github.com/nicolov/9f90a13c -b master; \
    cd 9f90a13c; \
    mkdir build; \
    cd build; \
    cmake ../; \
    make -j2
