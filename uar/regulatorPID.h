#pragma once
#include<deque>
using namespace std;

class RegulatorPID
{
public:
	enum class LiczCalk { Zew, Wew };
private:
	double m_poprzedniUchyb;//ei-1
	double m_wzmocnienie;//wprowadzane przez uz, do P, we wzorze k
	double m_WartosciSterowaniaP;//u liczone 
	double m_WartosciSterowaniaI;//u liczone 
	double m_WartosciSterowaniaD;//u liczone 
	double m_stalaCalkowania;//2 sposoby liczenia
	double m_sumaBledow;
	double m_stalaRozniczkowania;
	LiczCalk m_tryb_calkowania;//wybor, czy robimy sumeI1 czy I2
	double m_ostatnia_suma = 0.0;
	double m_ostatnie_P = 0.0; 
	double m_ostatnie_I = 0.0;
	double m_ostatnie_D = 0.0;
public:
	RegulatorPID();
	RegulatorPID(double wzmocnienie, double stalaCalkowania = 0.0, double stalaRozniczkowania = 0.0, LiczCalk tryb = LiczCalk::Zew);
	double symuluj(double aktualnyUchyb);
	void reset();
	//setery i getery
	void setLiczCalk(LiczCalk nowyTryb);
	void set_wzmocnienie_k(double k) { m_wzmocnienie = k; }
	void setStalaCalk(double stala) { m_stalaCalkowania = stala; }
	void set_stala_rozniczkowania(double stala) { m_stalaRozniczkowania = stala; }
	double get_wzmocnienie() { return m_wzmocnienie; }
	double get_stala_calkowania() { return m_stalaCalkowania; }
	double get_stala_rozniczkowania() { return m_stalaRozniczkowania; }
	double get_wartosc_p() { return m_ostatnie_P; }
	double get_wartosc_i() { return m_ostatnie_I; }
	double get_wartosc_d() { return m_ostatnie_D; }
	double get_suma_pid() { return m_ostatnia_suma; }
    LiczCalk get_tryb_calkowania(){return m_tryb_calkowania;}
    void reset_pamieci_calkowania()
    {
        m_sumaBledow = 0.0;
    }
    void reset_pamieci_rozniczkowania()
    {
        m_poprzedniUchyb = 0.0;
    }
};
