#include <iostream>
#include <string>
#include <functional>
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
        desni = true;
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
template <typename TipKljuca, typename TipVrijednosti>
class Pair {
public:
    TipKljuca first;
    TipVrijednosti second;
    bool DEL;
    Pair() : DEL(false) {}
    Pair(const TipKljuca& first, const TipVrijednosti& second, bool DEL = false) : first(first), second(second), DEL(DEL) {}
};
template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
	static const int velicina = 1000000;
	int brEl, prostor;
	Pair<TipKljuca, TipVrijednosti> **niz, **kraj;
	function<unsigned int(const TipKljuca&, unsigned int)> h;
	unsigned int hash(const TipKljuca& kljuc, unsigned int i) const { return (h(kljuc, prostor) + i) % prostor; }
	int UmetniHash(const TipKljuca& k);
	bool ObrisiHash(const TipKljuca& k);
	int TraziHash(const TipKljuca& k) const;
	void Prosiri();
public:
	HashMapa() : brEl(0), prostor(velicina), niz(new Pair<TipKljuca, TipVrijednosti>*[prostor]{}), kraj(niz + prostor) {}
	HashMapa(const HashMapa& rhs);
	HashMapa(HashMapa&& rhs);
	~HashMapa();
	TipVrijednosti operator[](TipKljuca k) const override;
	TipVrijednosti &operator[](TipKljuca k) override;
	HashMapa& operator=(const HashMapa& rhs);
	HashMapa& operator=(HashMapa&& rhs);
	int brojElemenata() const override { return brEl; }
	void obrisi(const TipKljuca& kljuc) override;
	void obrisi() override;
	void definisiHashFunkciju(function<unsigned int(const TipKljuca&, unsigned int)> h) { this->h = h; }

};

template <typename TipKljuca, typename TipVrijednosti>
int HashMapa<TipKljuca, TipVrijednosti>::UmetniHash(const TipKljuca& k) {
	int j, i = 0;
	do {
		j = hash(k, i);
		if (niz[j] == nullptr || niz[j]->DEL) {
		    if(niz[j] != nullptr)
		        delete niz[j];
			niz[j] = new Pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti());
			brEl++;
			return j;
		}
		else
			i = i + 1;
	} while (i < prostor);
	return -1;
}

template <typename TipKljuca, typename TipVrijednosti>
bool HashMapa<TipKljuca, TipVrijednosti>::ObrisiHash(const TipKljuca& k) {
	int indeks(TraziHash(k));
	if (indeks != -1) {
		brEl--;
		niz[indeks]->DEL = true;
		return true;
	}
	return false;
}

template <typename TipKljuca, typename TipVrijednosti>
int HashMapa<TipKljuca, TipVrijednosti>::TraziHash(const TipKljuca& k) const {
	unsigned int indeks, i(0);
	do {
		indeks = hash(k, i);
		if (niz[indeks] && !niz[indeks]->DEL && niz[indeks]->first == k)
			return indeks;
		else
			i = i + 1;
	} while (niz[indeks] != nullptr && i != prostor);
	return -1;
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(const HashMapa& rhs) : brEl(rhs.brEl), prostor(rhs.prostor), niz(new Pair<TipKljuca, TipVrijednosti>*[prostor] {}), kraj(niz + prostor), h(rhs.h) {
	for (int i = 0; i < rhs.prostor; i++)
		if (rhs.niz[i]) {
			niz[i] = new Pair<TipKljuca, TipVrijednosti>(*rhs.niz[i]);
		}
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(HashMapa&& rhs) : brEl(rhs.brEl), prostor(rhs.prostor), niz(rhs.niz), kraj(rhs.kraj), h(rhs.h) {
	rhs.niz = rhs.kraj = nullptr;
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>& HashMapa<TipKljuca, TipVrijednosti>::operator=(const HashMapa& rhs) {
	if (this == &rhs)
		return *this;
	obrisi();
	delete[] niz;
	brEl = rhs.brEl;
	prostor = rhs.prostor;
	niz = new Pair<TipKljuca, TipVrijednosti>*[prostor] {};
	kraj = niz + prostor;
	h = rhs.h;
	for (int i = 0; i < rhs.prostor; i++)
		if (rhs.niz[i]) {
			niz[i] = new Pair<TipKljuca, TipVrijednosti>(*rhs.niz[i]);
		}
	return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>& HashMapa<TipKljuca, TipVrijednosti>::operator=(HashMapa&& rhs) {
	if (this == &rhs)
		return *this;
	obrisi();
	delete[] niz;
	brEl = rhs.brEl;
	prostor = rhs.prostor;
	niz = rhs.niz;
	kraj = rhs.kraj;
	h = rhs.h;
	rhs.niz = rhs.kraj = nullptr;
	return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::Prosiri() {
	Pair<TipKljuca, TipVrijednosti> **noviNiz = new Pair<TipKljuca, TipVrijednosti>*[brEl * 2]{};
	Pair<TipKljuca, TipVrijednosti> **it, **it2;
	for (it = niz, it2 = noviNiz; it != kraj; it++)
		if (*it != nullptr)
			(*it2++) = new Pair<TipKljuca, TipVrijednosti>(**it++);
	int brEl2(brEl * 2);
	obrisi();
	delete[] niz;
	prostor = brEl = brEl2;
	kraj = it2;
	niz = noviNiz;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& HashMapa<TipKljuca, TipVrijednosti>::operator[]( TipKljuca kljuc) {
	if (!h)
		throw "Hash funkcija nije definisana";
	int indeks(TraziHash(kljuc));
	if (indeks == -1)
		return niz[UmetniHash(kljuc)]->second;
	else
		return niz[indeks]->second;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti HashMapa<TipKljuca, TipVrijednosti>::operator[]( TipKljuca kljuc) const {
	if (!h)
		throw "Hash funkcija nije definisana";
	int indeks(TraziHash(kljuc));
	if (indeks == -1)
		return TipVrijednosti();
	else
		return niz[indeks]->second;
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca& kljuc) {
	if (!ObrisiHash(kljuc))
		throw "Kljuc nije pronadjen";
}


template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::obrisi() {
	for (Pair<TipKljuca, TipVrijednosti> **it = niz; it != kraj; it++) {
		delete *it;
		*it = nullptr;
	}
	brEl = 0;
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::~HashMapa() {
	obrisi();
	delete[] niz;
}
unsigned int hashic(int ulaz,
unsigned int max) {
	return 10;
}
int main() {
   BinStabloMapa<int, int> binmapa;
    NizMapa<int, int> nizmapa;
    HashMapa<int,int> hashmapa;
    hashmapa.definisiHashFunkciju(hashic);
    clock_t b_dodavanje1, b_dodavanje2, n_dodavanje1, n_dodavanje2,h_dodavanje1, h_dodavanje2;
    for(int i=1; i<1000; i++){
        binmapa[i]=rand();
        nizmapa[i]=rand();
        hashmapa[i]=rand();
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
    h_dodavanje1=clock();
    hashmapa[10]=rand();
    h_dodavanje2=clock();
    cout<<"Vrijeme potrebno za dodavanje novog elementa u hash mapu"<<
    " punu random vrijednosti:"<<h_dodavanje2-h_dodavanje1<<"\n";
    clock_t b_pristup1, b_pristup2, n_pristup1, n_pristup2,h_pristup1, h_pristup2;
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
    h_pristup1=clock();
    hashmapa[988];
    h_pristup2=clock();
    cout<<"Vrijeme potrebno za pristup elementu u hash mapi"<<
    " punoj random vrijednosti:"<<h_pristup2-h_pristup1<<"\n";
    clock_t b_brisanje1, b_brisanje2, n_brisanje1, n_brisanje2,h_brisanje1, h_brisanje2;
    b_brisanje1=clock();
    binmapa.obrisi(40);
    b_brisanje2=clock();
    cout<<"Vrijeme potrebno za brisanje elementa u binarnoj mapi"<<
    " punoj random vrijednosti:"<<b_brisanje2-b_brisanje1<<"\n";
    n_brisanje1=clock();
    nizmapa.obrisi(40);
    n_brisanje2=clock();
    cout<<"Vrijeme potrebno za brisanje elementa u niz mapi"<<
    " punoj random vrijednosti:"<<n_brisanje2-n_brisanje1<<"\n";
    h_brisanje1=clock();
    hashmapa.obrisi(10);
    h_brisanje2=clock();
    cout<<"Vrijeme potrebno za brisanje elementa u hash mapi"<<
    " punoj random vrijednosti:"<<h_brisanje2-h_brisanje1<<"\n";
    //Uporedimo perfromanse: vrijeme potrebno za dodavanje novog
    //elementa daleko najbrze u hashmapi, zatim slijedi niz pa binarna,
    //vrijeme potrebno za pristup elementu najbrze u niz mapi, zatim binarna
    //pa tek onda hash mapa, vrijeme potrebno za brisanje daleko najbolje
    //u hash mapi, zatim slijedi binarna pa niz mapa.
    //Zakljucak: svaka od navedenih mapa ima svoje prednosti i nedostatke,
    //te ovisno od zeljene primjene biramo onu koja nam najvise odgovara.
  return 0;
}
