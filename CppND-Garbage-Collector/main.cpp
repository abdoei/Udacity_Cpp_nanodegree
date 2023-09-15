#include "gc_pointer.h"
#include "LeakTester.h"
#include <iostream>

int main() {
  Pointer<int> p;
  p = new int(19);
  Pointer<int> p2 = p; Pointer<int> p3 = p;  Pointer<int> p4 = p;
  Pointer<std::string> str = new std::string("This string is allocated on the heap!");
  std::cout << *str;
  return 0;
}