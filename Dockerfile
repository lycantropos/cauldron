ARG CMAKE_VERSION

FROM lycantropos/cmake:${CMAKE_VERSION}

ARG CATCH2_VERSION
ENV CATCH2_VERSION=${CATCH2_VERSION}
RUN wget -P /usr/local/include https://github.com/catchorg/Catch2/releases/download/v${CATCH2_VERSION}/catch.hpp

WORKDIR /opt/cauldron

RUN apt-get update && \
    apt-get install -y lcov

COPY CMakeModules ./CMakeModules/
COPY CMakeLists.txt .
COPY cauldron/ ./cauldron/
COPY tests/ ./tests/

WORKDIR build

RUN cmake -DTESTS=ON .. && \
    make -j2 && \
    make install

CMD ["./main"]
