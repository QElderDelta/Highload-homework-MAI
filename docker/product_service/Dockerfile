FROM ubuntu

ENV TZ=Europe/Moscow

RUN apt-get clean && apt-get update -y 
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata git sudo build-essential autoconf libtool libssl-dev zlib1g-dev librdkafka-dev pkg-config cmake gcc git g++ mysql-client python3 pip libmysqlclient-dev libboost-all-dev libssl-dev && apt-get clean
RUN echo 1
RUN python3 -m pip install conan==1.55

RUN echo 1
COPY cpp cpp
RUN cd cpp && mkdir build && cd build && cmake ../ -DCMAKE_BUILD_TYPE=Release && cmake --build .

ENTRYPOINT [ "cpp/build/product_service/product_service" ]