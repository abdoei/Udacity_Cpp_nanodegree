template<class T>
class PtrDetails {
public:
    int refCount = 0;
    T memPtr;
    bool isArray;
    int arraySize;
    PtrDetails(T* ptr, int s = 0);
    ~PtrDetails() = default;
};

template<class T>
PtrDetails<T>::PtrDetails(T* ptr, int s = 0) : memPtr(ptr), arraySize(s) {
    ++refCount;
    isArray = s; // if s == 0 then isArray = false
}

template <class T>
bool operator==(const PtrDetails<T> &obj1, const PtrDetails<T> &obj2)
{return obj1.memPtr == obj2.memPtr; }
