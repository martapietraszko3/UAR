#pragma once

enum class TypSygnalu{
	sinusoidalny,
	prostakatny
};

class Generator {
private:
	TypSygnalu m_typ_sygnalu;
	int m_okres_rzeczywisty;//Trz
	double m_amplituda;//A
	double m_skladowa_stala;//S
	double m_wypelnienie;//p
	int m_interwal_symulacji;//Tt
	double m_ostatni_sygnal=0;

public:
	Generator(TypSygnalu typ, int okres_rzeczywisty, double amplituda, double skladowa_stala, int interwal_symulacji, double wypelnienie = 0.5):
		m_typ_sygnalu(typ), m_okres_rzeczywisty(okres_rzeczywisty), m_amplituda(amplituda), m_skladowa_stala(skladowa_stala),
		m_wypelnienie(wypelnienie), m_interwal_symulacji(interwal_symulacji)
	{
		if(wypelnienie>1 || wypelnienie<0)
			m_wypelnienie=0.5;
	}
	Generator()
		: m_typ_sygnalu(TypSygnalu::prostakatny),
		m_okres_rzeczywisty(1),
		m_amplituda(0.0),
		m_skladowa_stala(0.0),
		m_wypelnienie(0.5),
		m_interwal_symulacji(200),
		m_ostatni_sygnal(0.0)
	{
	}
	double Generuj(int i);
	void Resetuj();
	void ZmienTypSygnalu();
	friend class Testy_Generator;
	//setery i getery
	void set_typ_sygnalu(TypSygnalu typ) { m_typ_sygnalu = typ; }
	void set_okres_rzeczywisty(int okres) { m_okres_rzeczywisty = okres; }
	void set_amplituda(double amplituda) { m_amplituda = amplituda; }
	void set_stala_skladowa(double stala) { m_skladowa_stala = stala; }
	void set_wypelnienie(double wypelnienie) { m_wypelnienie = wypelnienie; }
	void set_interwal(int interwal) { m_interwal_symulacji = interwal; }
	double get_amplituda(){ return m_amplituda; }
	double get_okres_rzeczywisty() { return m_okres_rzeczywisty; }
	double get_stala_skladowa() { return m_skladowa_stala; }
	double get_wypelnienie() { return m_wypelnienie; }
	TypSygnalu get_typ() { return m_typ_sygnalu; }
	double get_sygnal() { return m_ostatni_sygnal; }
    double get_interwal() {return m_interwal_symulacji;}

};
