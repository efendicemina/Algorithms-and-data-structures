#include <iostream>
using namespace std;
template <typename tip> class Stek {
  struct Cvor {
    tip el;
    Cvor *sljedeci;
  };
  Cvor *start;
  int velicina;
  void test() {
    if (velicina == 0)
      throw "Prazan!";
  }

public:
  Stek() : start(nullptr), velicina(0) {}
  Stek(const Stek &s);
  ~Stek() { brisi(); }
  int brojElemenata() { return velicina; }
  tip skini();
  tip &vrh() {
    test();
    return start->el;
  };
  void stavi(const tip &s);
  void brisi();
  Stek &operator=(const Stek &s);
};
template <typename tip> Stek<tip>::Stek(const Stek &s) {
  start = nullptr;
  velicina = 0;
  auto p = s.start;
  Cvor *pom = nullptr;
  while (p != nullptr) {
    auto pom2 = new Cvor{p->el, nullptr};
    if (velicina == 0) {
      start = pom2;
    } else {
      pom->sljedeci = pom2;
    }
    p = p->sljedeci;
    pom = pom2;
    velicina++;
  }
}
template <typename tip> tip Stek<tip>::skini() {
  test();
  auto p = start;
  tip ret = p->el;
  start = start->sljedeci;
  delete p;
  velicina--;
  return ret;
}
template <typename tip> void Stek<tip>::stavi(const tip &s) {
  start = new Cvor{s, start};
  velicina++;
}
template <typename tip> void Stek<tip>::brisi() {
  while (start != nullptr) {
    skini();
  }
  velicina = 0;
}
template <typename tip> Stek<tip> &Stek<tip>::operator=(const Stek &s) {
  if (this == &s)
    return *this;
  brisi();
  start = nullptr;
  auto p = s.start;
  Cvor *pom;
  while (p != nullptr) {
   auto  pom2 = new Cvor{p->el, nullptr};
    if (velicina == 0)
      start = pom2;
    else
      pom->sljedeci=pom2;
    pom=pom2;
    p = p->sljedeci;
    velicina++;
  }
  return *this;
}
//TESTOVI STEK
template<typename tip>
void TestKonstruktor(Stek<tip> s){
   if(s.brojElemenata()==0) cout<<"ok ";
   else cout<<"not ok ";
}
template<typename tip>
void TestKopirajuciK(Stek<tip> s1, Stek<tip> s2){
    s1.brisi();
    cout<<s2.vrh()<<" ";
}
template<typename tip>
void TestSkini(Stek<tip> s){
   auto p=s.vrh();
   s.skini();
   cout<<p<<" "<<s.vrh()<<" ";
}
template<typename tip>
void TestStavi(Stek<tip> s){
    s.stavi(0);
    cout<<s.vrh()<<" ";
}
template<typename tip>
void TestBrisi(Stek <tip> s){
    s.brisi();
    if(s.brojElemenata()==0) cout<<"ok ";
    else cout<<"not ok ";
}
template<typename tip>
void TestDodjela(Stek<tip> s1, Stek<tip> s2){
    s1=s2;
    cout<<s1.vrh()<<" "<<s2.vrh()<<" "<<s1.brojElemenata()<<" "<<s2.brojElemenata()<<" ";
}
int main() {
  cout<<"Unesite 4 clana steka: ";
  Stek<int> s1;
  TestKonstruktor(s1);
  int n;
for(int i=0; i<4; i++) {
    cin>>n;
    s1.stavi(n);
}
Stek<int> s2(s1);
TestKopirajuciK(s1,s2);
TestSkini(s2);
TestStavi(s2);
s1=s2;
TestDodjela(s1,s2);
TestBrisi(s2);
  return 0;
}
