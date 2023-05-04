FROM ubuntu

ENV TZ=Europe/Moscow

RUN apt-get clean && apt-get update -y 
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata git sudo build-essential autoconf libtool libssl-dev zlib1g-dev librdkafka-dev pkg-config cmake gcc git g++ mysql-client python3 pip libmysqlclient-dev libboost-all-dev libssl-dev && apt-get clean
RUN python3 -m pip install conan==1.55
COPY conan/default /root/.conan/profiles/default
COPY conan conan
RUN cd conan && conan install .

#RUN git clone https://github.com/QElderDelta/Highload-homework-MAI.git code
RUN mkdir code
COPY cpp code/cpp
RUN cd code/cpp && mkdir build && cd build && cmake ../ -DCMAKE_BUILD_TYPE=Release && cmake --build .

COPY run_service.sh code
RUN chmod +x code/run_service.sh

ENTRYPOINT [ "code/run_service.sh" ]