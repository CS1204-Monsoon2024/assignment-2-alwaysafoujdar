#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

class HashTable {
private:
    struct HashNode {
        int key;
        bool isDeleted;

        HashNode(int k) : key(k), isDeleted(false) {}
    };

    HashNode** table;  
    int capacity;
    int size;
    const float loadFactorThreshold;

    // Hash function we're asked to ues in the ques. is h(x) = k mod m
    int hashFunction(int key) {
        return key % capacity;
    }

    //I plan to use Miller Rabin for Primality Testing
    //I took help from GeeksforGeeks for the implementation of the Miller-Rabin test and used ChatGpt to fix some of the errors I encountered
    long long modExp(long long base, long long exp, long long mod) {
        long long result = 1;
        base = base % mod;
        
        while (exp > 0) {
            if (exp % 2 == 1) {  // 
                result = (result * base) % mod;
            }
            exp = exp >> 1;  
            base = (base * base) % mod;
        }
        
        return result;
    }

    // Helper function for Miller-Rabin 
    bool millerTest(long long d, long long n) {
        long long a = 2 + rand() % (n - 4);
        long long x = modExp(a, d, n);

        if (x == 1 || x == n - 1)
            return true;
        while (d != n - 1) {
            x = (x * x) % n;
            d *= 2;
            if (x == 1) return false;
            if (x == n - 1) return true;
        }
        return false;
    }

    bool isPrime(long long n, int k = 5) {
        if (n <= 1 || n == 4) return false;
        if (n <= 3) return true;
        long long d = n - 1;
        while (d % 2 == 0)
            d /= 2;

        for (int i = 0; i < k; i++)
            if (!millerTest(d, n))
                return false;

        return true;
    }

    int next_prime(int num) {
        while (!isPrime(num)) {
            num++;
        }
        return num;
    }

    void resize() {
        int old_capacity = capacity;
        capacity = next_prime(2 * capacity);  
        HashNode** old_table = table;
        table = new HashNode*[capacity];

        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }

        size = 0;

        for (int i = 0; i < old_capacity; i++) {
            if (old_table[i] != nullptr && !old_table[i]->isDeleted) {
                insert(old_table[i]->key);
                delete old_table[i];  
            }
        }

        delete[] old_table;  
    }

public:
    HashTable(int initial_capacity = 7)
        : loadFactorThreshold(0.8) {
        capacity = next_prime(initial_capacity);  
        size = 0;
        table = new HashNode*[capacity];

        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            if (table[i] != nullptr) {
                delete table[i];
            }
        }
        delete[] table;
    }

    float loadFactor() {
        return (float)size / capacity;
    }

    void insert(int key) {
        if (loadFactor() >= loadFactorThreshold) {
            resize();
        }

        int index = hashFunction(key);
        int i = 0;

        //quadractic probing
        while (table[(index + i * i) % capacity] != nullptr && !table[(index + i * i) % capacity]->isDeleted) {
            if (table[(index + i * i) % capacity]->key == key) {
                std::cout << "Duplicate key insertion is not allowed" << std::endl;
                return;
            }
            i++;
            if (i == capacity) {
                std::cout << "Max probing limit reached!" << std::endl;
                return;
            }
        }

        table[(index + i * i) % capacity] = new HashNode(key);
        size++;
    }

    int search(int key) {
        int index = hashFunction(key);
        int i = 0;

        while (table[(index + i * i) % capacity] != nullptr) {
            if (!table[(index + i * i) % capacity]->isDeleted && table[(index + i * i) % capacity]->key == key) {
                return (index + i * i) % capacity;
            }
            i++;
            if (i == capacity) {
                return -1;
            }
        }

        return -1;  
    }

    void remove(int key) {
        int index = search(key);
        if (index == -1) {
            std::cout << "Element not found" << std::endl;
            return;
        }

        table[index]->isDeleted = true;
        size--;
    }

    void printTable() {
        for (int i = 0; i < capacity; i++) {
            if (table[i] == nullptr || table[i]->isDeleted) {
                std::cout << "- ";
            } else {
                std::cout << table[i]->key << " ";
            }
        }
        std::cout << std::endl;
    }
};




