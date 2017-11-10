ARG CMAKE_VERSION

FROM lycantropos/cmake:${CMAKE_VERSION}

ARG CATCH2_VERSION
ENV CATCH2_VERSION=${CATCH2_VERSION}
RUN cd /usr/local/include && \
    curl -LJO https://github.com/catchorg/Catch2/releases/download/v${CATCH2_VERSION}/catch.hpp

RUN apt-get update && \
    apt-get install -y lcov

WORKDIR /opt/cauldron

COPY CMakeModules ./CMakeModules/
COPY CMakeLists.txt .
COPY cauldron/ ./cauldron/
COPY tests/ ./tests/

WORKDIR build

RUN cmake -DTESTS=ON .. && \
    make -j2 && \
    make install

CMD ["./main"]
