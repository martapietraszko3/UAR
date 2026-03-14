#pragma once
#include"SerwisSymulacji.h"
#include<iostream>
using namespace std;
class Testy_SerwisSymulacji
{
public:
	void test_zmiana_generatora();
	void test_reset_regulatora();
	void test_uar_prostokatny_zmiana_amplitudy();
	void test_uar_sinsuoida_zmiana_stalej_skladowej();
	void wykonaj_testy();
};