#include <iostream>

#include "mainwindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    /*
	Testy_Generator testy;
	cout << "=== TESTY GENERATOR ===" << endl;
	cout << "TEST SYGNAL PROSTOKATNY: ";
	testy.test_typ_prostokatny();
	cout << "TEST SYGNAL SINOSUIDALNY: ";
	testy.test_typ_sinusoidy();
	cout << "TEST ZMIANY SYGNALU: ";
	testy.test_zmiana_sygnalu();
	cout << "TEST RESET GENERATORA: ";
	testy.test_reset_generatora();
	std::cout << "=== TESTY ModelARX ===" << std::endl;
	TESTY_ModelARX::wykonaj_testy();
	std::cout << std::endl;
	


	std::cout << "=== TESTY RegulatorPID ===" << std::endl;
	TESTY_RegulatorPID::wykonaj_testy();
	std::cout << std::endl;

	
	std::cout << "=== TESTY ProstyUAR (UkladRegulacji) ===" << std::endl;
	TESTY_ProstyUAR::wykonaj_testy();
	std::cout << std::endl;
	std::cout << "=== TESTY SERWIS SYMULACJI ===" << std::endl;
	Testy_SerwisSymulacji t;
	t.wykonaj_testy();
	//wszystkie testy dzialaj¹, jak pojawia sie problem z linkerem, z roznych powodow, to nalezy wyczyscic projekt i go ponownie skompilowac
*/
    QApplication a(argc, argv);
    MainWindow w;
    w.show();  // <--- To polecenie wyœwietla Twoje okno GUI
    return a.exec();
}
