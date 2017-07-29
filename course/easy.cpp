// "Easy C++"
// The point of Easy C++ is to adapt C++ for beginner programmers. 
// We do not use pure C++ to avoid more complex concepts such as Object Oriented
// Programming, namespaces, operator overloading, and templates. We do not use
// C to avoid some archaic constructions (preprocessor, automatic degeneration of
// arrays to pointers...) The syntax of Easy C++ is inspired by Pascal.

// usage: #include <easy.cpp>

#include <iostream>
#include <array>
#include <string>

namespace easy {
  using std::cout;
  using std::cin;
  using std::endl;
  
  // character strings
  // C: char* s;
  // Pascal: string
  // C++: std::string
  // Easy: string
  using std::string;
  using namespace std::string_literals;
  
  // length of a string
  // C: strlen(s)
  // Pascal: length(s)
  // C++: (int) s.size()
  // Easy: length(s)
  int length(const string& s) { return s.size(); }
    
  // arrays
  // C: int a[20];
  // Pascal: var a: Array[0..19] of Integer;
  // C++: std::array<int, 20> a;
  // Easy: array<int, 20> a;
  using std::array;
  
  // writing and reading
  // C: printf("Czesc %s\n", kto);
  // Pascal: writeln('Czesc ', kto);
  // C++: cout << "Czesc " << kto << endl;
  // Easy: writeln('Czesc ', kto);

  void fwrite(std::ostream& os) {}

  template<class T, class... TT> void fwrite(std::ostream& os, const T& x, TT... tt) {
    os << x; fwrite(os, tt...);
    };
    
  template<class... TT> void fwriteln(std::ostream& os, TT... tt) {
    fwrite(os, tt...); os << endl;
    };
    
  void fread(std::istream& os) {}
  
  template<class T, class... TT> void fread(std::istream& is, T& x, TT&... tt) {
    is >> x; fread(is, tt...);
    };

  template<class... TT> void writeln(TT... tt) { fwrite(cout, tt...); cout << endl; };
    
  template<class... TT> void write(TT... tt) { fwrite(cout, tt...);  };

  template<class... TT> void read(TT&... tt) { fread(cin, tt...);  };
    
  };
