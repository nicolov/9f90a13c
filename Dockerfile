FROM ubuntu
MAINTAINER nicolov

RUN apt-get update
RUN apt-get -y install build-essential cmake git

RUN mkdir -p /root/knightboard; \
    cd /root/knightboard; \
    git clone https://github.com/nicolov/9f90a13c -b master . ; \
    mkdir build; \
    cd build; \
    cmake ../; \
    make -j2

COPY knightboard.txt /root/knightboard.txt

CMD ["/root/knightboard/build/run_tests"]