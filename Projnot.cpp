#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Definim o structura Tranzactie care contine data, suma si tipul tranzactiei
struct Tranzactie
{
  string data;
  double suma;
  string tip; // depunere sau retragere
};

// Supraincarcam operatorul de inserție pentru a scrie o tranzactie intr-un fisier text
ostream &operator<<(ostream &out, const Tranzactie &t)
{
  out << t.data << "," << t.suma << "," << t.tip;
  return out;
}

// Supraincarcam operatorul de extracție pentru a citi o tranzactie dintr-un fisier text
istream &operator>>(istream &in, Tranzactie &t)
{
  getline(in, t.data, ','); // citim data pana la virgula
  in >> t.suma;             // citim suma
  in.ignore();              // sarim peste virgula
  getline(in, t.tip);       // citim tipul pana la sfarsitul liniei
  return in;
}

// Definim o clasa Client care contine numele, prenumele, CNP-ul si un vector de tranzactii
class Client
{
private:
  string nume, prenume, CNP;
  vector<Tranzactie> tranzactii;

public:
  // Constructorul clasei Client care primeste ca parametri numele, prenumele si CNP-ul
  Client(string n, string p, string c)
  {
    nume = n;
    prenume = p;
    CNP = c;
  }

  // O metoda care adauga o tranzactie la vectorul de tranzactii al clientului
  void adauga_tranzactie(Tranzactie t)
  {
    tranzactii.push_back(t);
  }

  // O metoda care afiseaza datele si tranzactiile clientului
  void afiseaza()
  {
    cout << "Nume: " << nume << endl;
    cout << "Prenume: " << prenume << endl;
    cout << "CNP: " << CNP << endl;
    cout << "Tranzactii: " << endl;
    for (int i = 0; i < tranzactii.size(); i++)
    {
      cout << tranzactii[i] << endl;
    }
  }

  // O metoda care scrie datele si tranzactiile clientului intr-un fisier text
  void scrie_in_fisier(string nume_fisier)
  {
    ofstream fout(nume_fisier); // deschidem fisierul pentru scriere
    if (fout.is_open())
    {                                                       // verificam daca fisierul s-a deschis cu succes
      fout << nume << "," << prenume << "," << CNP << endl; // scriem datele clientului pe prima linie
      for (int i = 0; i < tranzactii.size(); i++)
      { // scriem tranzactiile clientului pe liniile urmatoare
        fout << tranzactii[i] << endl;
      }
      fout.close(); // inchidem fisierul
    }
    else
    {
      cout << "Nu s-a putut deschide fisierul " << nume_fisier << endl;
    }
  }

  // O metoda care citeste datele si tranzactiile clientului dintr-un fisier text
  void citeste_din_fisier(string nume_fisier)
  {
    ifstream fin(nume_fisier); // deschidem fisierul pentru citire
    if (fin.is_open())
    {                             // verificam daca fisierul s-a deschis cu succes
      getline(fin, nume, ',');    // citim numele clientului pana la virgula
      getline(fin, prenume, ','); // citim prenumele clientului pana la virgula
      getline(fin, CNP);          // citim CNP-ul clientului pana la sfarsitul liniei
      tranzactii.clear();         // golim vectorul de tranzactii
      Tranzactie t;               // cream o variabila temporara de tip Tranzactie
      while (fin >> t)
      {                          // citim tranzactiile clientului din fisier
        tranzactii.push_back(t); // adaugam tranzactia la vectorul de tranzactii
      }
      fin.close(); // inchidem fisierul
    }
    else
    {
      cout << "Nu s-a putut deschide fisierul " << nume_fisier << endl;
    }
  }
};

// O functie care scrie numele fisierelor care contin datele clientilor intr-un fisier text
void scrie_nume_fisiere(string nume_fisier, vector<string> nume_fisiere)
{
  ofstream fout(nume_fisier); // deschidem fisierul pentru scriere
  if (fout.is_open())
  { // verificam daca fisierul s-a deschis cu succes
    for (int i = 0; i < nume_fisiere.size(); i++)
    { // scriem numele fisierelor pe cate o linie
      fout << nume_fisiere[i] << endl;
    }
    fout.close(); // inchidem fisierul
  }
  else
  {
    cout << "Nu s-a putut deschide fisierul " << nume_fisier << endl;
  }
}

// O functie care citeste numele fisierelor care contin datele clientilor dintr-un fisier text
vector<string> citeste_nume_fisiere(string nume_fisier)
{
  vector<string> nume_fisiere; // cream un vector gol de stringuri
  ifstream fin(nume_fisier);   // deschidem fisierul pentru citire
  if (fin.is_open())
  {              // verificam daca fisierul s-a deschis cu succes
    string nume; // cream o variabila temporara de tip string
    while (getline(fin, nume))
    {                               // citim numele fisierelor din fisier
      nume_fisiere.push_back(nume); // adaugam numele la vectorul de nume
    }
    fin.close(); // inchidem fisierul
  }
  else
  {
    cout << "Nu s-a putut deschide fisierul " << nume_fisier << endl;
  }
  return nume_fisiere; // returnam vectorul de nume
}

int main()
{
  // Cream doi clienti si le adaugam cateva tranzactii
  Client c1("Popescu", "Ion", "1234567890123");
  c1.adauga_tranzactie(Tranzactie{"01.01.2024", 1000, "depunere"});
  c1.adauga_tranzactie(Tranzactie{"05.01.2024", 500, "retragere"});
  c1.adauga_tranzactie(Tranzactie{"10.01.2024", 200, "depunere"});

  Client c2("Ionescu", "Ana", "9876543210987");
  c2.adauga_tranzactie(Tranzactie{"02.01.2024", 1500, "depunere"});
  c2.adauga_tranzactie(Tranzactie{"06.01.2024", 300, "retragere"});