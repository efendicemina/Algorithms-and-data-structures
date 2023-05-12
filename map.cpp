#include<iostream>
#include<utility>
using namespace std;
template<typename TipKljuca, typename TipVrijednosti>
class Mapa{
  public:
  Mapa(){}
  virtual ~Mapa(){}
  virtual int brojElemenata() const=0;
  virtual void obrisi()=0;
  virtual void obrisi(const TipKljuca& kljuc)=0;
  virtual TipVrijednosti operator[](TipKljuca k)const=0;
  virtual TipVrijednosti &operator[](TipKljuca k)=0;
};
template<typename TipKljuca, typename TipVrijednosti>
class NizMapa: public Mapa<TipKljuca, TipVrijednosti>{
   pair<TipKljuca,TipVrijednosti> *niz;
   int duzina, kapacitet;
   public:
   NizMapa(): kapacitet(1000), duzina(0){niz=new pair<TipKljuca,TipVrijednosti>[kapacitet]();}
   NizMapa(const NizMapa &m);
   int brojElemenata()const override{return duzina;}
   ~NizMapa(){delete [] niz; duzina=0, kapacitet=0;}
   void obrisi()override;
   void obrisi(const TipKljuca& kljuc) override;
   TipVrijednosti operator[](TipKljuca k)const override;
   TipVrijednosti &operator[](TipKljuca k) override;
   NizMapa &operator=(const NizMapa &m);
};
template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(const NizMapa &m){
    niz=new pair<TipKljuca, TipVrijednosti> [m.kapacitet];
    kapacitet=m.kapacitet; duzina=m.duzina;
    for(int i=0; i<duzina; i++) niz[i]=m.niz[i];
}
template<typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>:: obrisi(){
  duzina=0;
}
template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti NizMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca k)const{
  for(int i=0; i<duzina; i++)
    if(niz[i].first==k) return niz[i].second;
return TipVrijednosti();
}
template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &NizMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca k){
   for(int i=0; i<duzina; i++)
    if(niz[i].first==k) return niz[i].second;
  if(duzina>=kapacitet){
      kapacitet=kapacitet+1000;
      auto pom=new pair<TipKljuca, TipVrijednosti> [kapacitet];
      for(int i=0; i<duzina; i++) pom[i]=niz[i];
      delete[] niz;
      niz=pom;
  }
  duzina++;
  niz[duzina-1].first=k;
  niz[duzina-1].second=TipVrijednosti();
  return niz[duzina-1].second;
}
template<typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca& kljuc){
  for(int i=0; i<duzina; i++){
      if(niz[i].first==kljuc){
          for(int j=i; j<duzina-1; j++) niz[j]=niz[j+1];
          duzina--;
          break;
      }
  }
}
template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti> &NizMapa<TipKljuca, TipVrijednosti>::operator=(const NizMapa &m){
   if(&m==this) return *this;
   delete[] niz;
   niz=new pair<TipKljuca, TipVrijednosti> [m.kapacitet];
   kapacitet=m.kapacitet; duzina=m.duzina;
   for(int i=0; i<duzina; i++) niz[i]=m.niz[i];
   return *this;
}
void Test1(){
   auto m=NizMapa<int, string>();
   int kljucevi[5]={1,2,3,4,5};
   for(int i=0; i<5; i++) m[i]="Emina";
   for(int i=0; i<m.brojElemenata(); i++)
   cout<<m[i]<<" ";
}
void Test2(){
   auto m=NizMapa<double, double>();
   for(int i=0; i<5; i++) m[i/3.]=i/2.;
   for(int i=0; i<m.brojElemenata(); i++) {
      double k=i/3.;
   cout<<m[k]<<" ";
   }
}
void Test3(){
   auto m=NizMapa<int, string>();
   int kljucevi[5]={1,2,3,4,5};
   for(int i=0; i<5; i++) m[i]="Emina";
   m.obrisi(3);
   for(int i=0; i<m.brojElemenata(); i++)
   cout<<m[i]<<" ";
}
void Test4(){
   auto m=NizMapa<int, int>();
   for(int i=0; i<7; i++) m[i]=i+3;
   auto m1(m);
   m.obrisi();
   for(int i=0; i<m1.brojElemenata(); i++)
   cout<<m1[i]<<" ";
}
void Test5(){
   auto m=NizMapa<int, char>();
   char c[4]={'a','b','c','d'};
   for(int i=0; i<4; i++) m[i]=c[i];
   auto m1=NizMapa<int, char>();
   m1=m;
   m.obrisi(0);
   for(int i=0; i<m1.brojElemenata(); i++)
   cout<<m1[i]<<" ";
}
int main(){
  Test1();
  Test2();
  Test3();
  Test4();
  Test5();
    return 0;
}
