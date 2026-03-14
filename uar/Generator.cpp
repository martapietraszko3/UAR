#include "Generator.h"
#include <cmath>
#include <numbers>

#ifndef M_PI
	constexpr double M_PI = 3.14159265358979323846;
#endif

double Generator::Generuj(int i) {

	int okres_dyskretny = static_cast<int>(round(m_okres_rzeczywisty * 1000.0 / m_interwal_symulacji)); // wyra¿ony w próbkach na okres

	double sygnal;
	if(m_typ_sygnalu == TypSygnalu::sinusoidalny) {
		sygnal = m_amplituda * sin((static_cast<double>(i % okres_dyskretny) / okres_dyskretny) * 2.0 * M_PI)+ m_skladowa_stala;//stala poza sinusem
	}
	else {
		if (i % okres_dyskretny < m_wypelnienie * okres_dyskretny) {
			sygnal = m_amplituda + m_skladowa_stala;	
		}
		else {
			sygnal = m_skladowa_stala;
		}
	}
	m_ostatni_sygnal = sygnal;
	return sygnal;
};

void Generator::Resetuj() {
	m_typ_sygnalu = TypSygnalu::prostakatny;
	m_okres_rzeczywisty=1;
	m_amplituda=0;
	m_skladowa_stala = 0;
	 m_wypelnienie=0.5;
	m_interwal_symulacji=200;

};

void Generator::ZmienTypSygnalu() {
	if (m_typ_sygnalu == TypSygnalu::sinusoidalny)
		m_typ_sygnalu = TypSygnalu::prostakatny;
	else
		m_typ_sygnalu = TypSygnalu::sinusoidalny;


};