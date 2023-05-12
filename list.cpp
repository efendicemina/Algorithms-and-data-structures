#include <iostream>
using namespace std;
template <typename tip> class Lista {
public:
  Lista() {}
  virtual ~Lista() {}
  virtual int brojElemenata() const = 0;
  virtual tip trenutni() const = 0;
  virtual tip &trenutni() = 0;
  virtual bool sljedeci() = 0;
  virtual bool prethodni() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIza(const tip &t) = 0;
  virtual void dodajIspred(const tip &t) = 0;
  virtual tip &operator[](int i) = 0;
  virtual tip operator[](int i) const = 0;
};
template <typename tip> class NizLista : public Lista<tip> {
  int kapacitet, velicina;
  tip **niz;
  int tekuci;

public:
  NizLista(int n = 100000);
  NizLista(const NizLista<tip> &n);
  NizLista(NizLista &&n);
  ~NizLista() {
    for (int i = 0; i < velicina; i++)
      delete niz[i];
    delete[] niz;
  }
  int brojElemenata() const override { return velicina; }
  tip trenutni() const override { return *niz[tekuci]; }
  tip &trenutni() override { return *niz[tekuci]; }
  bool sljedeci() override {
    if (tekuci < velicina - 1) {
      tekuci++;
      return true;
    }
    return false;
  }
  bool prethodni() override {
    if (tekuci != 0) {
      tekuci--;
      return true;
    }
    return false;
  }
  void pocetak() override { tekuci = 0; }
  void kraj() override { tekuci = velicina-1; }
  void obrisi() override;
  void dodajIza(const tip &t) override;
  void dodajIspred(const tip &t) override;
  NizLista &operator=(const NizLista &t);
  tip &operator[](int i) override { return *(niz[i]); }
  tip operator[](int i) const override { return *(niz[i]); }
};
template <typename tip>
NizLista<tip>::NizLista(int n) : kapacitet(n), velicina(0), tekuci(0) {
  niz = new tip *[kapacitet] {};
}
template <typename tip> NizLista<tip>::NizLista(NizLista &&n) {
  niz = n.niz;
  kapacitet = n.kapacitet;
  tekuci = n.tekuci;
  velicina = n.velicina;
}
template <typename tip> void NizLista<tip>::obrisi() {
  delete niz[tekuci];
  for (int i = tekuci; i < velicina - 1; i++)
    niz[i] = niz[i + 1];
  velicina--;
  if(tekuci!=0 && tekuci>=velicina) tekuci--;
}
template <typename tip> void NizLista<tip>::dodajIza(const tip &t) {
  if (velicina == 0) {
    niz[0] = new tip(t);
    velicina++;
  } else if (velicina < kapacitet) {
    for (int i = velicina; i > tekuci + 1; i--)
      niz[i] = niz[i - 1];
    niz[tekuci + 1] = new tip(t);
    velicina++;
  }
}
template <typename tip> void NizLista<tip>::dodajIspred(const tip &t) {
  if (velicina == 0) {
    niz[0] = new tip(t);
    velicina++;
  } else if (velicina < kapacitet) {
    for (int i = velicina; i > tekuci; i--)
      niz[i] = niz[i - 1];
    niz[tekuci] = new tip(t);
    velicina++;
    tekuci++;
  }
}
template <typename tip> NizLista<tip>::NizLista(const NizLista<tip> &n) {
  niz = new tip *[n.kapacitet] {};
  kapacitet = n.kapacitet;
  velicina = n.velicina;
  tekuci = 0;
  for (int i = 0; i < velicina; i++)
    niz[i] = new tip(*n.niz[i]);
}
template <typename tip>
NizLista<tip> &NizLista<tip>::operator=(const NizLista<tip> &t) {
  for (int i = 0; i < velicina; i++)
    delete niz[i];
  delete[] niz;
  niz = new tip *[t.kapacitet];
  kapacitet = t.kapacitet;
  velicina = t.velicina;
  tekuci = 0;
  for (int i = 0; i < velicina; i++)
    niz[i] = new tip(*t.niz[i]);
  return *this;
}
//TESTOVI ZA NIZLISTU
template <typename tip>
void TestKonstruktorBezParametara(const NizLista<tip> &n){
     if(n.brojElemenata()==0) cout<<"ok ";
     else cout<<"not ok ";
}
template <typename tip>
void TestKopirajuciKonstruktor(const NizLista<tip> n1, const NizLista<tip>n2){
    if(n1.brojElemenata()==n2.brojElemenata()) cout<<"ok ";
    else cout<<"not ok ";
}
template <typename tip>
void TestTrenutni( NizLista<tip> n1){
    n1.pocetak();
    if(n1.trenutni()==n1[0]) cout<<"ok ";
    else cout<<"not ok ";
    n1.kraj();
    if(n1.trenutni()==n1[n1.brojElemenata()-1]) cout<<"ok ";
     else cout<<"not ok ";
}
template <typename tip>
void TestSljedeciIIndeksiranje( NizLista<tip> n1,int n){
    n1.sljedeci();
    auto k=n1.trenutni();
    if(k==n1[n+1]) cout<<"ok ";
    else cout<<"not ok ";
}
template <typename tip>
void TestDodajIBrisi( NizLista<tip> n1, tip n){
    NizLista<tip> pom(n1);
    n1.dodajIza(n);
    n1.sljedeci();
    n1.obrisi();
    bool flag=true;
    for(int i=0; i<n1.brojElemenata(); i++){
        if(n1[i]!=pom[i]) flag=false;
    }
    if(flag) cout<<"ok";
    else cout<<"not ok";
}
template <typename tip> class JednostrukaLista : public Lista<tip> {
  struct Cvor {
    tip el;
    Cvor *sljedeci;
  };
  Cvor *start, *end, *tekuci;
  int velicina;

public:
  JednostrukaLista()
      : start(nullptr), end(nullptr), tekuci(nullptr), velicina(0) {}
  JednostrukaLista(const JednostrukaLista &t);
  JednostrukaLista(JednostrukaLista &&t);
  ~JednostrukaLista();
  int brojElemenata() const override { return velicina; }
  tip trenutni() const override {
    if (tekuci == nullptr && velicina > 0)
      return start->el;
    return tekuci->sljedeci->el;
  }
  tip &trenutni() override {
    if (tekuci == nullptr && velicina > 0)
      return start->el;
    return tekuci->sljedeci->el;
  }
  bool prethodni() override;
  void pocetak() override { tekuci = nullptr; }
  void kraj() override {
      auto p=start;
      while(p->sljedeci!=end)
      p=p->sljedeci;
      tekuci=p;
   }
  void dodaj(tip element);
  void dodajIza(const tip &t) override;
  void dodajIspred(const tip &t) override;
  void obrisi() override;
  bool sljedeci() override;
  JednostrukaLista &operator=(const JednostrukaLista &t);
  tip &operator[](int i) override;
  tip operator[](int i) const override;
};
template <typename tip> JednostrukaLista<tip>::~JednostrukaLista() {
  auto pom = start;
  while (pom != nullptr) {
    Cvor *p = pom;
    pom = pom->sljedeci;
    delete p;
  }
  tekuci = start = end = nullptr;
  velicina = 0;
}
template <typename tip>
JednostrukaLista<tip>::JednostrukaLista(const JednostrukaLista &t)
    : start(nullptr), end(nullptr), tekuci(nullptr), velicina(0) {
  auto p = t.start;
  while (p != nullptr) {
    dodaj(p->el);
    p = p->sljedeci;
  }
  velicina=t.velicina;
}
template <typename tip> void JednostrukaLista<tip>::dodaj(tip element) {
  auto p = new Cvor{element, nullptr};
  if (velicina == 0)
    start = end = p;
  else
    end = end->sljedeci = p;
  velicina++;
}
template <typename tip> bool JednostrukaLista<tip>::prethodni() {
  if (tekuci == nullptr)
    return false;
  if (tekuci == start)
    tekuci = nullptr;
  else {
    auto p = start;
    while (p->sljedeci != tekuci)
      p = p->sljedeci;
    tekuci = p;
  }
  return true;
}
template <typename tip> void JednostrukaLista<tip>::dodajIspred(const tip &t) {
  auto p = new Cvor{t, nullptr};
  if (tekuci == nullptr) {
    if (start == nullptr)
      start = end = p;
    else {
      p->sljedeci = start;
      tekuci = p;
      start = p;
    }
  } else {
    p->sljedeci = tekuci->sljedeci;
    tekuci->sljedeci = p;
    tekuci = p;
  }
  velicina++;
}
template <typename tip> void JednostrukaLista<tip>::dodajIza(const tip &t) {
  auto p = new Cvor{t, nullptr};
  if (tekuci == nullptr) {
    if (start == nullptr)
      start = end = p;
    else {
      p->sljedeci = start->sljedeci;
      start->sljedeci = p;
      if (start == end)
        end = p;
    }
  } else {
    p->sljedeci = tekuci->sljedeci->sljedeci;
    tekuci->sljedeci->sljedeci = p;
    if (tekuci->sljedeci == end)
      end = p;
  }
  velicina++;
}
template <typename tip> void JednostrukaLista<tip>::obrisi() {
  Cvor *p;
  if (velicina <= 0)
    throw;
  if (tekuci == nullptr) {
    auto pom = start;
    start = start->sljedeci;
    delete pom;
  } else {
    if (tekuci->sljedeci == end) {
      auto p = tekuci;
      prethodni();
      if (tekuci == nullptr) {
        auto pom = start;
        start = start->sljedeci;
        delete pom;
      } else {
        delete end;
        end = tekuci->sljedeci;
        tekuci->sljedeci->sljedeci = nullptr;
      }
    } else {
      auto pom = tekuci->sljedeci;
      tekuci->sljedeci = tekuci->sljedeci->sljedeci;
      delete pom;
    }
  }
  velicina--;
}
template <typename tip> tip &JednostrukaLista<tip>::operator[](int i) {
  auto p = start;
  for (int n = 0; n < i; n++)
    p = p->sljedeci;
  return p->el;
}
template <typename tip> tip JednostrukaLista<tip>::operator[](int i) const {
  auto p = start;
  for (int n = 0; n < i; n++)
    p = p->sljedeci;
  return p->el;
}
template <typename tip> bool JednostrukaLista<tip>::sljedeci() {
  if (velicina <= 1)
    return false;
  if (tekuci == nullptr)
    tekuci = start;
  else if (tekuci->sljedeci == end)
    return false;
  else
    tekuci = tekuci->sljedeci;
  return true;
}
template <typename tip>
JednostrukaLista<tip> & JednostrukaLista<tip>::operator=(const JednostrukaLista &t){
      auto pom = start;
  while (pom != nullptr) {
    Cvor *p = pom;
    pom = pom->sljedeci;
    delete p;
  }
    tekuci=nullptr;
    auto p=t.start;
    while(p!=nullptr){
        dodajIza(p->el);
        sljedeci();
        p=p->sljedeci;
    }
    velicina=t.velicina;
    return *this;
}
//TESTOVI JEDNOSTRUKALISTA
template<typename tip>
void TestKonstruktorJL(JednostrukaLista<tip> l){
    if(l.brojElemenata()==0) cout<<"ok ";
    else cout<<"not ok ";
}
template<typename tip>
void TestKopKonstruktorJL(JednostrukaLista<tip> l1, JednostrukaLista<tip> l2){
    if(l1.brojElemenata() && l2.brojElemenata()) cout<<"ok ";
    else cout<<"not ok ";
}
template<typename tip>
void TestPrethodniSljedeciTrenutni(JednostrukaLista<tip> l){
   auto pom=l.trenutni();
   l.sljedeci();
   l.prethodni();
   if(pom==l.trenutni())cout<<"ok ";
    else cout<<"not ok ";
}
template<typename tip>
void TestIndeksiranjaJL(JednostrukaLista<tip> l){
    l.pocetak();
    if(l.trenutni()==l[0])cout<<"ok ";
    else cout<<"not ok ";
}
template<typename tip>
void TestObrisi(JednostrukaLista<tip> l){
    l.pocetak();
    auto pom=l.trenutni();
    l.obrisi();
    if(l.trenutni()!=pom)cout<<"ok ";
    else cout<<"not ok ";
}
int main() {
  cout<<"Unesite velicinu niz liste: ";
  int n;
  cin>>n;
  NizLista<int> n1;
  TestKonstruktorBezParametara(n1);
  cout<<"\nUnesite elemente: ";
  for(int i=0; i<n; i++){
      int a;
      cin>>a;
      n1.dodajIza(a);
  }
  NizLista<int> n2(n1);
  TestKopirajuciKonstruktor(n1,n2);
  TestTrenutni(n1);
  TestDodajIBrisi(n1,0);
   cout<<"\nUnesite velicinu jednostruke liste: ";
  cin>>n;
  JednostrukaLista<int> l1;
  TestKonstruktorJL(l1);
  cout<<"\nUnesite elemente: ";
  for(int i=0; i<n; i++){
      int a;
      cin>>a;
      l1.dodajIza(a);
  }
  JednostrukaLista<int> l2(l1);
  TestKopKonstruktorJL(l1,l2);
  TestPrethodniSljedeciTrenutni(l1);
  TestIndeksiranjaJL(l1);
  TestObrisi(l1);
  return 0;
}
