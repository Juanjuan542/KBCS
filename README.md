# KBCS

This repository contains the source code for the paper:

**Keyword-constrained Community Search Over Bipartite Networks**

## Overview

Bipartite graphs are widely used to model interactions between two types of entities, such as users and items, authors and papers, and users and pages.

This project studies **Keyword-constrained Bi-Community Search (KBCS)** over unweighted bipartite graphs. Given a query vertex, structural parameters `(α, β)`, and a set of query keywords, KBCS aims to find a connected community that:

- contains the query vertex;
- satisfies the `(α, β)-core` degree constraints;
- satisfies the given keyword constraints; and
- is maximal under these constraints.

To efficiently process KBCS queries, we implement two baseline algorithms and a Hybrid Index (HI)-based framework.

## Methods

The repository includes the following methods:

### Global

The global algorithm first filters vertices according to the query keywords, computes the corresponding `(α, β)-core`, and then extracts the connected component containing the query vertex.

### Local

The local algorithm starts from the query vertex and incrementally explores its neighborhood while maintaining the structural and keyword constraints.

### Hybrid Index (HI)

To reduce the online query cost, we propose a Hybrid Index that combines:

- **Keyword Inverted Index** for retrieving vertices satisfying keyword constraints;
- **Bi-core Offset Labels** for pruning vertices that cannot satisfy the given `(α, β)` structural constraints.

Based on HI, we implement two accelerated query algorithms:

- **HI+Global**
- **HI+Local**

## Datasets

The experiments use four real-world bipartite graph datasets:

| Dataset | Type | \|U\| | \|V\| | \|E\| |
|---|---|---:|---:|---:|
| DBLP | Authorship | 315,950 | 168,511 | 542,405 |
| MovieLens | Movie Rating | 6,041 | 3,953 | 1,000,209 |
| Anime | Movie Rating | 73,515 | 11,197 | 7,813,727 |
| Epinions | Product Rating | 120,492 | 755,760 | 13,668,320 |

The original datasets can be obtained from their corresponding public sources:

- DBLP: http://dblp.uni-trier.de/xml/
- MovieLens: https://grouplens.org/datasets/movielens
- Anime: https://www.kaggle.com/datasets/CooperUnion/anime-recommendations-database
- Epinions: http://konect.cc/networks/

Please preprocess the datasets into the input format required by the implementation before running the experiments.

## Environment

The implementation is written in **C++**.

The experiments reported in the paper were conducted on a Windows Server with:

- 2.10 GHz 16-core CPU
- 256 GB RAM

## Experimental Evaluation

We compare the following four methods:

- `Global`
- `Local`
- `HI+Global`
- `HI+Local`

The experiments evaluate query efficiency under different settings of:

- `α`
- `β`
- number of query keywords `|QK|`
- graph size `|E|`

The experimental results show that the HI-based methods consistently reduce query time compared with their corresponding baseline algorithms.

## Citation

If you find this work useful, please cite our paper:

```bibtex
@article{jiang2026kbcs,
  title   = {Keyword-constrained Community Search Over Bipartite Networks},
  author  = {Junyi Jiang and Xiaoxu Song and Guofeng Jin},
  year    = {2026}
}
