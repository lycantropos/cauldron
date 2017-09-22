FROM lycantropos/cmake:3.9.1

RUN apt-get update && \
    apt-get install -y python-dev \
                       python-pip \
                       python-setuptools

RUN python -m pip install conan

WORKDIR /opt/cauldron

COPY conanfile.txt .

RUN mkdir build && \
    cd build && \
    conan install ..

COPY CMakeLists.txt .
COPY cauldron/ ./cauldron/
COPY tests/ ./tests/

RUN cmake -G "CodeBlocks - Unix Makefiles" . && \
    cmake --build . --target all -- -j 2

CMD ["./bin/main"]
