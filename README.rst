========
cauldron
========

.. image:: https://travis-ci.org/lycantropos/cauldron.svg?branch=master
  :target:  https://travis-ci.org/lycantropos/cauldron

.. image:: https://codecov.io/gh/lycantropos/cauldron/branch/master/graph/badge.svg
  :target: https://codecov.io/gh/lycantropos/cauldron

.. image:: https://readthedocs.org/projects/cauldron-cpp/badge/?version=latest
  :target: https://cauldron-cpp.readthedocs.io/en/latest

.. image:: https://img.shields.io/github/license/lycantropos/cauldron.svg
  :target: https://github.com/lycantropos/cauldron/blob/master/LICENSE

-----------
Downloading
-----------

Release
-------

There are no releases yet.

Developer
---------

Requirements:

- `git <https://git-scm.com/book/en/v2/Getting-Started-Installing-Git>`__.

Download the latest version from ``GitHub`` repository

.. code-block:: bash

  git clone https://github.com/lycantropos/cauldron.git
  cd cauldron

.. note:: Next instructions are executed from project's directory.

------------
Installation
------------

Requirements:

- `CMake 3.9+ <https://cmake.org/install/>`__,
- `Clang 3.4+ <http://releases.llvm.org/download.html>`__ or
  `GCC 5.0+ <https://gcc.gnu.org/install/>`__
  (on ``Linux`` systems ``Clang`` will require ``libstdc++5`` or newer).

Create **build** directory

.. code-block:: bash

  mkdir build
  cd build

.. note:: if **build** directory already exists re-create it after removing

  .. code-block:: bash

    rm -r build
    mkdir build
    cd build

Build and install

.. code-block:: bash

  cmake ..
  make
  make install

-----------
Development
-----------

Running tests
-------------

Plain
~~~~~

Requirements:

- `curl <https://curl.haxx.se/download.html>`__,
- `Catch 2.0.1+ single header <https://github.com/catchorg/Catch2/releases>`__,
- `CMake 3.9+ <https://cmake.org/install/>`__,
- `Clang 3.4+ <http://releases.llvm.org/download.html>`__ or
  `GCC 5.0+ <https://gcc.gnu.org/install/>`__
  (on ``Linux`` systems ``Clang`` will require ``libstdc++5`` or newer).

Download `Catch <https://github.com/catchorg/Catch2>`__ framework header
(may require ``sudo``)

.. code-block:: bash

  cd /usr/local/include
  curl -LJO https://github.com/catchorg/Catch2/releases/download/v${CATCH_VERSION}/catch.hpp
  cd -

where ``CATCH_VERSION`` is the ``Catch`` framework version (e.g. ``2.0.1``).

Create **build** directory

.. code-block:: bash

  mkdir build
  cd build

.. note:: if **build** directory already exists re-create it after removing

  .. code-block:: bash

    rm -r build
    mkdir build
    cd build

Build with tests

.. code-block:: bash

  mkdir -p build
  cd build
  cmake -DTESTS=ON ..
  make

Run tests

.. code-block:: bash

  ./main

``Docker``
~~~~~~~~~~

Requirements:

- `Docker CE 17.05.0+ <https://www.docker.com/community-edition/>`__ or
  `Docker EE 17.06.1+ <https://www.docker.com/enterprise-edition/>`__,
- `Docker Compose 1.13.0+ <https://docs.docker.com/compose/install/>`__.

Run tests in ``Docker`` containers

- directly with ``Docker Compose``

  .. code-block:: bash

    docker-compose up

- with ``Bash`` script (e.g. can be used in ``Git`` hooks)

  .. code-block:: bash

    ./run-tests.sh

Bumping version
---------------

Requirements:

- `Python 2.7+ <https://www.python.org/downloads/>`__,
- `bumpversion <https://github.com/peritus/bumpversion#installation>`__.

Choose which version number category to bump following `semver specification <http://semver.org/>`__.

Test bumping version

.. code-block:: bash

  bumpversion --dry-run --verbose $VERSION

where ``$VERSION`` is the target version number category name,
possible values are ``patch``/``minor``/``major``.

Bump version

.. code-block:: bash

  bumpversion --verbose $VERSION

.. note::
  to avoid inconsistency between branches and pull requests,
  bumping version should be merged into ``master`` branch as separate pull request.
