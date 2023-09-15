#pragma once
template<class T>
class PtrDetails {
public:
    int refCount;
    T* memPtr;
    bool isArray;
    int arraySize;
    // PtrDetails(T* ptr, int s = 0);
    ~PtrDetails() = default;
    PtrDetails(T *ptr, int s = 0);
};

template<class T>
PtrDetails<T>::PtrDetails(T* ptr, int s)
    : refCount(1), memPtr(ptr), isArray(s), arraySize(s)
{ /* if s == 0 then isArray = false */ }

template <class T>
bool operator==(const PtrDetails<T> &obj1, const PtrDetails<T> &obj2)
{return obj1.memPtr == obj2.memPtr; }
