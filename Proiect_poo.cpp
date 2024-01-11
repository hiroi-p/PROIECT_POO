#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <regex>
using namespace std;
// ce simplu era daca foloseam listele de la inceput <3 <3
void Meniu_conectare();
void Meniu_Autentificat(string fisier);
void Meniu_principal();
void Meniu_contact();
void Meniu_contact(string fisier);
void Meniu_Rezervare();
void Meniu_Istoric();
void Meniu_Autentificare();
void Meniu_Inregistrare();
void Meniu_Rezervare(string fisier);
// Optiune folosim toate datele unui utilizator pe o linie(si impartim cu strtok).
// Putem sa folosim un delimitator la finalul istoricului, ex:linie,spatiu ca sa delimitam userii
// Putem sa numim in fisier inainte de nume sa scriem Nume: inainte de prenume sa scriem Prenume: si asa mai departe
int carti_stoc = 0;
class Carte
{
private:
    string nume;
    list<string> autori;
    string editura;
    string stare;

public:
    Carte(string nume, string editura)
    {
        this->nume = nume;
        this->editura = editura;
    }

    virtual int pret() = 0;
};

class Carte_fictiune : private Carte
{
    int nr_pag;

public:
    Carte_fictiune(string nume, string editura, int nr_pag) : Carte(nume, editura)
    {
        this->nr_pag = nr_pag;
    }
    int pret() override
    {
        return 50000 + nr_pag;
    }
};
class Carte_non_fictiune : private Carte
{
    int an;

public:
    Carte_non_fictiune(string nume, string editura, int an) : Carte(nume, editura)
    {
        this->an = an;
    }
    int pret() override
    {
        return 50000 - 25 * an;
    }
};

class Tranzactie
{
private:
    string carte;         // sau carte- obiect
    string tip_achizitie; // aici am modificat
    int suma;

public:
    Tranzactie(string carte, string tip_achizitie, int suma)
    {
        this->carte = carte;
        this->tip_achizitie = tip_achizitie;
        this->suma = suma;
    }
};
class User
{
private:
    string nume, prenume, CNP, email, parola;
    list<Tranzactie *> istoric;

public:
    User(string nume, string prenume, string CNP, string email, string parola)
    {
        this->CNP = CNP;
        this->email = email;
        this->nume = nume;
        this->parola = parola;
        this->prenume = prenume;
    }
    void adaugaree(string carte, string tip_achizitie, int suma)
    {
        fstream f;
        f.open(nume + "_" + prenume, ios::out);
        Tranzactie *isto;
        isto = new Tranzactie(carte, tip_achizitie, suma);
        istoric.push_back(isto);
    }
    void afisare()
    {
        cout << "Nume:" << nume << endl;
        cout << "Prenume:" << prenume << endl;
        cout << "CNP:" << CNP << endl;
        cout << "Email:" << email << endl;
    }
    string getNume()
    {
        return nume;
    }
    string getPrenume()
    {
        return prenume;
    }
    void scriere_fisier(fstream &fout)
    {
        fout << nume << endl
             << prenume << endl
             << CNP << endl
             << email << endl
             << parola << endl;
        fout << "Istoric:" << endl;
    }
    // functia de autentificare se poate face aici
    // functia de resetare parola se poate face aici, precum toate celelalte functii care au legatura cu continutul fisierului
};
fstream &operator>>(fstream &fin, User *u)
{
    string nume, prenume, CNP, email, parola;
    fin >> nume >> prenume >> CNP >> email >> parola;
    getline(fin, nume);
    getline(fin, prenume);
    getline(fin, CNP);
    getline(fin, email);
    getline(fin, parola);

    u = new User(nume, prenume, CNP, email, parola);

    return fin;
}
fstream &operator<<(fstream &fout, User *u)
{
    u->scriere_fisier(fout);
    return fout;
}
void citire_lista(list<User *> &lista_Utilizatori)
{
    User *u;
    fstream f;
    f.open("Date_Utilizatori.txt", ios::in);
    while (f >> u)
    {
        lista_Utilizatori.push_back(u);
    }
    f.close();
}
void Meniu_Inregistrare()
{
    fstream f;
    User *u;
    string nume, prenume, CNP, email, parola;
    cout << "Nume: ";
    cin >> nume;
    cout << "Prenume: ";
    cin >> prenume;
    f.open(nume + "_" + prenume + ".txt", ios::out);
    cout << "CNP: ";
    cin >> CNP;
eticheta:
    try
    {
        cout << "Email: ";
        cin >> email;
        regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        if (!regex_match(email, emailRegex))
        {
            throw logic_error("Adresa de email nu respecta formatul.");
        }
    }
    catch (logic_error &e)
    {
        fstream flog;
        flog.open("log.txt", ios::app);
        flog << e.what() << endl;
        cout << e.what() << endl;
        flog.close();
        goto eticheta;
    }
etiketa:
    try
    {

        cout << "Parola: ";
        cin >> parola;
        if (parola.length() < 3)
        {
            throw logic_error("Parola prea scurta");
        }
    }
    catch (logic_error &e)
    {
        fstream flog;
        flog.open("log.txt", ios::app);
        flog << e.what() << endl;
        cout << e.what() << endl;
        flog.close();
        goto etiketa;
    }
    u = new User(nume, prenume, CNP, email, parola);
    f << u;
    f.close();
    fstream fi("Date_Utilizatori.txt", ios::app);
    fi << email << endl
       << nume + "_" + prenume << endl;
    fi.close();
    Meniu_Autentificat(nume + "_" + prenume + ".txt");
}
void Meniu_Autentificare()
{
    int ok = 0;
    string email, parola, linie;
    ifstream f;
    ifstream fi;
    fstream flog;

eticheta:

    try
    {
        cout << "Email: ";
        cin >> email;
        regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        if (!regex_match(email, emailRegex))
        {
            throw logic_error("Adresa de email nu respecta formatul.");
        }
    }
    catch (logic_error &e)
    {
        flog.open("log.txt", ios::app);
        flog << e.what() << endl;
        cout << e.what() << endl;
        flog.close();
        goto eticheta;
    }

etiketa:

    try
    {

        cout << "Parola: ";
        cin >> parola;
        if (parola.length() < 3)
        {
            throw logic_error("Parola prea scurta");
        }
    }
    catch (logic_error &e)
    {
        flog.open("log.txt", ios::app);
        flog << e.what() << endl;
        cout << e.what() << endl;
        flog.close();
        goto etiketa;
    }
    f.open("Date_Utilizatori.txt");
    while (getline(f, linie))
    {
        ok = 0;
        regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        if (regex_match(linie, emailRegex))
        {
            if (linie == email)
            {
                ok = 1;
                getline(f, linie);
                fi.open(linie + ".txt");
                string nume_fisier;
                nume_fisier = linie + ".txt";
                for (int i = 1; i <= 5; i++)
                {
                    getline(fi, linie);
                }
                if (parola == linie)
                {
                    cout << "Autentificare cu succes" << endl;
                    Meniu_Autentificat(nume_fisier);
                }
                else
                {
                    cout << "Date de autentificare eronate." << endl;
                    flog.open("log.txt", ios::app);
                    flog << "Date de autentificare eronate." << endl;
                    flog.close();
                    fi.close();
                    f.close();
                    goto eticheta;
                }
            }
        }
    }
    if (ok == 0)
    {
        flog.open("log.txt", ios::app);
        flog << "Date de autentificare eronate." << endl;
        cout << "Date de autentificare eronate" << endl;
        flog.close();
        f.close();
        goto eticheta;
    }
}

void Resetare_Parola()
{
    int ok = 0;
    string email, parola, linie, linie2;
    ifstream f;
    fstream fi;
    fstream flog;

eticheta:

    try
    {
        cout << "Email: ";
        cin >> email;
        regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        if (!regex_match(email, emailRegex))
        {
            throw logic_error("Adresa de email nu respecta formatul.");
        }
    }
    catch (logic_error &e)
    {
        flog.open("log.txt", ios::app);
        flog << e.what() << endl;
        cout << e.what() << endl;
        flog.close();
        goto eticheta;
    }
    f.open("Date_Utilizatori.txt");
    while (getline(f, linie))
    {
        ok = 0;
        regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        if (regex_match(linie, emailRegex))
        {
            if (linie == email)
            {
                ok = 1;
                string parola_noua;
                cout << "Parola noua este:";
                cin >> parola_noua;
                getline(f, linie);
                fi.open(linie + ".txt", ios::in);
                list<string> linii;
                while (getline(fi, linie2))
                    linii.push_back(linie2);
                fi.close();
                fi.open(linie + ".txt", ios::out);
                int aux = 1;
                for (auto i : linii)
                {
                    if (aux == 5)
                    {
                        fi << parola_noua << endl;
                    }
                    else
                    {
                        fi << i << endl;
                    }
                    aux++;
                }
                cout << "Parola a fost modificata" << endl;
                Meniu_Autentificat(linie + ".txt");
                break;
            }
        }
    }
    if (ok == 0)
    {
        flog.open("log.txt", ios::app);
        flog << "Email inexistent" << endl;
        cout << "Email inexistent" << endl;
        flog.close();
        f.close();
        goto eticheta;
    }
}
void Meniu_conectare()
{
    int opt;

    cout << "Alegeti optiunea: " << endl;
    cout << "1.Inregistrare" << endl;
    cout << "2.Autentificare" << endl;
    cout << "3.Resetare Parola" << endl;
    cout << "4.Inapoi" << endl;
    cin >> opt;
    switch (opt)
    {
    case 1:
        Meniu_Inregistrare();
        break;
    case 2:
        Meniu_Autentificare();
        break;
    case 3:
        Resetare_Parola();
        break;
    case 4:
        Meniu_principal();
        break;
    default:
        Meniu_conectare();
        break;
    }
}
void Meniu_contact()
{
    int opt;
    while (1)
    {
        cout << "Suport tehnic" << endl
             << "In cazul in care intalnit dificultati echipa de suport va va sta la dispozitie." << endl
             << "Tot ce trebuie să faceti este sa completati urmatorul formular in care trebuie descrisa problema." << endl
             << "Alegeti o optiune:" << endl
             << "     1. Trimitere sesizare" << endl
             << "     2. Inapoi" << endl
             << "Optiune: ";
        cin >> opt;
        switch (opt)
        {
        case 1:
            break;
        case 2:

            Meniu_principal();
            break;
        default:
            break;
        }
    }
}
void Meniu_Autentificat(string fisier)
{
    fstream f;
    string linie;
    int opt;
    int aux = 1;
    f.open(fisier, ios::in);
    getline(f, linie);
    cout << "Bookstore App"
         << " " << linie << " ";
    getline(f, linie);
    cout << linie << endl;
    f.close();
    int suma_totala = 0;
    while (1)
    {
        aux = 1;
        cout << "1.Rezervare" << endl;
        cout << "2.Istoric" << endl;
        cout << "3.Contact" << endl;
        cout << "Optiune: ";
        cin >> opt;
        switch (opt)
        {
        case 1:
            Meniu_Rezervare(fisier);
            break;
        case 2:
            suma_totala = 0;
            f.open(fisier, ios::in);
            while (getline(f, linie))
            {
                if (aux >= 6)
                {
                    if (aux % 5 == 0)
                    {
                        suma_totala = suma_totala + stoi(linie);
                    }
                    cout << linie << endl;
                }
                aux++;
            }
            cout << "Suma totala cheltuita:" << suma_totala << endl;
            f.close();
            break;
        case 3:
            Meniu_contact(fisier);
            break;
        default:
            cout << "optiune gresita" << endl;
            break;
        }
    }
}
void Meniu_Rezervare(string fisier)
{
    fstream f;
    string linie, nume, linie2;
    int opt;
    int aux = 1;
    f.open(fisier, ios::in);
    getline(f, linie);
    cout << "Bookstore App"
         << " " << linie << " ";
    getline(f, linie);
    cout << linie << endl;
    f.close();
    cout << "Cartile sunt:" << endl;
    fstream fi("Carti.txt", ios::in);
    while (getline(fi, linie))
    {
        cout << linie << endl;
    }
    fi.close();
    while (1)
    {
        int opt;
        cout << "1.Achizitie" << endl;
        cout << "2.Imprumut" << endl;
        cout << "3.Inapoi" << endl;
        cin >> opt;

        switch (opt)
        {
        case 1:
            cout << "Numele cartii dorite:";
            cin.ignore();
            getline(cin, nume);
            fi.open("Carti.txt", ios::in);
            while (getline(fi, linie))
            {
                if (linie == nume)
                {
                    getline(fi, linie);
                    fi.close();
                    if (linie == "Disponibil")
                    {
                        cout << "Carte achizitionata" << endl;
                        fi.open("Carti.txt", ios::in);
                        list<string> linii;
                        while (getline(fi, linie))
                            linii.push_back(linie);
                        fi.close();
                        fi.open("Carti.txt", ios::out);
                        auto i = find(linii.begin(), linii.end(), nume);
                        *next(i) = "Vandut";
                        for (auto i : linii)
                            fi << i << endl;
                        carti_stoc--;
                        fi.close();
                        f.open(fisier, ios::app);
                        fi.open("Carti.txt", ios::in);
                        while (getline(fi, linie))
                        {
                            if (linie == nume)
                            {

                                for (int i = 1; i <= 6; i++)
                                {
                                    if (i != 2)
                                        f << linie << endl;
                                    getline(fi, linie);
                                }

                                f.close();
                                fi.close();
                            }
                        }
                    }
                    else
                    {
                        cout << "Carte indisponibila" << endl;
                    }
                }
            }
            Meniu_Autentificat(fisier);
            break;
        case 2:
            cout << "Numele cartii dorite:";
            cin.ignore();
            getline(cin, nume);
            fi.open("Carti.txt", ios::in);
            while (getline(fi, linie))
            {
                if (linie == nume)
                {
                    getline(fi, linie);
                    fi.close();
                    if (linie == "Disponibil")
                    {
                        cout << "Carte imprumutata" << endl;
                        fi.open("Carti.txt", ios::in);
                        list<string> linii;
                        while (getline(fi, linie))
                            linii.push_back(linie);
                        fi.close();
                        fi.open("Carti.txt", ios::out);
                        auto i = find(linii.begin(), linii.end(), nume);
                        *next(i) = "Imprumutat";
                        for (auto i : linii)
                            fi << i << endl;
                        fi.close();
                        f.open(fisier, ios::app);
                        fi.open("Carti.txt", ios::in);
                        while (getline(fi, linie))
                        {
                            if (linie == nume)
                            {

                                for (int i = 1; i <= 6; i++)
                                {
                                    if (i != 2)
                                        if (i == 5)
                                            f << "0" << endl;
                                        else
                                            f << linie << endl;
                                    getline(fi, linie);
                                }

                                f.close();
                                fi.close();
                            }
                        }
                    }
                    else
                    {
                        cout << "Carte indisponibila" << endl;
                    }
                }
            }

            Meniu_Autentificat(fisier);
            break;
        case 3:
            Meniu_Autentificat(fisier);
            break;
        default:
            cout << "Optiune Gresita!";
            break;
        }
    }
}
void Meniu_principal()
{
    int opt;
    cout << "-------------------------------" << endl;
    cout << "Bookstore app" << endl
         << endl
         << endl;
    cout << "Alegeti o optiune: " << endl;
    cout << "1.Conectare" << endl;
    cout << "2.Contact" << endl;
    cout << "-------------------------------" << endl;
    cin >> opt;
    switch (opt)
    {
    case 1:
        Meniu_conectare();
        break;
    case 2:

        Meniu_contact();
        break;
    default:
        cout << "Optiune incorecta!   Mai alege-ti o data! " << endl;
        Meniu_principal();
        break;
    }
}
void Meniu_contact(string fisier)
{
    int opt;
    while (1)
    {
        cout << "Suport tehnic" << endl
             << "In cazul in care intalnit dificultati echipa de suport va va sta la dispozitie." << endl
             << "Tot ce trebuie să faceti este sa completati urmatorul formular in care trebuie descrisa problema." << endl
             << "Alegeti o optiune:" << endl
             << "     1. Trimitere sesizare" << endl
             << "     2. Inapoi" << endl
             << "Optiune: ";
        cin >> opt;
        switch (opt)
        {
        case 1:
            break;
        case 2:

            Meniu_Autentificat(fisier);
            break;
        default:
            break;
        }
    }
}
int main()
{
    Meniu_principal();
    return 0;
}
