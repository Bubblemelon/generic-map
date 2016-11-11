// A generic Map implemented with right-threaded BST
// BST is not balanced

#include "Map.h"

#include <map> // optional used in the helper function for thread copying

template <typename KEY, typename T>
Map<KEY, T>::Map()
{
    // create a dummy root node
    _root = new Elem;
    _root->left = _root;  // empty tree
    _root->right = 0;
    _root->rightThread = false;
    _size = 0;

}


// copy constructor
template <typename KEY, typename T>
Map<KEY, T>::Map(const Map<KEY,T> &v){
    // if empty tree
    if (v._root == v._root->left){
        _root = new Elem;
        _root->left = _root;  // empty tree
        _root->right = 0;
        _size = 0;
    } else {
        _root = new Elem;
        _root->left = _root;
        _root->right = 0;
        copyCode(_root->left, v._root->left); // to deep copy the tree without dummy nodes
        copyThread(_root, v._root); // to copy the threading; must pass dummy nodes to complete the threads
        _size = v._size;
    }
}

// common copy code for deep copy a tree without copying threads
template <typename KEY, typename T>
void  Map<KEY,T>::copyCode(Elem* &newRoot, Elem* origRoot){

    if (origRoot == 0)
        newRoot = 0;
    else{
        newRoot = new Elem;
        newRoot->key = origRoot->key;
        newRoot->data = origRoot->data;
        newRoot->rightThread = origRoot->rightThread;
        copyCode(newRoot->left, origRoot->left);
        if (!origRoot->rightThread)
            copyCode(newRoot->right, origRoot->right);
    }
}



// construct a key-element map to rethread the new tree
// the map contains all nodes key values and their corresponding elem node address
// for future lookup in setting up threads
template <typename KEY, typename T>
void Map<KEY, T>::addToMap(Elem* root, map<KEY, Elem*> &keyElemMap){

    if (root) {
        keyElemMap[root->key] = root;
        addToMap(root->left, keyElemMap);
        if (!root->rightThread)
            addToMap(root->right, keyElemMap);
    }
}

// common copy code for thread copying
template <typename KEY, typename T>
void Map<KEY, T>::copyThread(Elem* &newRoot, Elem* origRoot){
    // construct the key-element map for new and orig tree
    map<KEY, Elem*> newKeyElemMap;
    map<KEY, Elem*> origKeyElemMap;
    addToMap(newRoot->left, newKeyElemMap);
    addToMap(origRoot->left, origKeyElemMap);

    // start at the last element in the tree, which threads to root
    typename std::map<KEY, Elem*>::reverse_iterator it = origKeyElemMap.rbegin();
    newKeyElemMap[it->first] -> rightThread = true;
    newKeyElemMap[it->first] -> right = newRoot;

    // then thread the rest of the tree backwardly
    it++;
    while(it != origKeyElemMap.rend()){
        if (it->second->rightThread){
            newKeyElemMap[it->first] -> rightThread = true;
            newKeyElemMap[it->first] -> right = newKeyElemMap[ origKeyElemMap[it->first]->right->key ];
        }
        it++;
    }
}


// destructor
template <typename KEY, typename T>
Map<KEY, T>::~Map()
{
    if( !_size ) //empty tree
        delete _root;
    else
    {
        destructCode( _root->left );
        delete _root;
    }

}

// assignment operator : produces a deep copy of the threaded tree
template <typename KEY, typename T>
 Map<KEY, T> &Map<KEY, T>::operator=(const Map &rhs) {

    if( this == &rhs )
        return *this;

    if( _size )
    {
        destructCode(_root);
    }
        delete _root;
        _root = new Elem;
        _root->left = _root;
        _root->right = 0;
        copyCode( _root ->left, rhs._root->left );  // new & original map
        copyThread(_root, rhs._root); // to copy the threading; must pass dummy nodes to complete the threads
        _size = rhs._size;

    return *this;

}


// insert an element; return true if successful
template <typename KEY, typename T>
bool Map<KEY, T>::insert( KEY key, T data)
{

// This algorithm is independent of of the helper function
// " bool insert(Elem *& root, const KEY &key, const T &data, Elem *lastLeft);  "
//
//    Elem* newNode = new Elem;
//    newNode->key = k;
//    newNode->data = d;
//    newNode->rightThread = true;
//    newNode->right = 0;
//    newNode->left = 0;
//
//    if( _root->left == _root )
//    {
//        newNode->right = _root;
//        _root->left = newNode;
//    }
//    else
//    {
//        Elem* current = _root->left;
//        Elem* prevNode = _root;
//        while (current)
//        {
//            prevNode = current;
//
//            if( k > current->key)
//            {
//                if(current->rightThread)
//                    break;
//                else
//                    current = current->right;
//            }
//            else if(k < current->key)
//            {
//                current = current->left;
//
//
//            }
//            else
//            {
//                delete newNode;
//                return false;
//            }
//        }
//
//        //add right
//        if(current)
//        {
//            newNode->right =prevNode->right;
//            prevNode->rightThread =false;
//            prevNode->right = newNode;
//
//        }
//        else
//        {
//            prevNode->left = newNode;
//            newNode->right = prevNode;
//        }
//
//    }
//
//
//    _size++;
//    return true;


/* Use this code below if you want to implement the helper function
 * "bool insert(Elem *& root, const KEY &key, const T &data, Elem *lastLeft); "
 *
 * otherwise comment it out and un-comment the above code.
 */

    if ( insert( _root->left, key, data, _root ) )
    {
        _size++;
        return true;
    }
    return false;

/*
 *
 *
 */
}


// remove an element; return true if successful
template <typename KEY, typename T>
bool Map<KEY, T>::erase(KEY k)
{

// GENERAL GUIDELINE(s):

//Key does not exist , return false


    //key does exist, return true.


        //swap node with inorder successor

        // delete node with the matching query key



    Iterator i = find( k );

    Iterator j( _root );

    if( i == j )   //nothing to erase, empty tree
    {
        return false;
    }

    Elem* p = _root;
    Elem* n = _root->left;
    bool isRight = 0; // isRight is false


    while( n->key != k ) //iterate through the left and right branches
    {
        if( k < n->key )
        {
            p = n;
            n = n->left;
            isRight = 0;
        }
        else
        {

            p = n;
            n = n->right;
            isRight = 1; //isRight is true
        }
    }
    if( ( n->left == 0 ) && ( n->rightThread ) )
    {
        if(isRight)
        {
            p->right = n->right;
            p->rightThread = 1;
            delete n;
            _size--;
            return true;
        }
        else
        {
            p->left = 0;
            delete n;
            _size--;
            return true;
        }
    }
    else if( ( ( n->left == 0 ) &&! ( n->rightThread ) ) || ( ( n->left != 0 )  && ( n->rightThread ) ) )
    {
        if( !n->rightThread )
        {

            p->right = n->right;
            delete n;
            _size--;
            return true;

        }

        else
        {

            p->right = n->left;
            Elem * t = n->left->right;
            while( !t->rightThread )
                t = t->right;
            t->right = n->right;
            delete n;
            _size--;
            return true;


        }
    }
    else if( (n->left != 0 ) &&! ( n->rightThread ) )
    {
        Elem* m = n->right;
        Elem* f = n;

        KEY tempKey;
        T tempVal;
        if( m->left == 0 )
        {
            tempKey = m->key;
            tempVal = m->data;
            m->key = n->key;
            m->data = n->data;
            n->key = tempKey;
            n->data = tempVal;
            f->right = m->right;
            f->rightThread = 1;
            delete m;
            _size--;
            return true;
        }
        while( m->left != 0 )
        {
            f = m ;
            m = m->left;
        }

        tempKey = m->key;
        tempVal = m->data;
        m->key = n->key;
        m->data = n->data;
        n->key = tempKey;
        n->data = tempVal;
        f->left = 0 ;
        delete m;
        _size--;
        return true;

    }



}
// return size of the Map
template <typename KEY, typename T>
int Map<KEY, T>::size() const{
    return _size;
}

// return an iterator pointing to the end if an element is not found,
// otherwise, return an iterator to the element
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::find(KEY k ) const {

    Elem* p =_root;
    Elem* r =_root;

    if( _size ) //if _size != 0
    {

        p = p->left;
    }
    else //empty tree
    {
        Iterator emp(r);
        return emp;
    }


    while( true ) //if _size != 0
    {
        if( p->key == k )   // the key of p->left is ==  ( query key "k" from parameter )
            break;
        else
        {
            if( k < p->key )
            {
                if( p->left == NULL )
                {
                    Iterator emp(r);
                    return emp;
                }
                p = p->left;
            }
            else
            {


                if( p->right == NULL || ( p->rightThread ) )
                {
                    Iterator emp(r);
                    return emp;
                }
                p = p->right;
            }
        }
    }
    Iterator it(p);
    return it;

}


// a simple Iterator, traverse the collection in one direction
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::end() const
{  // return the dummy root node

    Map::Iterator it ( _root );
    return it;
}

template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::begin() const {  // return the left most (smallest) tree node


    Elem *n = _root;

    if( _size ) //if _size is not zero
    {
        //What about right side?? Answer: Dummy is connected to left node and this left node is the real root
        while ( n->left != NULL )  // or != 0
        {
            n = n->left;
        }

    }
        Map::Iterator it(n);

    return it;

}
/*
 * A Map iterator that supports one-directional (++) iteration.
 * The Map is implemented as a sorted container so the iterator traverses all elements in order.
 * The container is a right-threaded tree that uses a dummy root sentinel.
 */
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::Iterator::operator++(int){


    if( _cur->rightThread )
    {
        _cur = _cur->right;
    }
    else
    {
        if( _cur->right != NULL )
        {
            _cur = _cur->right; // point to the next right
        }


        while( _cur->left != NULL )
        {
           _cur = _cur->left; // point to the next left
        }

    }

        return *this;

}

// overloaded subscript operator
template <typename KEY, typename T>
T& Map<KEY, T>::operator[](KEY k)
{

    T t = T(); // some generic type of variable name "t"

    Elem* e;

    Map::Iterator it = find( k );

    if( it !=  end() )  // k is not equal to dummy root node
    {
        e = &*it;
        return e -> data;  // add it as the data for the specified key in parameter
    }
    else
    {
        insert( k, t ); // insert as a new node with key and data
        it = find(k);
        return it -> data; // add it as the data for the specified key in parameter
    }

}
template <typename KEY, typename T>
typename Map<KEY, T>::Elem& Map<KEY, T>::Iterator::operator*()
{
    return *_cur; // de-reference current element

}

template <typename KEY, typename T>
typename Map<KEY, T>::Elem* Map<KEY, T>::Iterator::operator->()
{

    return _cur; // current element
}


template<class KEY, class T>
bool Map<KEY, T>::Iterator::operator==(Map::Iterator it)
{


    if ( _cur == it._cur )
    {
        return true;
    }
    else
        return false;

    // same as " return _cur == it._cur "
}
template <typename KEY, typename T>
bool Map<KEY, T>::Iterator::operator!=(Map::Iterator it)
{

    if ( _cur != it._cur )
    {
        return true;
    }
    else
        return false;

    // same as " return _cur != it._cur "

}

template <typename KEY, typename T>
bool Map<KEY, T>::insert(Map::Elem *&root, const KEY &key, const T &data, Map::Elem *lastLeft) {

    if (root == _root || root == 0 ) //or root == NULL
    {
        root = new Elem;
        root->key = key;
        root->data = data;
        root->left = 0;
        root->right = lastLeft;
        root->rightThread = true;
        return true;
    }

    if (key == root->key) // dont insert if Key already exists
    {
        return false;
    }
    // insert at left branch
    if (key < root->key)
        return insert( root->left, key, data, root);

    // insert at right branch
    if (! root->rightThread )  // if right Thread is false
    {
        return insert(root->right, key, data, lastLeft);

    }
    else
    {  //if current elem is right threaded then add a new node

        root->rightThread = false;
        root->right = new Elem;
        root->right->key = key;
        root->right->data = data;
        root->right->left = 0;
        root->right->right = lastLeft;
        root->right->rightThread = true;
        return true;
    }



}


template <typename KEY, typename T>
void Map<KEY, T>::destructCode(Map::Elem *&p)
{


    if( p->left != NULL )
    {
        destructCode( p->left );
    }
    if( p->right != NULL && ( !p->rightThread ) ) // p is not right-threaded
    {
        destructCode( p->right );
    }

    delete p;

}


// output the structure of tree. The tree is output as "lying down"
// in which _root is the LEFT most Elem.
template <typename KEY, typename T>
void Map<KEY, T>::printTree(ostream& out, int level, Elem *p) const
{
    int i;
    if (p)
    {
        if (p->right && !p->rightThread)
            printTree(out, level+1,p->right);
        for(i=0;i<level;i++)
        {
            out << "\t";
        }
        out << p->key << " " << p->data << '\n';
        printTree(out, level+1,p->left);
    }
}

// outputs information in tree in inorder traversal order
template <typename KEY, typename T>
ostream& Map<KEY, T>::dump(ostream& out) const
{
    if ( _root == _root->left)
    {// tree empty
        return out;
    }
    printTree(out, 0, _root->left);   // print tree structure
    return out;
}


// outputs using overloaded << operator
template<typename KEY, typename T>
ostream& operator<< (ostream& out, const Map<KEY, T>& v)
{
    v.dump(out);
    return out;
}

