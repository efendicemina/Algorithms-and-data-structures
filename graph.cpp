#include<iostream>
#include<limits>
#include<vector>
#include <queue>
using namespace std;
template <typename tip>
class Grana;
template <typename tip>
class Cvor;
template <typename tip>
class GranaIterator;

template <typename tip>
class UsmjereniGraf {
    protected:
    UsmjereniGraf() {}
    public:
    UsmjereniGraf(int) {}
    virtual ~UsmjereniGraf() {};

    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int i) = 0;

    virtual void dodajGranu(int i, int j, float k = 0) = 0;
    virtual void obrisiGranu(int i, int j) = 0;
    virtual void postaviTezinuGrane(int i, int j, float k= 0) = 0;
    virtual float dajTezinuGrane(int i, int j) const = 0;
    virtual bool postojiGrana(int i, int j)  const = 0;

    virtual void postaviOznakuCvora(int i, tip T) = 0;
    virtual tip dajOznakuCvora(int i) const = 0;
    virtual void postaviOznakuGrane(int i, int j, tip T) = 0;
    virtual tip dajOznakuGrane(int i, int j) const = 0;

    virtual Grana<tip> dajGranu(int i, int j) = 0;
    virtual const Grana<tip> dajGranu(int i, int j) const = 0;
    virtual Cvor<tip> dajCvor(int i) = 0;
    virtual const Cvor<tip> dajCvor(int i) const = 0;

    GranaIterator<tip> dajGranePocetak() {
        return ++GranaIterator<tip>(this, 0, -1);
    }
    const GranaIterator<tip> dajGranePocetak() const {
        return ++GranaIterator<tip>(const_cast<UsmjereniGraf*>(this), 0, -1);
    }

    GranaIterator<tip> dajGraneKraj() {
        return GranaIterator<tip>(this, dajBrojCvorova(), 0);
    }
    const GranaIterator<tip> dajGraneKraj() const {
        return GranaIterator<tip>(const_cast<UsmjereniGraf*>(this), dajBrojCvorova(), 0);
    }

};

template <typename tip>
class Grana {
    UsmjereniGraf<tip>* grafic;
    int polazni, dolazni;
    public:
    Grana(UsmjereniGraf<tip>* grafic, int polazni, int dolazni) : grafic(grafic),
    polazni(polazni), dolazni(dolazni) {}
    float dajTezinu() const { return grafic->dajTezinuGrane(polazni, dolazni); }
    void postaviTezinu(float tezina) { grafic->postaviTezinuGrane(polazni, dolazni, tezina); }
    tip dajOznaku() const { return grafic->dajOznakuGrane(polazni, dolazni); }
    void postaviOznaku(tip oznaka) { grafic->postaviOznakuGrane(polazni, dolazni, oznaka); }
    Cvor<tip> dajPolazniCvor() { return grafic->dajCvor(polazni); }
    const Cvor<tip> dajPolazniCvor() const { return grafic->dajCvor(polazni); }
    Cvor<tip> dajDolazniCvor() { return grafic->dajCvor(dolazni); }
    const Cvor<tip> dajDolazniCvor() const { return grafic->dajCvor(dolazni); }
};

template <typename tip>
class Cvor {
    UsmjereniGraf<tip>* grafic;
    int broj;
    public:
    Cvor(UsmjereniGraf<tip>* grafic, int broj) : grafic(grafic),
    broj(broj) {}
    tip dajOznaku() const { return grafic->dajOznakuCvora(broj); }
    void postaviOznaku(tip oznaka) { grafic->postaviOznakuCvora(broj, oznaka); }
    int dajRedniBroj() const { return broj; }
};

template <typename tip>
class GranaIterator {
    UsmjereniGraf<tip>* grafic;
    int i, j;
    void inkrementuj(int &i, int &j) {
        j + 1 >= grafic->dajBrojCvorova() ? j = 0, i++ : j++;
    }
    public:
    GranaIterator(UsmjereniGraf<tip>* grafic, int i, int j) :
    grafic(grafic), i(i), j(j) {}
    Grana<tip> operator*() { return grafic->dajGranu(i, j); }
    const Grana<tip> operator*() const { return grafic->dajGranu(i, j); }
    bool operator==(const GranaIterator &iter) const { return i == iter.i && j == iter.j && grafic == iter.grafic; }
    bool operator!=(const GranaIterator &iter) const { return !(*this == iter); }
    GranaIterator& operator++();
    GranaIterator operator++(int i);
};

template <typename tip>
GranaIterator<tip>& GranaIterator<tip>::operator++() {
    do
    inkrementuj(i, j);
    while (i < grafic->dajBrojCvorova() && !grafic->postojiGrana(i, j));
    return *this;
}

template <typename tip>
GranaIterator<tip> GranaIterator<tip>::operator++(int i) {
    auto kopija(*this);
    ++(*this);
    return kopija;
    }

    template <typename tip>
    class MatricaGraf : public UsmjereniGraf<tip> {
        const float INF = std::numeric_limits<float>::infinity();
        vector<vector<float>> m;
        vector<tip> oznakeCvorova;
        vector<vector<tip>> oznakeGrana;

        void validirajCvor(int i) const;
        void validirajGranu(int i, int j) const;
        void validirajPostojanjeGrane(int i, int j) const;
        public:
        MatricaGraf(int brojCvorova);

        int dajBrojCvorova() const { return m.size(); }
        void postaviBrojCvorova(int);

        void dodajGranu(int i, int j, float k = 0);
        void obrisiGranu(int i, int j);
        void postaviTezinuGrane(int i, int j, float k= 0);
        float dajTezinuGrane(int i, int j) const;
        bool postojiGrana(int i, int j) const;

        void postaviOznakuCvora(int, tip);
        tip dajOznakuCvora(int) const;
        void postaviOznakuGrane(int i, int j, tip T);
        tip dajOznakuGrane(int i, int j) const;

        Grana<tip> dajGranu(int i, int j);
        const Grana<tip> dajGranu(int i, int j) const;
        Cvor<tip> dajCvor(int i);
        const Cvor<tip> dajCvor(int i) const;
    };

    template <typename tip>
    void MatricaGraf<tip>::validirajCvor(int indeks) const {
        if (indeks < 0 || indeks >= dajBrojCvorova())
        throw "Neispravan broj cvora";
    }

    template <typename tip>
    void MatricaGraf<tip>::validirajGranu(int i, int j) const {
        validirajCvor(i);
        validirajCvor(j);
    }

    template <typename tip>
    void MatricaGraf<tip>::validirajPostojanjeGrane(int i, int j) const {
        validirajGranu(i, j);
        if (!postojiGrana(i, j))
        throw "Grana ne postoji";
    }

    template <typename tip>
    MatricaGraf<tip>::MatricaGraf(int brojCvorova) {
        if (brojCvorova < 0)
        throw "Broj cvorova ne moze biti negativan";
        m = vector<vector<float>>(brojCvorova, vector<float>(brojCvorova, INF));
        oznakeCvorova = vector<tip>(brojCvorova);
        oznakeGrana = vector<vector<tip>>(brojCvorova, vector<tip>(brojCvorova));
        }

        template <typename tip>
        void MatricaGraf<tip>::postaviBrojCvorova(int noviBrojCvorova) {
            if (noviBrojCvorova < dajBrojCvorova())
            throw "Novi broj cvorova ne moze biti manji od trenutnog";
            m.resize(noviBrojCvorova);
            for (auto& vf : m)
            vf.resize(noviBrojCvorova, INF);
            oznakeCvorova.resize(noviBrojCvorova);
            oznakeGrana.resize(noviBrojCvorova);
            for (auto& vt : oznakeGrana)
            vt.resize(noviBrojCvorova);
        }

        template <typename tip>
        void MatricaGraf<tip>::dodajGranu(int i, int j, float w) {
            validirajGranu(i, j);
            m[i][j] = w;
        }

        template <typename tip>
        void MatricaGraf<tip>::obrisiGranu(int i, int j) {
            validirajPostojanjeGrane(i, j);
            m[i][j] = INF;
        }

        template <typename tip>
        void MatricaGraf<tip>::postaviTezinuGrane(int i, int j, float w) {
            validirajPostojanjeGrane(i, j);
            m[i][j] = w;
        }

        template <typename tip>
        float MatricaGraf<tip>::dajTezinuGrane(int i, int j) const {
            validirajPostojanjeGrane(i, j);
            return m[i][j];
        }

        template <typename tip>
        bool MatricaGraf<tip>::postojiGrana(int i, int j) const {
            validirajGranu(i, j);
            return m[i][j] != INF;
        }

        template <typename tip>
        void MatricaGraf<tip>::postaviOznakuCvora(int i, tip oznaka) {
            validirajCvor(i);
            oznakeCvorova[i] = oznaka;
        }

        template <typename tip>
        tip MatricaGraf<tip>::dajOznakuCvora(int i) const {
            validirajCvor(i);
            return oznakeCvorova[i];
        }

        template <typename tip>
        void MatricaGraf<tip>::postaviOznakuGrane(int i, int j, tip T) {
            validirajPostojanjeGrane(i, j);
            oznakeGrana[i][j] = T;

        }

        template <typename tip>
        tip MatricaGraf<tip>::dajOznakuGrane(int i, int j) const {
            validirajPostojanjeGrane(i, j);
            return oznakeGrana[i][j];
        }

        template <typename tip>
        Grana<tip> MatricaGraf<tip>::dajGranu(int i, int j) {
            validirajPostojanjeGrane(i, j);
            return Grana<tip>(this, i, j);
        }

        template <typename tip>
        const Grana<tip> MatricaGraf<tip>::dajGranu(int i, int j) const {
            validirajPostojanjeGrane(i, j);
            return Grana<tip>(const_cast<MatricaGraf*>(this), i, j);
        }

        template <typename tip>
        Cvor<tip> MatricaGraf<tip>::dajCvor(int i) {
            validirajCvor(i);
            return Cvor<tip>(this, i);
        }

        template <typename tip>
        const Cvor<tip> MatricaGraf<tip>::dajCvor(int i) const {
            validirajCvor(i);
            return Cvor<tip>(const_cast<MatricaGraf*>(this), i);
        }

        template <typename tip>
        void dfs(UsmjereniGraf<tip> *grafic, vector<Cvor<tip> > &dfs_obilazak,
        Cvor<tip> cvor) {
            cvor.postaviOznaku(1);
            dfs_obilazak.push_back(cvor);
            for (GranaIterator<tip> it = grafic->dajGranePocetak(); it != grafic->dajGraneKraj(); ++it) {
                Cvor<tip> susjedni = (*it).dajDolazniCvor();
                if ((*it).dajPolazniCvor().dajRedniBroj() == cvor.dajRedniBroj() &&
                susjedni.dajOznaku() != 1)
                dfs(grafic, dfs_obilazak, susjedni);
            }
        }

        template <typename tip>
        void bfs(UsmjereniGraf<tip> *grafic, vector<Cvor<tip> > &bfs_obilazak,
        Cvor<tip> pocetni_cvor) {
            pocetni_cvor.postaviOznaku(1);
            bfs_obilazak.push_back(pocetni_cvor);
            queue<Cvor<tip>> red;
            red.push(pocetni_cvor);
            while (!red.empty()) {
                Cvor<tip> c = red.front();
                red.pop();
                for (GranaIterator<tip> it = grafic->dajGranePocetak(); it != grafic->dajGraneKraj(); ++it) {
                    Cvor<tip> susjedni = (*it).dajDolazniCvor();
                    if ((*it).dajPolazniCvor().dajRedniBroj() == c.dajRedniBroj() &&
                    susjedni.dajOznaku() != 1) {
                        susjedni.postaviOznaku(1);
                        bfs_obilazak.push_back(susjedni);
                        red.push(susjedni);
                    }
                    }

            }

        }
int main (){
    return 0;
}
