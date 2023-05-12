#include <iostream>
using namespace std;
template <typename tip> class Red {
  struct Cvor {
    tip el;
    Cvor *sljedeci;
  };
  int velicina;
  Cvor *start, *end;
  void prazan() {
    if (velicina == 0)
      throw "Izuzetak";
  }

public:
  Red() : velicina(0), start(nullptr), end(nullptr) {}
  Red(const Red<tip> &r);
  ~Red();
  void brisi();
  void stavi(const tip &el);
  tip skini();
  tip &celo() {
    prazan();
    return start->el;
  };
  int brojElemenata() { return velicina; }
  Red &operator=(const Red &r);
};
template <typename tip>
Red<tip>::Red(const Red<tip> &r) : velicina(0), start(nullptr), end(nullptr) {
  auto p = r.start;
  while (p != nullptr) {
    stavi(p->el);
    p = p->sljedeci;
  }
}
template <typename tip> Red<tip>::~Red() { brisi(); }
template <typename tip> void Red<tip>::brisi() {
  if (velicina > 0)
    while (start != nullptr)
      skini();
}
template <typename tip> void Red<tip>::stavi(const tip &el) {
  auto p = new Cvor{el, nullptr};
  if (start == nullptr)
    start = end = p;
  else {
    end->sljedeci = p;
    end = p;
  }
  velicina++;
}
template <typename tip> tip Red<tip>::skini() {
  prazan();
  auto p = start;
  auto ret = start->el;
  start = start->sljedeci;
  if (start == nullptr)
    end = nullptr;
  velicina--;
  delete p;
  return ret;
}
template <typename tip> Red<tip> &Red<tip>::operator=(const Red &r) {
  if (this == &r)
    return *this;
  brisi();
  auto p = r.start;
  while (p != nullptr){
    stavi(p->el);
    p=p->sljedeci;
  }
  return *this;
}
template <typename tip>
void TestKonstruktor(Red<tip> r){
    if(r.brojElemenata()==0) cout<<"ok ";
    else cout<<"not ok ";
}
template <typename tip>
void TestKopirajuciK(Red<tip> r1, Red<tip> r2){
    r1.brisi();
    cout<<r2.celo();
}
template <typename tip>
void TestSkini(Red<tip> r){
    auto p=r.celo();
    r.skini();
    cout<<p<<" "<<r.celo()<<" ";
}
template <typename tip>
void TestBrisi(Red<tip> r){
    r.brisi();
    if(r.brojElemenata()==0) cout<<"ok ";
    else cout<<"not ok ";
}
template <typename tip>
void TestCelo(Red<tip> r){
    auto p=r.celo();
    r.celo()=0;
    cout<<p<<" "<<r.celo()<<" ";
}
template <typename tip>
void TestStavi(Red<tip> r){
    r.stavi(0);
    cout<<r.celo()<<" ";
}
template <typename tip>
void TestDodjela(Red<tip> r1, Red<tip> r2){
    r1=r2;
    cout<<r1.celo()<<" "<<r2.celo()<<" ";
}
template <typename tip>
void TestBrojEl(Red<tip> r){
    cout<<r.brojElemenata()<<" ";
    r.skini();
    cout<<r.brojElemenata()<<" ";
}
int main() {
   cout<<"Unesite 4 clana reda: ";
   Red<int> r1;
   TestKonstruktor(r1);
   int n;
   for(int i=0; i<4; i++){
       cin>>n;
       r1.stavi(n);
   }
   Red<int> r2(r1);
   TestKopirajuciK(r1,r2);
   TestSkini(r2);
   TestStavi(r2);
   r1=r2;
   TestDodjela(r1,r2);
   TestBrisi(r2);
  return 0;
}
