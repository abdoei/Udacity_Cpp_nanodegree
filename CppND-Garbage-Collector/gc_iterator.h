// Exception thrown when an attempt is made to
// use an Iter that exceeds the range of the
// underlying object.
//
class OutOfRangeExc{
    // Add functionality if needed by your application.
    OutOfRangeExc() {
        std::cout << "Attempted Access is Out of Range" << std::endl;
    }
};


// An iterator-like class for cycling through arrays
// that are pointed to by GCPtrs. Iter pointers
// ** do not ** participate in or affect garbage
// collection. Thus, an Iter pointing to
// some object does not prevent that object
// from being recycled.
//
template <class T>
class Iter
{
    // current pointer value
    T *ptr;
    // points to element one past end
    T *end;
    // points to start of allocated array
    T *begin;
    // length of sequence (size)
    unsigned length;
public:
    Iter() : ptr(NULL), end(NULL), begin(NULL), length(0) {}
    Iter(T *p, T *first, T *last) : ptr(p), end(last),
                                    begin(first), length(last - first) {}
    // Return length of sequence to which this
    // Iter points.
    unsigned size() { return length; }
    // Return value pointed to by ptr.
    // Do not allow out-of-bounds access.
    T &operator*()
    {
        if ((ptr >= end) || (ptr < begin))
            throw OutOfRangeExc();
        return *ptr;
    }
    // Return address contained in ptr.
    // Do not allow out-of-bounds access.
    T *operator->()
    {
        if ((ptr >= end) || (ptr < begin))
            throw OutOfRangeExc();
        return ptr;
    }
    // Prefix ++. ++It
    Iter operator++()
    {
        ptr++;
        return *this;
    }
    // Prefix --.
    Iter operator--()
    {
        ptr--;
        return *this;
    }
    // Postfix ++.
    Iter operator++(int notused) // notused, unused or dummy is to diff bet ++it and it++
    {
        T *tmp = ptr;
        ptr++;
        return Iter<T>(tmp, begin, end);
    }
    // Postfix --.
    Iter operator--(int notused)
    {
        T *tmp = ptr;
        ptr--;
        return Iter<T>(tmp, begin, end);
    }
    // Return a reference to the object at the
    // specified index. Do not allow out-of-bounds
    // access.
    T &operator[](int i)
    {
        if ((i < 0) || (i >= (end - begin)))
            throw OutOfRangeExc();
        return ptr[i];
    }
    // Define the relational operators.
    bool operator==(Iter op2)
    {
        return ptr == op2.ptr;
    }
    bool operator!=(Iter op2)
    {
        return ptr != op2.ptr;
    }
    bool operator<(Iter op2)
    {
        return ptr < op2.ptr;
    }
    bool operator<=(Iter op2)
    {
        return ptr <= op2.ptr;
    }
    bool operator>(Iter op2)
    {
        return ptr > op2.ptr;
    }
    bool operator>=(Iter op2)
    {
        return ptr >= op2.ptr;
    }
    // Subtract an integer from an Iter.
    Iter operator-(int n)
    {
        ptr -= n;
        return *this;
    }
    // Add an integer to an Iter.
    Iter operator+(int n)
    {
        ptr += n;
        return *this;
    }
    // Return number of elements between two Iters.
    int operator-(Iter<T> &itr2)
    {
        return ptr - itr2.ptr;
    }
};