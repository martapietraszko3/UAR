#include "Testy_Generator.h"
#include "Generator.h"
#include <cmath>
#include <iostream>
#include"Testy_UAR.h"
#include<vector>

void Testy_Generator::test_typ_prostokatny() { // prostok¹tny - stan niski
    try{
        Generator generator(TypSygnalu::prostakatny, 1.0, 10.0, 5.0, 250,0.5);
        constexpr int liczba_iteracji = 5;
        vector<double> spodz_wynik(liczba_iteracji);
        vector<double> fin_wynik(liczba_iteracji);
        spodz_wynik = { 15.0, 15.0, 5.0,5.0,15.0 };
        for (int i = 0; i < liczba_iteracji; i++)
            fin_wynik[i] = generator.Generuj(i);
        myAssert(spodz_wynik, fin_wynik);  
    }
    catch (...)
    {
        std::cerr << "Inny blad" << std::endl;
    }
       
	
}

void Testy_Generator::test_typ_sinusoidy()
{
    try {
        Generator generator(TypSygnalu::sinusoidalny, 1.0, 10.0, 5.0, 250, 0.5);

        constexpr int liczba_iteracji = 5;
        vector<double> spodz_wynik(liczba_iteracji);
        vector<double> fin_wynik(liczba_iteracji);
        spodz_wynik = { 5.0, 15.0, 5.0,-5.0,5.0 };
        for (int i = 0; i < liczba_iteracji; i++)
            fin_wynik[i] = generator.Generuj(i);
        myAssert(spodz_wynik, fin_wynik);
    }
    catch (...)
    {
        std::cerr << "Inny blad" << std::endl;
    }
}
void Testy_Generator::test_zmiana_sygnalu()
{
    try {
        Generator generator(TypSygnalu::sinusoidalny, 1.0, 10.0, 5, 250);
        constexpr int liczba_iteracji = 5;
        vector<double> spodz_wynik(liczba_iteracji);
        vector<double> fin_wynik(liczba_iteracji);
        spodz_wynik = { 5.0, 15.0, 5.0, 5.0, 15.0 };
        for (int i = 0; i < liczba_iteracji; i++)
        {

            fin_wynik[i] = generator.Generuj(i);
            if (i == 1)
                generator.ZmienTypSygnalu();
        }
        myAssert(spodz_wynik, fin_wynik);
    }
    catch(...)
    {
        std::cerr << "Inny blad" << std::endl;
    }
        
}
void Testy_Generator::test_reset_generatora()
{
    try {
        Generator generator(TypSygnalu::sinusoidalny, 1.0, 100.0, 5, 250);
        vector<double> spodz_wynik={0};
        vector<double> fin_wynik(1);
        generator.Resetuj();
        fin_wynik[0]=generator.Generuj(0);
        myAssert(spodz_wynik, fin_wynik);
    }
    catch (...)
    {
        std::cerr << "Inny blad" << std::endl;
    }
}
