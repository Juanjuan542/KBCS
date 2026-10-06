# KBCS

C++ implementation of keyword-constrained community search on bipartite graphs.

## Implementation

The code contains four query methods:

- `Global`: global search.
- `Local`: local search.
- `GlobalBasedIndex`: global search using the index.
- `LocalBasedIndex`: local search using the index.

The implementation also provides index construction and query verification functions.

## Input

The program reads a dataset from a directory containing the following files:

- `graph.txt`: basic graph information.
- `edge.txt`: edges of the bipartite graph.
- `word.txt`: keyword information of vertices.
- `query.txt`: query cases.

A small test dataset corresponding to the example graph is included in the repository.

Each query in `query.txt` has the following format:

```text
alpha beta q type keyword1 keyword2 ...
