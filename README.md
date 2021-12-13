NMSSearch
==========

A tool for fast similarity search of genomic sequences, using [NMSLIB](https://github.com/nmslib/nmslib).

Installation
------------

Note: this repository uses Git submodules. After cloning the repository, you must initialize the submodules via
```console
    git submodule init
    git submodule update
```

The application uses a `cmake`-based set of build scripts and will automatically build and link external dependencies. Assuming that you have `cmake` installed, as well as any libraries required to build Parasail and NMSLIB, you can just run

```console
    mkdir build
    cd build
    cmake ..
    make -j4
```

When the build finishes, you should have an executable `nmssearch` in the build directory. You can move the application to wherever it is needed. It is statically linked against the project dependencies, and only requires ZLib and the C++ standard library to run.

How to use
----------

NMSSearch supports two algorithms to index sequences, using vantage-point search trees (`--algorithm vptree`), and using hierarchical navigable small world graphs (`--algorithm hnsw`). Generally, HNSW takes much longer to build the index, but is faster at query time.

To build an index for a FASTA file of target sequences:
```console
nmssearch --algorithm hnsw build <my-fasta-file.fa>
```

To query the index, given a FASTA of sequences to look up:
```console
nmssearch --algorithm hnsw query hnsw <my-queries.fa>
```

The name of the generated index is currently hardcoded as `hnsw` or `vptree`.

License
-------

NMSSearch is released under the GNU Public License (GPL), v3.