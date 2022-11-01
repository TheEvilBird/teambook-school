
# files: generators.py gen.py stress.py

# =================================================================

import random
import heapq

# ============================================

def gen_num(L: int, R: int):
    """
    Generates a number between L and R.
    """
    return random.randint(L, R)

# ============================================

# abcdefghijklmnopqrstuvwxyz

def gen_string_abc(LEN: int, ALPH_LEN: int = 26):
    """
    Generates a string of length LEN using the first ALPH_LEN lowercase letters of the alphabet.
    """
    abc = "abcdefghijklmnopqrstuvwxyz"
    s = abc[:ALPH_LEN]
    res = ""
    for i in range(LEN):
        res += random.choice(s)
    return res

def gen_string_any_aplh(LEN: int, ALPH: str):
    """
    Generates a string of length LEN using ALPH as the alphabet.
    """
    res = ""
    # ALPH_LEN = len(ALPH)
    for i in range(LEN):
        kek = 1
        res += random.choice(ALPH)
    return res

# ============================================

def gen_tree(N: int):
    """
    Generates a tree with N vertices.
    """
    edges = []
    for i in range(2, N + 1):
        v = gen_num(1, i - 1)
        edges.append((v, i))
    return edges

def gen_DAG(N: int, M: int):
    """
    Generates a directed acyclic graph with N vertices and M edges.
    """
    edges = []
    # for i in range(1, N):
    #     if len(edges) == M:
    #         break
    #     v = gen_num(i + 1, N)
    #     edges.append((i, v))
    while len(edges) < M:
        v = gen_num(1, N - 1)
        u = gen_num(v + 1, N)
        edges.append((v, u))
    return edges

# ============================================

def gen_graph(N: int, M: int):
    """
    Generates a graph with N vertices and M edges.
    """
    edges_set = set()
    for i in range(M):
        v, u = 0, 0
        while (v, u) in edges_set or v == u:
            v, u = gen_num(1, N), gen_num(1, N)
            v, u = min(v, u), max(v, u)
        edges_set.add((v, u))
    return list(edges_set)

def gen_multigraph(N: int, M: int):
    """
    Generates a multigraph with N vertices and M edges.
    """
    edges = []
    for i in range(M):
        v, u = -1, 0
        while v == -1:
            v, u = gen_num(1, N), gen_num(1, N)
            v, u = min(v, u), max(v, u)
        edges.append((v, u))
    return edges

def gen_directed_graph(N: int, M: int):
    """
    Generates a directed graph with N vertices and M edges.
    """
    edges_set = set()
    for i in range(M):
        v, u = 0, 0
        while (v, u) in edges_set or v == u:
            v, u = gen_num(1, N), gen_num(1, N)
        edges_set.add((v, u))
    return list(edges_set)

def gen_connected_directed_graph(N: int, M: int):
    """
    Generates a directed connected graph with N vertices and M edges.
    """
    edges_set = set(gen_tree(N))
    for i in range(M - (N - 1)):
        v, u = 0, 0
        while (v, u) in edges_set or v == u:
            v, u = gen_num(1, N), gen_num(1, N)
        edges_set.add((v, u))
    return list(edges_set)

def gen_connected_graph(N: int, M: int):
    """
    Generates a connected graph with N vertices and M edges.
    """
    edges_set = set(gen_tree(N))
    for i in range(M - (N - 1)):
        v, u = 0, 0
        while (v, u) in edges_set or v == u:
            v, u = gen_num(1, N), gen_num(1, N)
            v, u = min(v, u), max(v, u)
        edges_set.add((v, u))
    return list(edges_set)

def gen_connected_multigraph(N: int, M: int):
    """
    Generates a connected multigraph with N vertices and M edges.
    """
    edges = gen_tree(N)
    for i in range(M - (N - 1)):
        v, u = 0, 0
        while v == u:
            v, u = gen_num(1, N), gen_num(1, N)
        edges.append((v, u))
    return edges

# ============================================

def gen_perm(N: int, FIR: int = 1):
    """
    Generates a permutation of length N with min element FIR.
    """
    arr = [FIR + i for i in range(N)]
    # arr = arr[1:]
    random.shuffle(arr)
    return arr

def gen_array(N: int, L: int, R: int):
    """
    Generates an array of length N with elements between L and R.
    """
    arr = [gen_num(L, R) for i in range(N)]
    return arr

def gen_array_pairs(N: int, L: int, R: int):
    """
    Generates an array of pairs of length N with elements between L and R.
    """
    arr = [(gen_num(L, R), gen_num(L, R)) for i in range(N)]
    return arr

def gen_array_pairs(N: int, L1: int, R1: int, L2: int, R2: int):
    """
    Generates an array of pairs of length N with the first elements of each pair between L1 and R1 and between L2 and R2 for the second element.
    """
    arr = [(gen_num(L1, R1), gen_num(L2, R2)) for i in range(N)]
    return arr

# ============================================

def gen_tree_ivanq(N: int):
    """
    Generates a tree with N vertices. Code by IvanQ.
    """
    code = [random.randint(1, N) for _ in range(N - 2)]
    histogram = [0] * (N + 1)
    unused = list(set(range(1, N + 1)) - set(code))
    heapq.heapify(unused)
    for u in code:
        histogram[u] += 1
    
    res = []
    for u in code:
        v = heapq.heappop(unused)
        res.append((v, u))
        histogram[u] -= 1
        if histogram[u] == 0:
            heapq.heappush(unused, u)
    return res + [tuple(unused)]

# =================================================================

import sys
import random
from generators import *

SEED = 228 
if len(sys.argv) > 1:
    SEED = int(sys.argv[1])
random.seed(SEED)

# print(gen_num(1, 666))

# =================================================================

import os
import sys 

os.system("g++ -std=c++17 smart.cpp -o smart")
# os.system("g++ -std=c++17 -g -fsanitize=undefined -fsanitize=bounds -fsanitize=address -D_GLIBCXX_DEBUG smart.cpp -o smart")
os.system("g++ -std=c++17 stupid.cpp -o stupid")

def print_testcase():
    test = open("test.txt").read().strip()
    print(test)
    print("=============")
    ans1 = open("smart.out").read().strip()
    ans2 = open("stupid.out").read().strip()
    print(ans1)
    print("=============")
    print(ans2)
    print("=============")
    
i = 0
while True:
    os.system(f"python3 gen.py {i} > test.txt")

    r1 = os.system("./smart < test.txt > smart.out")
    r2 = os.system("./stupid < test.txt > stupid.out")

    if r1 + r2 != 0:
        print(f"Runtime! {i}")
        print_testcase()
        exit(0)

    ans1 = open("smart.out").read()
    ans2 = open("stupid.out").read()

    if ans1 != ans2:
        print(f"POPALCYA! {i}\n")
        print_testcase()
        exit(0)
    print(f"OK: {i}")
    i += 1