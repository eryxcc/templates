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
#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <memory>

namespace easy {
  using std::cout;
  using std::cin;
  using std::endl;
  using std::make_pair;
  using std::pair;
  using std::ostream;
  using std::unique_ptr;
  using std::make_unique;
  using std::shared_ptr;
  using std::make_shared;
  using std::weak_ptr;

  
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
  using std::vector;
  using std::deque;
  using std::map;
  
  // writing and reading
  // C: printf("Czesc %s\n", kto);
  // Pascal: writeln('Czesc ', kto);
  // C++: cout << "Czesc " << kto << endl;
  // Easy: writeln('Czesc ', kto);

  void fwrite(std::ostream& os) {}
  template<class T> void fwriterange(std::ostream& os, const T& x);
  template<class T, class... TT> void fwrite(std::ostream& os, const T& x, TT... tt);

  template<class T> void fwrite(std::ostream& os, const T& x) {
    os << x;
    };
    
  template<class T, class U> void fwrite(std::ostream& os, const std::pair<T,U>& p) {
    fwrite(os, '(', p.first, ',', p.second, ')');
    }
  
  template<class T> void fwrite(std::ostream& os, const vector<T>& x) {
    fwriterange(os, x);
    };
    
  template<class T, class U> void fwrite(std::ostream& os, const map<T,U>& x) {
    fwriterange(os, x);
    };
    
  template<class T> void fwrite(std::ostream& os, const deque<T>& x) {
    fwriterange(os, x);
    };
    
  template<class T, int U> void fwrite(std::ostream& os, const array<T,U>& x) {
    fwriterange(os, x);
    };
  
  template<class T> void fwriterange(std::ostream& os, const T& x) {
    fwrite(os, "{"); 
    bool first = true;
    for(auto& el: x) { 
      if(first) 
        first = false;
      else 
        fwrite(os, ',');
      fwrite(os, el);
      }      
    fwrite(os, '}');
    };
  
  template<class T, class... TT> void fwrite(std::ostream& os, const T& x, TT... tt) {
    fwrite(os, x); fwrite(os, tt...);
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
  
  // a helper
  template <class T, class U> struct iterator_pair {
    T t;
    U u;
    iterator_pair(T _t, U _u) : t(_t), u(_u) {} 
    T& begin() { return t; }
    U& end() { return u; }
    const T& begin() const { return t; }
    const U& end() const { return u; }
   };

  template <class T, class U> iterator_pair<T,U> make_iterator_pair(T t, U u) { return iterator_pair<T,U>(t,u); }
  
  struct iterator_int {
    int value;
    const int& operator*() { return value; }
    iterator_int(int v) : value(v) {}
    };
  
  inline bool operator < (const iterator_int& i1, const iterator_int& i2) { return i1.value < i2.value; }
  inline bool operator > (const iterator_int& i1, const iterator_int& i2) { return i1.value > i2.value; }
  inline bool operator <= (const iterator_int& i1, const iterator_int& i2) { return i1.value <= i2.value; }
  inline bool operator >= (const iterator_int& i1, const iterator_int& i2) { return i1.value >= i2.value; }
  inline bool operator == (const iterator_int& i1, const iterator_int& i2) { return i1.value == i2.value; }
  inline bool operator != (const iterator_int& i1, const iterator_int& i2) { return i1.value != i2.value; }
  inline bool operator ++ (iterator_int& i) { i.value++; }
  
  inline iterator_pair<iterator_int, iterator_int> ints(int to) { return make_iterator_pair(iterator_int(0), iterator_int(to)); }
  inline iterator_pair<iterator_int, iterator_int> ints(int from, int to) { return make_iterator_pair(iterator_int(from), iterator_int(to)); }
  
  template<class T, class U, class... V> void sort(iterator_pair<T,U> p, V... v) { return std::sort(p.begin(), p.end(), v...); }
  template<class T, class U, class... V> void reverse(iterator_pair<T,U> p, V... v) { return std::reverse(p.begin(), p.end(), v...); }
  
  template<class T> iterator_pair<iterator_int, iterator_int> indices(const vector<T>& v) { return ints(v.size()); }
  template<class T> iterator_pair<iterator_int, iterator_int> indices(const deque<T>& v) { return ints(v.size()); }
  template<class T, int U> iterator_pair<iterator_int, iterator_int> indices(const array<T,U>& v) { return ints(v.size()); }

  // copied from: https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
  template<typename ... Args>
  string format( const std::string& format, Args ... args )
  {
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    }
     
  };
