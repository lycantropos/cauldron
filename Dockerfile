ARG CMAKE_VERSION

FROM lycantropos/cmake:${CMAKE_VERSION}

ARG CATCH_VERSION
ENV CATCH_VERSION=${CATCH_VERSION}
RUN cd /usr/local/include && \
    curl -LJO https://github.com/catchorg/Catch2/releases/download/v${CATCH_VERSION}/catch.hpp

RUN apt-get update && \
    apt-get install -y lcov

WORKDIR /opt/cauldron

COPY CMakeModules ./CMakeModules/
COPY CMakeLists.txt .
COPY cauldron/ ./cauldron/
COPY tests/ ./tests/
COPY run-plain-tests.sh ./run-plain-tests.sh

WORKDIR build

RUN cmake -DTESTS=ON .. && \
    make -j$(nproc --all) && \
    make install

ENTRYPOINT ["/opt/cauldron/run-plain-tests.sh"]
