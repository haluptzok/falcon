import os
import time
from typing import List
import random
import numpy as np

'''
Q1. Implement this interface of 3 functions, all functions operate in *constant* time.
Hint: Use versioning trick for support of putAll.

Interface:
set(key, value): standard set
get(key): standard get
putAll(value): sets all previous key/value pairs to this value.  

putAll can't be done in constant time by simply enumerating through and updating all key/value pairs.
Instead have to use a version number trick and record key/(value, version) in the hash table.
Then in get, check if the version is the same as the current version.  If not, return the putAll(value).              
'''

class FunkyHash:
    def __init__(self):
        """
        Initialize FunkyHash
        """
        self.hash_table = {}  # Just use a dictionary to implement hash table
        self.put_version = 0 # version of hash table - each key has (value, put_version) stored. Older versions return last put_value set by putAll.
        self.put_value = 0 # value to use for previous versions.

    def set(self, key, value):
        """
        Set key/value pair - store tuple of the version in the hash table.
        """
        self.hash_table[key] = (self.put_version, value)

    def get(self, key):
        """
        Get value for key - return None if key not found.
        """
        if key in self.hash_table:
            if self.hash_table[key][0] == self.put_version:
                return self.hash_table[key][1]
            else:
                return self.put_value
        else:
            return None

    def putAll(self, value):
        """
        Effectively set all previous keys to this value
        Implemented by checking in get if stored put_version is the same as the current put_version.  If not, return the put_value.
        """
        self.put_version += 1
        self.put_value = value

def TestFunkyHash():
    h1 = FunkyHash()

    # Test set/get/putAll - interleave them a bit
    # make sure values are updated correctly from putAll

    h1.set("a", 1)
    h1.set("b", 2)
    h1.set("c", 3)
    h1.set("d", 4)
    assert h1.get("a") == 1
    assert h1.get("b") == 2
    assert h1.get("c") == 3
    assert h1.get("d") == 4
    assert h1.get("e") == None
    assert h1.get("g") == None
    h1.putAll(5)
    assert h1.get("g") == None
    h1.set("g", 9)
    assert h1.get("g") == 9
    assert h1.get("a") == 5
    assert h1.get("b") == 5
    assert h1.get("c") == 5
    assert h1.get("d") == 5
    assert h1.get("e") == None
    h1.set("g", 9)
    assert h1.get("a") == 5
    assert h1.get("b") == 5
    assert h1.get("c") == 5
    assert h1.get("d") == 5
    assert h1.get("e") == None
    assert h1.get("g") == 9
    h1.set("a", 1)
    h1.set("b", 2)
    h1.set("c", 3)
    h1.set("d", 4)
    h1.set("f", 7)
    assert h1.get("a") == 1
    assert h1.get("b") == 2
    assert h1.get("c") == 3
    assert h1.get("d") == 4
    assert h1.get("f") == 7
    assert h1.get("e") == None
    assert h1.get("g") == 9
    h1.putAll(11)
    assert h1.get("a") == 11
    assert h1.get("b") == 11
    assert h1.get("c") == 11
    assert h1.get("d") == 11
    assert h1.get("f") == 11
    assert h1.get("e") == None
    assert h1.get("g") == 11
    h1.set("a", 1)
    h1.set("b", 2)
    h1.set("c", 3)
    h1.set("d", 4)
    h1.set("f", 7)
    assert h1.get("a") == 1
    assert h1.get("b") == 2
    assert h1.get("c") == 3
    assert h1.get("d") == 4
    assert h1.get("f") == 7
    assert h1.get("e") == None
    assert h1.get("g") == 11
    h1.putAll(12)
    assert h1.get("a") == 12
    assert h1.get("b") == 12
    assert h1.get("c") == 12
    assert h1.get("d") == 12
    assert h1.get("f") == 12
    assert h1.get("e") == None
    assert h1.get("g") == 12
    print("TestFunkyHash passed")

TestFunkyHash()
    
# Question 2: Sparse Dot Product
# Solution: Represent a sparse vector as a list of tuples (index, value) where index is the location of the non-zero value, sorted by index

def SparseDotProduct(sparse1: List[tuple], sparse2: List[tuple]):
    """
    Sparse dot product computation
    """
    ret = 0  # dot product to return
    i = 0 # index for sparse1
    j = 0 # index for sparse2
    while i < len(sparse1) and j < len(sparse2):
        index1 = sparse1[i][0]
        value1 = sparse1[i][1]
        index2 = sparse2[j][0]
        value2 = sparse2[j][1]
        if index1 == index2:
            ret += value1 * value2
            i += 1
            j += 1
        elif index1 < index2:
            i += 1
        else: # index2 < index1
            j += 1
    
    # print("answer", ret, sparse1, sparse2)
    return ret

def TestSparseDotProduct():
    """
    Test SparseDotProduct
    """
    # test full overlap same vector
    sparse1 = [(0, 1), (1, 2), (2, 3), (3, 4)]
    assert SparseDotProduct(sparse1, sparse1) == 30
    # test full overlap different vectors
    sparse1 = [(0, 1), (1, 2), (2, 3), (3, 4)]
    sparse2 = [(0, 5), (1, 6), (2, 7), (3, 8)]
    assert SparseDotProduct(sparse1, sparse2) == 70
    # test no overlap 1 after 2
    sparse1 = [(10, 1), (11, 2), (12, 3), (13, 4)]
    sparse2 = [(0, 1), (1, 2), (2, 3), (3, 4)]
    assert SparseDotProduct(sparse1, sparse2) == 0
    # test no overlap 2 after 1
    sparse1 = [(0, 1), (1, 2), (2, 3), (3, 4)]
    sparse2 = [(10, 1), (11, 2), (12, 3), (13, 4)]
    assert SparseDotProduct(sparse1, sparse2) == 0
    # test partial overlap 1 before 2
    sparse1 = [(0, 1), (1, 2), (2, 3), (3, 4)]
    sparse2 = [(3, 1), (4, 2), (12, 3), (13, 4)]
    assert SparseDotProduct(sparse1, sparse2) == 4
    # test partial overlap 2 before 1
    sparse1 = [(3, 1), (4, 2), (12, 3), (13, 4)]
    sparse2 = [(0, 1), (1, 2), (2, 3), (3, 4)]
    assert SparseDotProduct(sparse1, sparse2) == 4
    # test empty 1
    sparse1 = []
    sparse2 = [(0, 1), (1, 2), (2, 3), (3, 4)]
    assert SparseDotProduct(sparse1, sparse2) == 0
    # test empty 2
    sparse1 = [(3, 1), (4, 2), (12, 3), (13, 4)]
    sparse2 = []
    assert SparseDotProduct(sparse1, sparse2) == 0
    # test empty 1 and empty 2
    sparse1 = []
    sparse2 = []
    assert SparseDotProduct(sparse1, sparse2) == 0
    # test different size inputs
    sparse1 = [(1, 2)]
    sparse2 = [(0, 5), (1, 6), (2, 7), (3, 8)]
    assert SparseDotProduct(sparse1, sparse2) == 12
    # test different size inputs
    sparse1 = [(0, 5), (1, 6), (2, 7), (3, 8)]
    sparse2 = [(1, 2)]
    assert SparseDotProduct(sparse1, sparse2) == 12
    print("TestSparseDotProduct passed")

TestSparseDotProduct()





