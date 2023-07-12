// rickj.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdio>
#include <cassert>
#include <map>
#include <unordered_map>
using namespace std;

// add a bunch of types {} ; [] to make Python --> C++ with the minimal amount of changes

/*
Q1. Implement this interface of 3 functions, all functions operate in *constant* time.
Hint: Use versioning trick for support of putAll.

Interface:
set(key, value): standard set
get(key): standard get
putAll(value): sets all previous key/value pairs to this value.

putAll can't be done in constant time by simply enumerating through and updating all key/value pairs.
Instead have to use a version number trick and record key/(value, version) in the hash table.
Then in get, check if the version is the same as the current version.  If not, return the putAll(value).
*/

class versionValue
{
public:
    int version;  // put_value version this value was stored with - value is overridend by put_value if this version isn't the current put_version
    int value;    
    versionValue() { version = 0; value = 0; }
    versionValue(int v, int val)
    {
        version = v;
        value = val;
    }
};

#define ERROR_INVALID_FH_KEY INT_MIN   // In Python we returned None - could pick -1 or just throw exception.

class FunkyHash
{
public:
    unordered_map<string, versionValue> hash_table; // Just use a map to implement hash table
    int put_version = 0;                  // version of hash table - each key has (value, version) stored. Older versions return last put_value set by putAll, instead of their value
    int put_value = 0;                    // value to use for versionValue records that have previous versions.
    FunkyHash()
    {
        return;
    }

    void set(string key, int value)
    {
        // Set key/value pair - store tuple of the version and value in the hash table.
        versionValue vV(put_version, value);
        hash_table[key] = vV;
    }

    int get(string key)
    {
        // Get value for key - return ERROR_INVALID_FH_KEY if key not found.

        if (hash_table.count(key))
        {
            versionValue vV = hash_table[key];
            if (vV.version == put_version)
                return vV.value;
            else
                return put_value;
        }
        else
            return ERROR_INVALID_FH_KEY;   // or throw exception?
    }

    void putAll(int value)
    {
        // Effectively set all previous keys to this value
        // Implemented by checking in get if stored put_version is the same as the current put_version.  If not, return the put_value.

        put_version += 1;
        put_value = value;
    }
};

void TestFunkyHash(void)
{
    FunkyHash h1;

    h1.putAll(100);

    // Test set/get/putAll - interleave them a bit
    // Make sure values are updated correctly from putAll

    h1.set("a", 1);
    h1.set("b", 2);
    h1.set("c", 3);
    h1.set("d", 4);
    assert( h1.get("a") == 1);
    assert( h1.get("b") == 2);
    assert( h1.get("c") == 3);
    assert( h1.get("d") == 4);
    assert( h1.get("e") == ERROR_INVALID_FH_KEY);
    assert( h1.get("g") == ERROR_INVALID_FH_KEY);
    h1.putAll(5);
    assert( h1.get("g") == ERROR_INVALID_FH_KEY);
    h1.set("g", 9);
    assert( h1.get("g") == 9);
    assert( h1.get("a") == 5);
    assert( h1.get("b") == 5);
    assert( h1.get("c") == 5);
    assert( h1.get("d") == 5);
    assert( h1.get("e") == ERROR_INVALID_FH_KEY);
    h1.set("g", 9);
    assert( h1.get("a") == 5);
    assert( h1.get("b") == 5);
    assert( h1.get("c") == 5);
    assert( h1.get("d") == 5);
    assert( h1.get("e") == ERROR_INVALID_FH_KEY);
    assert( h1.get("g") == 9);
    h1.set("a", 1);
    h1.set("b", 2);
    h1.set("c", 3);
    h1.set("d", 4);
    h1.set("f", 7);
    assert( h1.get("a") == 1);
    assert( h1.get("b") == 2);
    assert( h1.get("c") == 3);
    assert( h1.get("d") == 4);
    assert( h1.get("f") == 7);
    assert( h1.get("e") == ERROR_INVALID_FH_KEY);
    assert( h1.get("g") == 9);
    h1.putAll(11);
    assert( h1.get("a") == 11);
    assert( h1.get("b") == 11);
    assert( h1.get("c") == 11);
    assert( h1.get("d") == 11);
    assert( h1.get("f") == 11);
    assert( h1.get("e") == ERROR_INVALID_FH_KEY);
    assert( h1.get("g") == 11);
    h1.set("a", 1);
    h1.set("b", 2);
    h1.set("c", 3);
    h1.set("d", 4);
    h1.set("f", 7);
    assert( h1.get("a") == 1);
    assert( h1.get("b") == 2);
    assert( h1.get("c") == 3);
    assert( h1.get("d") == 4);
    assert( h1.get("f") == 7);
    assert( h1.get("e") == ERROR_INVALID_FH_KEY);
    assert( h1.get("g") == 11);
    h1.putAll(12);
    assert( h1.get("a") == 12);
    assert( h1.get("b") == 12);
    assert( h1.get("c") == 12);
    assert( h1.get("d") == 12);
    assert( h1.get("f") == 12);
    assert( h1.get("e") == ERROR_INVALID_FH_KEY);
    assert( h1.get("g") == 12);
    printf("TestFunkyHash passed");
}

/*
# Question 2: Sparse Dot Product
# Solution: Represent a sparse vector as an array of index and value for non-zero values (index, value) -> sparse_iv
# where index refers to the location of the non-zero value in full vector
# array sorted by index, and the end of array is represented by sparse_iv with index = -1 -> (-1, ???)
*/

class sparse_iv
{
public:
    int index;
    int value;
};

int SparseDotProduct(sparse_iv* sparse1, sparse_iv* sparse2)
{
    int ret = 0;  // dot product to return
    int i = 0;    // index for sparse1
    int j = 0;    // index for sparse2

    while (sparse1[i].index != -1 && sparse2[j].index != -1)
    {
        int index1 = sparse1[i].index;        
        int index2 = sparse2[j].index;

        if (index1 == index2)
        {
            int value1 = sparse1[i].value;
            int value2 = sparse2[j].value;
            ret += (value1 * value2);
            i += 1;
            j += 1;
        }
        else if (index1 < index2)
        {
            i += 1;
        }
        else // index2 < index1
        {
            j += 1;
        }
    }

    return ret;
}

void TestSparseDotProduct(void)
{
    // test full overlap same vector
    sparse_iv sparse1[] = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {-1, 0} };
    assert(SparseDotProduct(sparse1, sparse1) == 30);
    // test full overlap different vectors
    sparse_iv sparse2[] = { {0, 5}, {1, 6}, {2, 7}, {3, 8}, {-1, 0} };
    assert(SparseDotProduct(sparse1, sparse2) == 70);
    // test no overlap 1 after 2
    sparse_iv sparse3[] = { {10, 1}, {11, 2}, {12, 3}, {13, 4}, {-1, 0} };
    sparse_iv sparse4[] = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {-1, 0} };
    assert(SparseDotProduct(sparse3, sparse4) == 0);
    // test no overlap 2 after 1
    sparse_iv sparse5[] = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {-1, 0} };
    sparse_iv sparse6[] = { {10, 1}, {11, 2}, {12, 3}, {13, 4}, {-1, 0} };
    assert(SparseDotProduct(sparse5, sparse6) == 0);
    // test partial overlap 1 before 2
    sparse_iv sparse7[] = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {-1, 0} };
    sparse_iv sparse8[] = { {3, 1}, {4, 2}, {12, 3}, {13, 4}, {-1, 0} };
    assert(SparseDotProduct(sparse7, sparse8) == 4);
    // test partial overlap 2 before 1
    sparse_iv sparse9[] = { {3, 1}, {4, 2}, {12, 3}, {13, 4}, {-1, 0} };
    sparse_iv sparse10[] = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {-1, 0} };
    assert(SparseDotProduct(sparse9, sparse10) == 4);
    // test empty 1
    sparse_iv sparse11[] = { {-1, 0} };
    sparse_iv sparse12[] = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {-1, 0} };
    assert(SparseDotProduct(sparse11, sparse12) == 0);
    // test empty 2
    sparse_iv sparse13[] = { {3, 1}, {4, 2}, {12, 3}, {13, 4}, {-1, 0} };
    sparse_iv sparse14[] = { {-1, 0} };
    assert(SparseDotProduct(sparse13, sparse14) == 0);
    // test empty 1 and empty 2
    sparse_iv sparse15[] = { {-1, 0} };
    sparse_iv sparse16[] = { {-1, 0} };
    assert(SparseDotProduct(sparse15, sparse16) == 0);
    // test different size inputs
    sparse_iv sparse17[] = { {1, 2}, {-1, 0} };
    sparse_iv sparse18[] = { {0, 5}, {1, 6}, {2, 7}, {3, 8}, {-1, 0} };
    assert(SparseDotProduct(sparse17, sparse18) == 12);
    // test different size inputs
    sparse_iv sparse19[] = { {0, 5}, {1, 6}, {2, 7}, {3, 8}, {-1, 0} };
    sparse_iv sparse20[] = { {1, 2}, {-1, 0} };
    assert(SparseDotProduct(sparse19, sparse20) == 12);

    printf("TestSparseDotProduct passed\n");
}

int main()
{
    std::cout << "Hello World!\n";
    printf("Hello World\n");
    TestFunkyHash();
    TestSparseDotProduct();    
    return 0;
}

