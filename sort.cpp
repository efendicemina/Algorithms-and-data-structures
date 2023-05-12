#include<iostream>
#include <fstream>
using namespace std;
template<typename Tip>
void bubble_sort(Tip* niz, int vel){
    for (int i =vel-1; i>=1; i--) {
      for (int j=1; j<=i; j++) {
       if (niz[j-1]>niz[j]) {
       auto priv = niz[j-1];
       niz[j-1] = niz[j];
       niz[j] = priv;
       }
      }
    }
}

template<typename Tip>
void selection_sort(Tip* niz, int vel){
   for(int i=0; i<vel-1; i++){
       auto min=niz[i];
       int pmin=i;
       for(int j=i+1; j<vel; j++){
        if(niz[j]<min){
            min=niz[j];
            pmin=j;
        }
       }
       niz[pmin]=niz[i];
       niz[i]=min;
   }
}
template<typename Tip>
void zamjeni(Tip *niz, int i, int j){
    auto p=niz[i];
    niz[i]=niz[j];
    niz[j]=p;
}
template<typename Tip>
int particija(Tip*niz, int prvi, int zadnji){
    auto pi=niz[prvi];
    int p=prvi+1;
    while(p<=zadnji){
        if(niz[p]<pi) p++;
        else break;
    }
    for(int i=p+1; i<=zadnji; i++){
        if(niz[i]<pi){
            zamjeni(niz,p,i);
            p++;
        }
    }
    zamjeni(niz,prvi,p-1);
    return p-1;
}
template<typename Tip>
void quick_help(Tip* niz, int prvi,int zadnji){
    if(prvi<zadnji){
        int j=particija(niz,prvi,zadnji);
        quick_help(niz,prvi,j-1);
        quick_help(niz, j+1, zadnji);
    }
}
template<typename Tip>
void quick_sort(Tip* niz, int vel){
    quick_help(niz,0,vel-1);
}
template<typename Tip>
void merge(Tip* niz,int l, int p, int q, int u){
    int i=0, j=q-l, k=l;
    Tip *pom=new Tip[u-l+1];
    for(int m=0; m<=u-l; m++) pom[m]=niz[l+m];
    while(i<=p-l && j<=u-l){
        if(pom[i]<pom[j]) niz[k]=pom[i++];
        else niz[k]=pom[j++];
        k++;
    }
    while(i<=p-l) niz[k++]=pom[i++];
    while(j<=u-l) niz[k++]=pom[j++];
    delete [] pom;
}
template<typename Tip>
void merge_help(Tip* niz,int l, int u){
    if (u > l) {
   int p = (l + u - 1) / 2;
   int q = p + 1;
   merge_help<Tip>(niz,l, p);
   merge_help<Tip>(niz,q, u);
   merge<Tip>(niz,l, p, q, u);
  }
}
template<typename Tip>
void merge_sort(Tip* niz, int vel){
   merge_help<Tip>(niz, 0, vel-1);
}
void ucitaj(string filename, int*& niz, int &vel){
    ifstream inputFile(filename);
    int br=0;
    int i=0;
    while(true){
    if(inputFile.eof()) break;
    inputFile>>br;
       i++;
    }
    inputFile.close();
    vel=i;
    niz=new int[vel];
    ifstream file(filename);
    i=0;
    while(true){
        if(file.eof()) break;
        file>>br;
        niz[i]=br;
        i++;
    }
    file.close();
}
void generisi(string filename, int vel){
     ofstream file(filename);
     for(int i=0; i<vel; i++){
         file<<rand()%vel<<" ";
     }
}
template<typename Tip>
void TestnaFunkcija(string filename,Tip*niz,int vel, string izlaz ){
   ucitaj(filename, niz,vel);
   cout<<"Unesite: 1-> Bubble sort, 2-> Selection sort, 3-> Quick sort, 4-> Merge sort";
   int n;
   clock_t v1,v2;
   cin>>n;
   if(n==1){
     v1=clock();
    bubble_sort(niz, vel);
     v2=clock();
   }
   if(n==2){
     v1=clock();
    selection_sort(niz, vel);
     v2=clock();
   }
   if(n==3){
    v1=clock();
    quick_sort(niz, vel);
     v2=clock();
   }
   if(n==4){
    v1=clock();
    merge_sort(niz, vel);
     v2=clock();
   }
   else{
       cout<<"Pogresan unos!";
       return;
   }
   cout<<"Vrijeme trajanja sortiranja u milisekundama je: "<<v2-v1<<"\n";
   bool jel=true;
   for(int i=0; i<vel-1; i++){
       if(niz[i]>niz[i+1]) jel=false;
   }
   cout<<"Rezultat provjere (0 za netacno, 1 za tacno): "<<jel<<"\n";
   ofstream dat(izlaz);
   for(int i=0; i<vel; i++) izlaz<<niz[i];
   dat.close();
   delete[] niz;
}
int main(){

    return 0;
}
