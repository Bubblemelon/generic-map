// A generic Map implemented with right-threaded BST
// BST is not balanced

#ifndef MAP_MAP_H  //optional
#define MAP_MAP_H  //optional

#include <iostream>
#include <map>
using namespace std;

template <typename KEY, typename T>
class Map{
private:
    struct Elem {
        KEY key;
        T data;
        Elem *left;
        Elem *right;
        bool rightThread;  // right child link or a thread link
    };
    Elem *_root;  // dummy root sentinel
    int _size;

    // helper function for inserting the key's corresponding data into tree.
    bool insert(Elem *& root, const KEY &key, const T &data, Elem *lastLeft);

    // helper function for print tree
    void printTree(ostream& out, int level, Elem *p) const;

    // common code for deallocation
    void destructCode(Elem *& p);

    // common code for copy tree
    void copyCode(Elem* &newRoot, Elem* origRoot);

    // common code for deep copying threaded tree
    void addToMap(Elem* root, map<KEY, Elem*> &keyElemMap);
    void copyThread(Elem* &newRoot, Elem* origRoot);

public:
    // a simple Iterator, traverse the collection in one direction
    class Iterator{
    public:
        Iterator(){}
        explicit Iterator(Elem *cur):_cur(cur) {}
        Elem& operator*();
        Elem* operator->();
        Iterator operator++(int);
        bool operator==(Iterator it);
        bool operator!=(Iterator it);
    private:
        Elem* _cur;
    };
    Iterator begin() const;
    Iterator end() const;

    // constructs empty Map
    Map();

    // copy constructor
    Map(const Map &rhs);

    // destructor
    ~Map();

    // assignment operator
    Map& operator=(const Map &rhs);

    // inserts an element into the Map; return true if successful
    bool insert(KEY, T);

    // removes an element from the Map; return true if successful
    bool erase(KEY);

    // return size of the Map
    int size() const;

    // returns an iterator pointing to the end if the element is not found,
    // otherwise, returns an iterator to the element
    Iterator find(KEY) const;

    // overloaded subscript operator
    T& operator[](KEY);

    // test output the underlying BST
    ostream& dump(ostream& out) const;
};

template<typename KEY, typename T>
ostream& operator<< (ostream&, const Map<KEY, T>&);

#include "Map.cpp"  // this is the include source file for template compilation


#endif //MAP_MAP_H   //optional
