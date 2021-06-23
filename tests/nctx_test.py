import os,sys
import unittest

class TestCases(unittest.TestCase):

  def test_init(self):
    import nctx.directed as nctx
    g = nctx.Graph()
    self.assertEqual(g.numVertices(),0)
    self.assertEqual(g.numEdges(),0)
    # ~ del sys.modules('nctx.directed')
  
  def test_mod_graph(self):
    import nctx.directed as nctx
    from collections import defaultdict
    g = nctx.Graph()
    self.assertEqual(g.numVertices(),0)
    self.assertEqual(g.numEdges(),0)
    
    vertices = defaultdict(lambda: g.add_vertex())
    for i in range(10):
      v = vertices[i]
    
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numEdges(),0)
    
    g = nctx.Graph()
    self.assertEqual(g.numVertices(),0)
    self.assertEqual(g.numEdges(),0)
    name_map = nctx.PropertyMapStr(g, "name")
    vertices = defaultdict(lambda: g.add_vertex())
    for i in range(10):
      v = vertices[i]
      name_map.set_elem(v, "vertex %d"%i)
    
    for i in g.vertices():
      self.assertEqual(name_map.get_elem(i), "vertex %d"%i)
      
    g.add_edge(vertices[0], vertices[1])
    g.add_edge(vertices[1], vertices[2])
    g.add_edge(vertices[2], vertices[3])
    g.add_edge(vertices[3], vertices[4])
    g.add_edge(vertices[2], vertices[4])
    g.add_edge(vertices[4], vertices[8])
    g.add_edge(vertices[5], vertices[3])
    g.add_edge(vertices[6], vertices[3])
    g.add_edge(vertices[1], vertices[5])
    g.add_edge(vertices[7], vertices[1])
    g.add_edge(vertices[8], vertices[3])
    g.add_edge(vertices[2], vertices[9])
    g.add_edge(vertices[9], vertices[4])
    g.add_edge(vertices[9], vertices[8])
    self.assertEqual(g.numEdges(), 14)
    self.assertEqual(g.numVertices(),10)
    # ~ nctx.write_graphml(g, "test.graphml", [name_map])
    
    
    g = nctx.Graph()
    self.assertEqual(g.numVertices(),0)
    self.assertEqual(g.numEdges(),0)
    for i in range(10):
      g.add_vertex()
    
    g.add_edge(0, 1)
    g.add_edge(1, 2)
    g.add_edge(2, 3)
    g.add_edge(3, 4)
    g.add_edge(2, 4)
    g.add_edge(4, 8)
    g.add_edge(5, 3)
    g.add_edge(6, 3)
    g.add_edge(1, 5)
    g.add_edge(7, 1)
    g.add_edge(8, 3)
    g.add_edge(2, 9)
    g.add_edge(9, 4)
    g.add_edge(9, 8)
    nctx.write_graphml(g, "/home/mirco/test.graphml", [])
    self.assertEqual(g.numEdges(), 14)
    self.assertEqual(g.numVertices(),10)
    
  
  def test_pmap(self):
    import nctx.directed as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    self.assertEqual(name_map.get_name(),"name")
    self.assertEqual(len(name_map),0)
    test_map = nctx.PropertyMapVecDouble(g, "context")
    test_map.set_list(0,  [6,7,8,9,10])
    self.assertEqual(test_map.get_elem(0,2), 8.0)
    
    complex_map = nctx.PropertyMapVecDouble(g, 'context')
    complex_map.set_list(0, [1,2,3,4,5])
    complex_map.get_elem(0,2) # this will return 3.0 (float) instead of 3 (int)!
    
  def test_assign_context(self):
    import random
    import nctx.directed as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.directed.graphml"), [name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(name_map.get_elem(1), 'B')
    
    context_map = nctx.PropertyMapDouble(g, "context")
    for v in g.vertices():
      context_map.set_elem(v, random.uniform(0.0, 1.0))
    
  def test_pmap_container(self):
    import numpy as np
    from nctx import core
    import nctx.directed as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    complex_map = nctx.PropertyMapVecDouble(g, "context")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.ctx.graphml"), [complex_map,name_map])
    self.assertEqual(complex_map.get_name(),"context")
    self.assertEqual(len(complex_map), 10)
    self.assertEqual(len(complex_map.get_list(0)), 5)
    self.assertEqual(type(complex_map.get_list(0)).__name__, "VecDouble")
    self.assertEqual(complex_map.get_elem(0,0), 9.13063e-08)
    self.assertEqual(complex_map.get_elem(9,2), 0.00556908)
    complex_map.set_list(0,  [1.0,2.0,3.0,4.0,5.0])
    self.assertEqual(complex_map.get_elem(0,2), 3.0)
    complex_map.set_list(0,  [6,7,8,9,10])
    self.assertEqual(complex_map.get_elem(0,2), 8.0)
    # ~ complex_map.set_list(0,  [1.0,2.0,3.0,"4.0",5.0])
    
    
    np_arr = np.array([np.array(xi) for xi in complex_map])
    print(np_arr)
    
    np_arr = np.array(list(name_map))
    print(np_arr)
    
    
  def test_io_directed(self):
    import nctx.directed as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.directed.graphml"), [name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numEdges(), 14)
    self.assertEqual(len(name_map), 10)
    self.assertEqual(name_map.get_elem(1), 'B')
    # ~ del sys.modules('nctx.directed')
    print("""io todo:
    * writer testen""")
    
  def test_io_undirected(self):
    import nctx.undirected as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.undirected.graphml"), [name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numEdges(), 14)
    self.assertEqual(len(name_map), 10)
    self.assertEqual(name_map.get_elem(1), 'B')
    print("""io todo:
    * writer testen""")
    # ~ del sys.modules('nctx.undirected')
    
  def test_iteration(self):
    import nctx.directed as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.directed.graphml"), [name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(g.numEdges(), 14)
    self.assertEqual(name_map.get_elem(1), 'B')
    for v in g.vertices():
      name_map.get_elem(v)
    start = 4 # E
    for c in g.children(start):
      name_map.get_elem(c)
      # ~ print(name_map.get_elem(start), name_map.get_elem(c))
    for p in g.parents(start):
      name_map.get_elem(p)
      # ~ print(name_map.get_elem(start), name_map.get_elem(p))
    
    for e in g.edges():
      source = g.source(e)
      target = g.target(e)
      # ~ print(f"{source} -> {target}")
    
    # ~ print("---")
    
    for e in g.in_edges(start):
      source = g.source(e)
      target = g.target(e)
      # ~ print(f"{source} -> {target}")
      self.assertEqual(target, start)
      self.assertNotEqual(source, start)
    
    for e in g.out_edges(start):
      source = g.source(e)
      target = g.target(e)
      # ~ print(f"{source} -> {target}")
      self.assertEqual(source, start)
      self.assertNotEqual(target, start)
    # ~ del sys.modules('nctx.directed')
    
    # ~ print("---")
    
    import nctx.undirected as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.undirected.graphml"), [name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(g.numEdges(), 14)
    self.assertEqual(name_map.get_elem(1), 'B')
    for v in g.vertices():
      name_map.get_elem(v)
    start = 4 # E
    for c in g.children(start):
      name_map.get_elem(c)
      # ~ print(name_map.get_elem(start), name_map.get_elem(c))
    for p in g.parents(start):
      name_map.get_elem(p)
      # ~ print(name_map.get_elem(start), name_map.get_elem(p))
      
    for e in g.in_edges(start):
      source = g.source(e)
      target = g.target(e)
      # ~ print(f"{source} -> {target}")
      # ~ self.assertEqual(target, start)
      # ~ self.assertNotEqual(source, start)
    
    for e in g.out_edges(start):
      source = g.source(e)
      target = g.target(e)
      # ~ print(f"{source} -> {target}")
    
    import string
    import nctx.core as utils
    import nctx.directed as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    context_map = nctx.PropertyMapVecDouble(g, "context")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.ctx.graphml"), [name_map, context_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(name_map.get_elem(1), 'B')
    
    names = string.ascii_uppercase[0:10]
    for i,n in enumerate(name_map):
      self.assertEqual(n, names[i])
    
    name_map[1] = 'X'
    self.assertEqual(name_map.get_elem(1), 'X')
    self.assertEqual(name_map[1], 'X')
    
    test = list(context_map[1]) # need to create a deep copy of the first list!
    test_pointer = context_map[1]
    context_map[1] = [.1,.2,.3,.4,.5]
    for i,v in enumerate(context_map[1]):
      self.assertNotEqual(test[i], v)
      self.assertEqual(test_pointer[i], v)
    self.assertEqual(context_map[1][1], .2)
    context_map[1][1] = .1337
    self.assertEqual(context_map[1][1], .1337)
    
    self.assertTrue('X' in name_map)
    self.assertTrue([.1,.1337,.3,.4,.5] in context_map)
    
    betw_ctx = nctx.AlgCentralities.betweenness_ctx(g, lambda _x, _y, _z: (True))
    # ~ print(type(betw_ctx).__name__)
    cls_ctx = nctx.AlgCentralities.closeness_ctx(g, lambda _x, _y, _z: (True))
    # ~ print(type(cls_ctx).__name__)
    self.assertEqual(utils.angular_distance(betw_ctx, cls_ctx), 0.23539316999405233)
    
    
    test_map = nctx.PropertyMapVecStr(g, 'test', 'gnaaa', 10)
    for i,subm in enumerate(test_map):
      # ~ print(i, subm)
      self.assertEqual(len(subm), 10)
      for j,elem in enumerate(subm):
        # ~ print(i, elem)
        self.assertEqual(elem, 'gnaaa')
    # ~ test_map[2][5] = 'bla'
    test_map = nctx.PropertyMapStr(g, 'test', 'bla')
    for i,elem in enumerate(test_map):
      # ~ print(i,elem)
      self.assertEqual(elem, 'bla')

      
  def test_betweenness(self):
    import nctx.core as core
    import nctx.directed as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    context_map = nctx.PropertyMapVecDouble(g, "context")
    # ~ centr_constr = nctx.PropertyMapDouble(g, "centrlt")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.ctx.graphml"), [context_map,name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(name_map.get_elem(1), 'B')
    
    max_diff = .33
    
    def decision(start, l, r):
      lc = context_map.get_list(start)
      rc = context_map.get_list(r)
      return abs(core.kl_divergence(lc,rc)) <= max_diff
    
    betw_ctx = nctx.AlgCentralities.betweenness_ctx(g, decision)
    # ~ nctx.AlgCentralities.betweenness_ctx(g, decision, centr_constr)
    betw = nctx.AlgCentralities.betweenness(g)
    for i in g.vertices():
      # ~ print(name_map.get_elem(i), ":", betw_ctx.get_elem(i), "vs", betw.get_elem(i))
      # ~ self.assertEqual(betw_ctx.get_elem(i), centr_constr.get_elem(i))
      self.assertTrue(betw_ctx.get_elem(i) <= betw.get_elem(i))
    # ~ del sys.modules('nctx.directed')
    
    
  def test_closeness(self):
    import nctx.undirected as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.undirected.graphml"), [name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(name_map.get_elem(1), 'B')
    
    # ~ centr_constr = nctx.PropertyMapDouble(g, "centrlt")
    
    def decision(start, l, r):
      return True
    
    clsn_ctx = nctx.AlgCentralities.closeness_ctx(g, decision)
    # ~ nctx.AlgCentralities.closeness_ctx(g, decision, centr_constr)
    clsn = nctx.AlgCentralities.closeness(g)
    for i in g.vertices():
      # ~ print(name_map.get_elem(i), ":", clsn_ctx.get_elem(i), "vs", clsn.get_elem(i))
      # ~ self.assertEqual(clsn_ctx.get_elem(i), centr_constr.get_elem(i))
      self.assertEqual(clsn_ctx.get_elem(i), clsn.get_elem(i))
    # ~ del sys.modules('nctx.directed')
    
    def decision(start, l, r):
      # ~ print(name_map.get_elem(l), "->", name_map.get_elem(r), ": ", not (name_map.get_elem(r)=="D" and (name_map.get_elem(l) =="E" or name_map.get_elem(l) =="C")))
      return not (name_map.get_elem(r)=="D" and (name_map.get_elem(l) =="E" or name_map.get_elem(l) =="C")) and not (name_map.get_elem(l)=="D" and (name_map.get_elem(r) =="E" or name_map.get_elem(r) =="C"))
    clsn_ctx = nctx.AlgCentralities.closeness_ctx(g, decision)
    for i in g.vertices():
      # ~ print(name_map.get_elem(i), ":", clsn_ctx.get_elem(i), "vs", clsn.get_elem(i))
      self.assertTrue(clsn_ctx.get_elem(i) <= clsn.get_elem(i))
      
  def test_dijkstra(self):
    import nctx.core as core
    import nctx.directed as nctx
    g = nctx.Graph()
    context_map = nctx.PropertyMapVecDouble(g, "context")
    name_map = nctx.PropertyMapStr(g, "name")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.ctx.graphml"), [context_map,name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(name_map.get_elem(1), 'B')
    
    shortpth_constr = nctx.PropertyMapULong(g, "shrtps")
    
    max_diff = .33
    
    def decision(start, l, r):
      lc = context_map.get_list(l)
      rc = context_map.get_list(r)
      return abs(core.js_distance(lc,rc)) <= max_diff
    
    start = 3
    sp_ctx = nctx.AlgPaths.dijkstra_ctx(g, start, decision)
    nctx.AlgPaths.dijkstra_ctx(g, start, decision, shortpth_constr)
    sp = nctx.AlgPaths.dijkstra(g, start)
    for i in g.vertices():
      # ~ print("from",name_map.get_elem(start),"to",name_map.get_elem(i), ":", sp_ctx.get_elem(i), "vs", sp.get_elem(i))
      self.assertEqual(shortpth_constr.get_elem(i), sp_ctx.get_elem(i))
    self.assertEqual(shortpth_constr.get_elem(start), 0.0)
    
    def decision(start, l, r):
      # ~ print(name_map.get_elem(l), "->", name_map.get_elem(r), ": ", not (name_map.get_elem(r)=="D" and (name_map.get_elem(l) =="E" or name_map.get_elem(l) =="C")))
      return not (name_map.get_elem(r)=="D" and (name_map.get_elem(l) =="E" or name_map.get_elem(l) =="C")) and not (name_map.get_elem(l)=="D" and (name_map.get_elem(r) =="E" or name_map.get_elem(r) =="C"))
    sp_ctx = nctx.AlgPaths.dijkstra_ctx(g, start, decision)
    # ~ for i in g.vertices():
      # ~ print("from",name_map.get_elem(start),"to",name_map.get_elem(i), ":", sp_ctx.get_elem(i), "vs", sp.get_elem(i))
      
  def test_apsp(self):
    from nctx import core
    import nctx.directed as nctx
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.directed.graphml"), [name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(name_map.get_elem(1), 'B')
    
    distance_matrix = nctx.PropertyMapVecULong(g, "distances")
    
    def decision(start, l, r):
      return True
    
    apsp_ctx = nctx.AlgPaths.dijkstra_apsp_ctx(g, decision)
    nctx.AlgPaths.dijkstra_apsp_ctx(g, decision, distance_matrix)
    
    for i in g.vertices():
      for j in g.vertices():
        # ~ print("from",name_map.get_elem(i),"to",name_map.get_elem(j), ":", apsp_ctx.get_elem(i,j), " bzw ", distance_matrix.get_elem(i,j))
        self.assertEqual(apsp_ctx.get_elem(i,j),distance_matrix.get_elem(i,j))
    
    
  def test_find_path_ctx(self):
    import nctx.directed as nctx
    
    def decision(start, l, r):
      return True
      
    g = nctx.Graph()
    name_map = nctx.PropertyMapStr(g, "name")
    context_map = nctx.PropertyMapVecDouble(g, "context")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.ctx.graphml"), [context_map,name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(name_map.get_elem(1), 'B')
    
    start = 3
    goal  = 7
    sp_ctx = nctx.AlgPaths.find_path_ctx(g, start, goal, decision)
    self.assertEqual(len(sp_ctx), 4)
    sp = nctx.AlgPaths.find_path(g, start, goal)
    self.assertEqual(len(sp), 4)
    start = 0
    goal  = 8
    sp_ctx = nctx.AlgPaths.find_path_ctx(g, start, goal, decision)
    self.assertEqual(len(sp_ctx), 5)
    sp = nctx.AlgPaths.find_path(g, start, goal)
    self.assertEqual(len(sp), 5)
  
  def test_path_simple(self):
    import nctx.undirected as nctx
    g = nctx.Graph()
    
    context = [1,1,0,1,1,1,1,0,1,0]
    def decision(strt, crnt, nxt):
      # ~ print(crnt, "->", nxt, ":", context[crnt], "==", context[nxt])
      return context[crnt] == context[nxt]
    
    for i in range(10):
      print(g.add_vertex())
    
    g.add_edge(0, 1)
    g.add_edge(1, 2)
    g.add_edge(2, 3)
    g.add_edge(3, 4)
    g.add_edge(2, 4)
    g.add_edge(4, 8)
    g.add_edge(5, 3)
    g.add_edge(6, 3)
    g.add_edge(1, 5)
    g.add_edge(7, 1)
    g.add_edge(8, 3)
    g.add_edge(2, 9)
    g.add_edge(9, 4)
    g.add_edge(9, 8)
    
    path = nctx.AlgPaths.find_path_ctx(g, 0, 6, decision)
    self.assertEqual(path, [0,1,5,3,6])
    reachables = nctx.AlgPaths.dijkstra_ctx(g, 0, decision)
    self.assertTrue(reachables[1],1)
    self.assertTrue(reachables[3],3)
    self.assertTrue(reachables[4],4)
    self.assertTrue(reachables[5],2)
    self.assertTrue(reachables[6],4)
    self.assertTrue(reachables[8],4)
    
  def test_find_path(self):
    import nctx.directed as nctxdir
    import nctx.undirected as nctxundir
    
    def decision(start, l, r):
      return True
      
    g = nctxdir.Graph()
    name_map = nctxdir.PropertyMapStr(g, "name")
    nctxdir.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.directed.graphml"), [name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(name_map.get_elem(1), 'B')
    
    start = 3
    goal  = 7
    sp_ctx = nctxdir.AlgPaths.find_path_ctx(g, start, goal, decision)
    self.assertEqual(len(sp_ctx), 0)
    sp = nctxdir.AlgPaths.find_path(g, start, goal)
    self.assertEqual(len(sp), 0)
    start = 0
    goal  = 8
    sp_ctx = nctxdir.AlgPaths.find_path_ctx(g, start, goal, decision)
    self.assertEqual(len(sp_ctx), 5)
    sp = nctxdir.AlgPaths.find_path(g, start, goal)
    self.assertEqual(len(sp), 5)
    
    # -----
    
    g = nctxundir.Graph()
    name_map = nctxundir.PropertyMapStr(g, "name")
    nctxundir.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.undirected.graphml"), [name_map])
    self.assertEqual(g.numVertices(), 10)
    self.assertEqual(g.numVertices(), len(g.vertices()))
    self.assertEqual(name_map.get_elem(1), 'B')
    
    start = 3
    goal  = 7
    sp_ctx = nctxundir.AlgPaths.find_path_ctx(g, start, goal, decision)
    self.assertEqual(len(sp_ctx), 4)
    sp = nctxundir.AlgPaths.find_path(g, start, goal)
    self.assertEqual(len(sp), 4)
    start = 0
    goal  = 8
    sp_ctx = nctxundir.AlgPaths.find_path_ctx(g, start, goal, decision)
    self.assertEqual(len(sp_ctx), 5)
    sp = nctxundir.AlgPaths.find_path(g, start, goal)
    self.assertEqual(len(sp), 5)
  
  def test_distances(self):
    import nctx.core as c
    import numpy as np
    A = [-0.3805950,-1.4635000,1.7565629,1.1039740,0.4493004,0.4984236,-0.8446116,2.2833076,0.2598573,-0.9920936]
    B = [0.03065272,0.08561547,1.35419445,1.21674446,1.46020546,1.75870975,-0.46519233,0.03100334,-0.12786839,0.04064652]
    
    self.assertEqual(c.cosine_similarity(A,B), 0.49634136855031963)
    self.assertEqual(c.euclidean_distance(A,B), 3.433288222859105)
    self.assertEqual(c.angular_distance(A,B), 0.3346764408212951)
    A = np.abs(A)
    A = A/np.sum(A) 
    B = np.abs(B)
    B = B/np.sum(B)
    self.assertEqual(c.kl_divergence(A,B), 1.3689655011086466)
    self.assertEqual(c.js_divergence(A,B), 0.21286088091616043)
    
    
    import nctx.directed as nctx
    g = nctx.Graph()
    context_map = nctx.PropertyMapVecDouble(g, "context")
    nctx.read_graphml(g, os.path.join(os.path.dirname(os.path.realpath(__file__)),"bcde.ctx.graphml"), [context_map])
    list1 = np.abs(context_map[0])
    list2 = [.1,.2,.3,.4,.5]
    list1 = list1/np.sum(list1)
    list2 = list2/np.sum(list2)
    self.assertEqual(c.js_divergence(list1, list2), 0.11794226754027952)
    self.assertEqual(c.kl_divergence(list1, list2), 0.38236213356053317)
    self.assertEqual(c.cosine_similarity(context_map[0], [.1,.2,.3,.4,.5]), 0.8368374907292926)
    self.assertEqual(c.euclidean_distance(context_map[0], [.1,.2,.3,.4,.5]), 0.4063153090787592)
    self.assertEqual(c.angular_distance(context_map[0], [.1,.2,.3,.4,.5]), 0.18440190063093045)
    
  
if __name__=='__main__':
  unittest.main()
