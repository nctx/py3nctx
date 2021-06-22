#ifndef _NCTX_H_
#define _NCTX_H_

#ifndef OUT_SEP
#define OUT_SEP '\t'
#endif

#include <iostream>

#include "utils/distances.hpp"

#include "topology/betweenness_centrality_ctx.hpp"
#include "topology/ctx_bfs_visitor.hpp"
#include "topology/closeness_centrality_ctx.hpp"
#include "topology/page_rank_ctx.hpp"
#include "topology/dijkstra_ctx.hpp"

#include "io/io_operations.hpp"

#include "graph/container.hpp"

#endif
