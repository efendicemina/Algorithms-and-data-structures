#include <iostream>
using namespace std;
template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
  Mapa() {}
  virtual ~Mapa() {}
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;
  virtual TipVrijednosti operator[](TipKljuca k) const = 0;
  virtual TipVrijednosti &operator[](TipKljuca k) = 0;
};
template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
  pair<TipKljuca, TipVrijednosti> *niz;
  int duzina, kapacitet;

public:
  NizMapa() : kapacitet(1000), duzina(0) {
    niz = new pair<TipKljuca, TipVrijednosti>[kapacitet]();
  }
  NizMapa(const NizMapa &m);
  int brojElemenata() const override { return duzina; }
  ~NizMapa() {
    delete[] niz;
    duzina = 0, kapacitet = 0;
  }
  void obrisi() override;
  void obrisi(const TipKljuca &kljuc) override;
  TipVrijednosti operator[](TipKljuca k) const override;
  TipVrijednosti &operator[](TipKljuca k) override;
  NizMapa &operator=(const NizMapa &m);
};
template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(const NizMapa &m) {
  niz = new pair<TipKljuca, TipVrijednosti>[m.kapacitet];
  kapacitet = m.kapacitet;
  duzina = m.duzina;
  for (int i = 0; i < duzina; i++)
    niz[i] = m.niz[i];
}
template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi() {
  duzina = 0;
}
template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti
NizMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca k) const {
  for (int i = 0; i < duzina; i++)
    if (niz[i].first == k)
      return niz[i].second;
  return TipVrijednosti();
}
template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &NizMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca k) {
  for (int i = 0; i < duzina; i++)
    if (niz[i].first == k)
      return niz[i].second;
  if (duzina >= kapacitet) {
    kapacitet = kapacitet + 1000;
    auto pom = new pair<TipKljuca, TipVrijednosti>[kapacitet];
    for (int i = 0; i < duzina; i++)
      pom[i] = niz[i];
    delete[] niz;
    niz = pom;
  }
  duzina++;
  niz[duzina - 1].first = k;
  niz[duzina - 1].second = TipVrijednosti();
  return niz[duzina - 1].second;
}
template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
  for (int i = 0; i < duzina; i++) {
    if (niz[i].first == kljuc) {
      for (int j = i; j < duzina - 1; j++)
        niz[j] = niz[j + 1];
      duzina--;
      break;
    }
  }
}
template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti> &
NizMapa<TipKljuca, TipVrijednosti>::operator=(const NizMapa &m) {
  if (&m == this)
    return *this;
  delete[] niz;
  niz = new pair<TipKljuca, TipVrijednosti>[m.kapacitet];
  kapacitet = m.kapacitet;
  duzina = m.duzina;
  for (int i = 0; i < duzina; i++)
    niz[i] = m.niz[i];
  return *this;
}
template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
private:
  struct Cvor {
    TipKljuca kljuc;
    TipVrijednosti vrijednosti;
    Cvor *lijevi = nullptr;
    Cvor *desni = nullptr;
    Cvor *roditelj = nullptr;
    Cvor(TipKljuca k, TipVrijednosti v) {
      kljuc = k;
      vrijednosti = v;
    }
  };
  int velicina = 0;
  Cvor *stablo;

public:
  BinStabloMapa() {
    stablo = nullptr;
    velicina = 0;
  }
  BinStabloMapa(BinStabloMapa<TipKljuca, TipVrijednosti> &s) {
    stablo = nullptr;
    velicina = 0;
    int i = s.velicina;
    while (i != 0) {
      (*this)[s.stablo->kljuc] = s.stablo->vrijednosti;
      i--;
    }
    velicina = s.velicina;
  }
  ~BinStabloMapa() {
    if (velicina != 0)
      obrisi();
  }
  int brojElemenata() const override { return velicina; }
  void obrisi() override {
    while (stablo)
      obrisi(stablo->kljuc);
    stablo = nullptr;
    velicina = 0;
  }
  void obrisi(const TipKljuca &kljuc) override {
    auto trazi = stablo;
    Cvor *roditeljp = nullptr;
    Cvor *pp = nullptr, *pom = nullptr, *q = nullptr;
    while (trazi != nullptr && kljuc != trazi->kljuc) {
      q = trazi;
      if (trazi->kljuc < kljuc)
        trazi = trazi->desni;
      else
        trazi = trazi->lijevi;
    }
    if (!trazi) {
      return;
    }
    if (!trazi->lijevi)
      roditeljp = trazi->desni;
    else if (!trazi->desni)
      roditeljp = trazi->lijevi;
    else {
      pp = trazi;
      roditeljp = trazi->lijevi;
      pom = roditeljp->desni;
      while (pom) {
        pp = roditeljp;
        roditeljp = pom;
        pom = roditeljp->desni;
      }
      if (pp != trazi) {
        pp->desni = roditeljp->lijevi;
        roditeljp->lijevi = trazi->lijevi;
      }
      roditeljp->desni = trazi->desni;
    }
    if (!q)
      stablo = roditeljp;
    else if (trazi == q->lijevi)
      q->lijevi = roditeljp;
    else
      q->desni = roditeljp;
    delete trazi;
    velicina--;
  }
  TipVrijednosti operator[](TipKljuca k) const override {
    auto trazi = stablo;
    while (trazi != nullptr) {
      if (trazi->kljuc == k)
        return trazi->vrijednosti;
      else if (trazi->kljuc < k)
        trazi = trazi->desni;
      else
        trazi = trazi->lijevi;
    }
    return TipVrijednosti();
  }
  TipVrijednosti &operator[](TipKljuca k) override {
    auto trazi = stablo;
    auto roditeljp = stablo;
    bool desni = true;
    while (trazi != nullptr) {
      if (trazi->kljuc == k)
        return trazi->vrijednosti;
      else if (trazi->kljuc < k) {
        roditeljp = trazi;
        trazi = trazi->desni;
        desni=true;
      } else {
        roditeljp = trazi;
        trazi = trazi->lijevi;
        desni = false;
      }
    }
    trazi = new Cvor(k, TipVrijednosti());
    trazi->roditelj = roditeljp;
    if (stablo == nullptr) {
      stablo = trazi;
      velicina++;
      return stablo->vrijednosti;
    }
    if (desni)
      roditeljp->desni = trazi;
    else
      roditeljp->lijevi = trazi;
    velicina++;
    return trazi->vrijednosti;
  }
  BinStabloMapa &operator=(const BinStabloMapa &m) {
    if (this == &m)
      return *this;
    obrisi();
    int i = m.velicina;
    while (i != 0) {
      (*this)[m.stablo->kljuc] = m.stablo->vrijednosti;
      i--;
    }
    velicina = m.velicina;
    return *this;
  }
};
int main() {
    BinStabloMapa<int, int> binmapa;
    NizMapa<int, int> nizmapa;
    clock_t b_dodavanje1, b_dodavanje2, n_dodavanje1, n_dodavanje2;
    for(int i=0; i<1000; i++){
        binmapa[i]=rand();
        nizmapa[i]=rand();
    }
    b_dodavanje1=clock();
    binmapa[1000]=rand();
    b_dodavanje2=clock();
    cout<<"Vrijeme potrebno za dodavanje novog elementa u binarnu mapu"<<
    " punu random vrijednosti:"<<b_dodavanje2-b_dodavanje1<<"\n";
    n_dodavanje1=clock();
    nizmapa[1000]=rand();
    n_dodavanje2=clock();
    cout<<"Vrijeme potrebno za dodavanje novog elementa u niz mapu"<<
    " punu random vrijednosti:"<<n_dodavanje2-n_dodavanje1<<"\n";
    clock_t b_pristup1, b_pristup2, n_pristup1, n_pristup2;
    b_pristup1=clock();
    binmapa[988];
    b_pristup2=clock();
    cout<<"Vrijeme potrebno za pristup elementu u binarnoj mapi"<<
    " punoj random vrijednosti:"<<b_pristup2-b_pristup1<<"\n";
    n_pristup1=clock();
    nizmapa[988];
    n_pristup2=clock();
    cout<<"Vrijeme potrebno za pristup elementu u niz mapi"<<
    " punoj random vrijednosti:"<<n_pristup2-n_pristup1<<"\n";
    //Primjetimo da za dodavanje novog elementa binarna mapa je dosta
    //efikasnija od niz mape, dok kod pristupa postojecem elementu
    //razlika nije toliko osjetna ali je niz mapa efikasnija
  return 0;
}
