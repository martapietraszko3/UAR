#include"Testy_SS.h"
#include"Testy_UAR.h"
void Testy_SerwisSymulacji::test_zmiana_generatora()
{
	cerr << "TEST SERWIS SYMULACJI - ZMIANA TYPU GENERATORA: ";
	try
	{
		SerwisSymulacji serwis;
		serwis.set_amplituda(10);
		double amplituda1 = serwis.get_amplituda();
		serwis.set_amplituda(50);
		double amplituda2 = serwis.get_amplituda();
		if (amplituda1 == 10 && amplituda2 == 50)
			cerr << "OK!" << endl;
		else
		{
			cerr << "FAIL!" << endl;
			cerr << "Oczekiwany wynik = 10, 50" << endl;
			cerr << "Finalny wynik: " << amplituda1 << ", " << amplituda2 << endl;
		}
			
	}
	catch (...)
	{
		cerr << "Inny blad" << endl;
	}


}
void Testy_SerwisSymulacji::test_reset_regulatora()
{
	cerr << "TEST SERWIS SYMULACJI - RESET REGULATORA: ";
	try
	{
		SerwisSymulacji serwis;
		serwis.set_wzmocnienie_k(0.0);
		serwis.set_stala_calkowania(1.0);
		serwis.setL_tryb_calkowania(RegulatorPID::LiczCalk::Zew);
		serwis.set_a({ 0.0 });
		serwis.set_b({ 1.0 });
		serwis.wykonajKrok(10.0);
		serwis.wykonajKrok(10.0);
		double i_przed = serwis.get_wartosc_i();
		serwis.reset_pid();
		serwis.wykonajKrok(10.0);
		double i_po = serwis.get_wartosc_i();
		if (abs(i_przed) > 0.1 && abs(i_po) < 1e-6)
			cerr << "OK!\n";
		else
			cerr << "FAIL! (Przed: " << i_przed << ", Po: " << i_po << ")\n";
	}
	catch (...) 
	{ 
		cerr << "Inny blad" << endl;
	}
	

}
void Testy_SerwisSymulacji::test_uar_prostokatny_zmiana_amplitudy()
{
	cerr << "TEST SERWIS SYMULACJI - UAR ZMIANA AMPLITUDY PRZY GENERATORZE PROSTOKATNYM: ";
	try
	{
		SerwisSymulacji serwis;
		serwis.set_typ_sygnalu(TypSygnalu::prostakatny);
		serwis.set_amplituda(10);
		serwis.set_stala_skladowa(0);
		serwis.set_wzmocnienie_k(1.0);
		constexpr int liczba_iteracji = 6;
		vector<double> spodz_wyn(liczba_iteracji);
		vector<double> fin_wyn(liczba_iteracji);
		spodz_wyn = { 0.0, 10.0, 10.0, -10.0, -10.0, 10.0 };
		Generator gen_pomocniczy(TypSygnalu::prostakatny, 1.0, 10.0, 0.0, 250, 0.5);
		for (int i = 0; i < liczba_iteracji; i++)
		{
			if (i == 2)
			{
				serwis.set_amplituda(20.0);
				gen_pomocniczy.set_amplituda(20.0);
			}
			double w = gen_pomocniczy.Generuj(i);
			fin_wyn[i] = serwis.wykonajKrok(w);
		}
		myAssert(spodz_wyn, fin_wyn);
	}
	catch (...)
	{
		cerr << "Inny blad" << endl;
	}


}
void Testy_SerwisSymulacji::test_uar_sinsuoida_zmiana_stalej_skladowej()
{
	cerr << "TEST SERWIS SYMULACJI - UAR ZMIANA STALEJ SKLADOWEJ PRZY GENERATORZE SINOSOIDALNYM: ";
	try
	{
		SerwisSymulacji serwis;
		serwis.set_typ_sygnalu(TypSygnalu::sinusoidalny);
		serwis.set_amplituda(10);
		serwis.set_stala_skladowa(0);
		serwis.set_wzmocnienie_k(1.0);
		constexpr int liczba_iteracji = 5;
		vector<double> spodz_wyn(liczba_iteracji);
		vector<double> fin_wyn(liczba_iteracji);
		Generator gen_pomocniczy(TypSygnalu::sinusoidalny, 1.0, 10.0, 0.0, 250, 0.5);
		spodz_wyn = { 0.0, 0.0, 10.0, 5.0, -10.0 };
		for (int i = 0; i < liczba_iteracji; i++)
		{
			if (i == 2)
			{
				serwis.set_stala_skladowa(5);
				gen_pomocniczy.set_stala_skladowa(5.0);
			}
			double w = gen_pomocniczy.Generuj(i);
			fin_wyn[i] = serwis.wykonajKrok(w);
				
		}
		myAssert(spodz_wyn, fin_wyn);
	}
	catch (...)
	{
		cerr << "Inny blad" << endl;
	}
}
void Testy_SerwisSymulacji::wykonaj_testy()
{
	test_zmiana_generatora();
	test_reset_regulatora();
	test_uar_prostokatny_zmiana_amplitudy();
	test_uar_sinsuoida_zmiana_stalej_skladowej();
}