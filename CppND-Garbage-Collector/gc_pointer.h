#pragma once
#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>
#include "gc_details.h"
#include "gc_iterator.h"
/*
    Pointer implements a pointer type that uses
    garbage collection to release unused memory.
    A Pointer must only be used to point to memory
    that was dynamically allocated using new.
    When used to refer to an allocated array,
    specify the array size.
*/
template <class T, int size = 0>
class Pointer{
private:
    // refContainer maintains the garbage collection list.
    static std::list<PtrDetails<T>> refContainer; // PS: it's a static member for a reason
    // addr points to the allocated memory to which
    // this Pointer pointer currently points.
    T *addr;
    /*  isArray is true if this Pointer points
        to an allocated array. It is false
        otherwise.
    */
    bool isArray;
    // true if pointing to array
    // If this Pointer is pointing to an allocated
    // array, then arraySize contains its size.
    unsigned arraySize; // size of the array
    static bool first; // true when first Pointer is created
    // Return an iterator to pointer details in refContainer.
    // typename is used to tell the compiler that std::list<PtrDetails<T>>::iterator is a type
    typename std::list<PtrDetails<T>>::iterator findPtrInfo(T *ptr);
public:
    // Define an iterator type for Pointer<T>.
    typedef Iter<T> GCiterator;
    // Empty constructor
    // NOTE: templates aren't able to have prototypes with default arguments
    // this is why constructor is designed like this:
    Pointer(){
        Pointer(NULL);
    }
    Pointer(T*);
    // Copy constructor.
    Pointer(const Pointer &);
    // Destructor for Pointer.
    ~Pointer();
    // Collect garbage. Returns true if at least
    // one object was freed.
    static bool collect();
    // Overload assignment of pointer to Pointer.
    T *operator=(T *t); // this is to allow method chaining like p1 = p2 = new int;
    // Overload assignment of Pointer to Pointer.
    Pointer &operator=(Pointer &rv);
    // Return a reference to the object pointed
    // to by this Pointer.
    T &operator*(){
        return *addr;
    } // to dereference the Pointer and access the element
    // Return the address being pointed to.
    T *operator->() { return addr; } // to enable using member fuctions of T using -> directly on a Pointer
    // Return a reference to the object at the
    // index specified by i.
    T &operator[](int i){ return addr[i];}
    // Conversion function to T *.
    operator T *() { return addr; } // to enable sth like int* x = Pointer<int>();
    // Return an Iter to the start of the allocated memory.
    Iter<T> begin(){
        int _size;
        if (isArray)
            _size = arraySize;
        else
            _size = 1;
        return Iter<T>(addr, addr, addr + _size);
    }
    // Return an Iter to one past the end of an allocated array.
    Iter<T> end(){
        int _size;
        if (isArray)
            _size = arraySize;
        else
            _size = 1;
        return Iter<T>(addr + _size, addr, addr + _size);
    }
    // Return the size of refContainer for this type of Pointer.
    static int refContainerSize() { return refContainer.size(); }
    // A utility function that displays refContainer.
    static void showlist();
    // Clear refContainer when program exits.
    static void shutdown();
};

// STATIC INITIALIZATION
// Creates storage for the static variables
// By doing this the the Pointer class ensures that the garbage collector is properly initialized before any Pointer objects are created. This allows the garbage collector to track all Pointer objects, regardless of when they were created.
template <class T, int size>
std::list<PtrDetails<T>> Pointer<T, size>::refContainer;
template <class T, int size>
bool Pointer<T, size>::first = true;

// Constructor for both initialized and uninitialized objects. -> see class interface
template<class T,int size>
Pointer<T,size>::Pointer(T *t) : addr(t), arraySize(size), isArray(size) {
    // Register shutdown() as an exit function.
    if (first)
        atexit(shutdown);
    first = false;
    auto p = findPtrInfo(t);
    if(p != refContainer.end()) p->refCount++;
    else{ refContainer.emplace_front(PtrDetails(t, size));}
}
// Copy constructor.
template< class T, int size>
Pointer<T,size>::Pointer(const Pointer &ob) : addr(ob.addr), isArray(ob.isArray), arraySize(ob.arraySize){
    findPtrInfo(ob.addr)->refCount++;
}

// Destructor for Pointer.
template <class T, int size>
Pointer<T, size>::~Pointer(){
    findPtrInfo(addr)->refCount--;
    // clean the refContainer from the unused pointers
    collect();
}

// Collect garbage. Returns true if at least
// one object was freed.
template <class T, int size>
bool Pointer<T, size>::collect(){
    // Note: collect() will be called in the destructor
    bool freedAnyMem = false;
    auto it = refContainer.begin();
    while (it != refContainer.end()){
        for(it = refContainer.begin();it != refContainer.end();it++){
            if(it->refCount > 0) continue;
            freedAnyMem = true;
            refContainer.remove(*it);
            if (it->isArray) delete[] it->memPtr;
            else             delete   it->memPtr;
            break;
        }
        // refresh the search
    }
    return freedAnyMem;
}

// Overload assignment of pointer to Pointer.
template <class T, int size>
T *Pointer<T, size>::operator=(T *t){
    //decrement the refCount of the current instance of pointer
    auto p = findPtrInfo(addr);
    p->refCount--;
    //find if the newly assigned is in the refContainer
    p = findPtrInfo(t);
    if(p != refContainer.end()) p->refCount++;
    else{
        PtrDetails<T>newInstance(t, size);
        refContainer.emplace_front(newInstance);
    }
    addr = t;
    isArray = arraySize = size;
    return t;
}
// Overload assignment of Pointer to Pointer.
template <class T, int size>
Pointer<T, size> &Pointer<T, size>::operator=(Pointer &rv){
    auto p = findPtrInfo(addr);
    p->refCount--;
    p = findPtrInfo(rv.addr);
    p->refCount++;
    addr = rv.addr;
    isArray = rv.isArray;
    arraySize = rv.arraySize;
    return rv;
}

// A utility function that displays refContainer.
template <class T, int size>
void Pointer<T, size>::showlist(){
    typename std::list<PtrDetails<T> >::iterator p;
    std::cout << "refContainer<" << typeid(T).name() << ", " << size << ">:\n";
    std::cout << "memPtr refcount value\n ";
    if (refContainer.begin() == refContainer.end())
    {
        std::cout << " Container is empty!\n\n ";
    }
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        std::cout << "[" << (void *)p->memPtr << "]"
             << " " << p->refcount << " ";
        if (p->memPtr)
            std::cout << " " << *p->memPtr;
        else
            std::cout << "---";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
// Find a pointer in refContainer.
template <class T, int size>
typename std::list<PtrDetails<T> >::iterator
Pointer<T, size>::findPtrInfo(T *ptr){
    typename std::list<PtrDetails<T> >::iterator p;
    // Find ptr in refContainer.
    for (p = refContainer.begin(); p != refContainer.end(); p++)
        if (p->memPtr == ptr)
            return p;
    return p;
}
// Clear refContainer when program exits.
template <class T, int size>
void Pointer<T, size>::shutdown(){
    if (refContainerSize() == 0)
        return; // list is empty
    typename std::list<PtrDetails<T> >::iterator p;
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        // Set all reference counts to zero
        p->refCount = 0;
    }
    collect();
}