#pragma once

#include <vector>
#include <deque>
#include <random>
using namespace std;

class ModelARX
{
private:
	vector<double> m_a; // vector wspoczynnikow a
	vector<double> m_b; // vector wspoczynnikow b
	size_t m_k; // rzad oponienia transportowego
	double m_z; // zaklucenia
	mt19937 m_generator_losowy;
	normal_distribution<double> m_rozklad_gaussa;
	deque<double> m_buforU; // bufor probek sygnalu wejsciowego
	deque<double> m_buforY; // bufor probek sygnalu wejsciowego
	deque<double> m_buforOpoznioneU;  // bufor probek sygnalu wejsciowego oponionego o k
	//domyslne granice
	double m_max_u = 10.0;
	double m_min_u = -10.0;
	double m_max_y = 10.0;
	double m_min_y = -10.0;
	// przelaczniki domyslnie w³¹czone
	bool m_czy_ograniczenia_aktywne = true;
	bool m_czy_zaklocenia_aktywne = true;
public:
	ModelARX(vector<double> a, vector<double> b, size_t k=1, double z=0);
	double symuluj(double noweU);
	double get_yi() { return m_buforY[0]; }
	//setery i getery
	void set_a(vector<double> a) { m_a = a;m_buforY.resize(m_a.size(), 0);}
	void set_b(vector<double> b) { m_b = b; m_buforU.resize(m_b.size(), 0);}
	void set_opoznienie_k(size_t k) {m_k = k; m_buforOpoznioneU.resize(m_k, 0.0);}
    void set_z(double nowe_z)
    {
        m_z = nowe_z;
        double bezpieczne_z = (m_z > 0.0) ? m_z : 1.0;
        m_rozklad_gaussa = std::normal_distribution<double>(0.0, bezpieczne_z);
    }
	void set_limit_u(double gora, double dol) { m_max_u = gora; m_min_u = dol; }
	void set_limit_y(double gora, double dol) { m_max_y = gora; m_min_y = dol; }
	void przelacz_ograniczenia(bool stan) { m_czy_ograniczenia_aktywne = stan; }
	void przelacz_zaklocenia(bool stan) { m_czy_zaklocenia_aktywne = stan; }
	vector<double> get_a() { return m_a; }
	vector<double> get_b() { return m_b; }
	size_t get_oponizenie_k() { return m_k; }
	double get_max_u() { return m_max_u; }
	double get_min_u() { return m_min_u; }
	double get_max_y() { return m_max_y; }
	double get_min_y() { return m_min_y; }
    vector<double> get_a() const { return m_a; }
    vector<double> get_b() const { return m_b; }
    int get_opoznienie_k() const { return (int)m_k; } // Rzutujemy na int dla wygody GUI
    double get_z() const { return m_z; }
    bool czy_ograniczenia_aktywne() {return m_czy_ograniczenia_aktywne;}
    bool czy_zaklocenia_aktywne() {return m_czy_zaklocenia_aktywne;}
    void reset_pamieci()
    {

        std::fill(m_buforU.begin(), m_buforU.end(), 0.0);
        std::fill(m_buforY.begin(), m_buforY.end(), 0.0);


    }
};
