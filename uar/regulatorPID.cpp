#include <iostream>
#include "regulatorPID.h"
#include<vector>
#include<assert.h>
#include<iomanip>
#include "Testy_UAR.h"
RegulatorPID::RegulatorPID()
	: m_wzmocnienie(0.0), m_stalaCalkowania(0.0), m_stalaRozniczkowania(0.0),
	m_sumaBledow(0.0), m_poprzedniUchyb(0.0), m_tryb_calkowania(LiczCalk::Zew)
{

}
void RegulatorPID::setLiczCalk(LiczCalk nowyTryb)
{
	LiczCalk staryTryb = m_tryb_calkowania;
	//jesli tryb sie nie zmienia lub Ti=0 to po prostu return
	if (nowyTryb == staryTryb) return;
	if (m_stalaCalkowania == 0.0)
	{
		m_tryb_calkowania = nowyTryb;
		return;
	}
	//skalowanie sumy bledow przy przejsciu zew->wew
	if (nowyTryb == LiczCalk::Wew && staryTryb == LiczCalk::Zew)
	{
		m_sumaBledow = m_sumaBledow / m_stalaCalkowania;
	}
	//skalowanie sumy bledow przy przejsciu wew->zew
	else if (nowyTryb == LiczCalk::Zew && staryTryb == LiczCalk::Wew)
	{
		m_sumaBledow = m_sumaBledow * m_stalaCalkowania;
	}
	m_tryb_calkowania = nowyTryb;
}
RegulatorPID::RegulatorPID(double wzmocnienie, double stalaCalkowania, double stalaRozniczkowania, LiczCalk tryb)
	: m_wzmocnienie(wzmocnienie),
	m_stalaCalkowania(stalaCalkowania),
	m_stalaRozniczkowania(stalaRozniczkowania),
	m_sumaBledow(0.0),
	m_poprzedniUchyb(0.0),
	m_WartosciSterowaniaP(0.0),
	m_WartosciSterowaniaI(0.0),
	m_WartosciSterowaniaD(0.0),
	m_tryb_calkowania(tryb)
{
}
void RegulatorPID::reset()
{
	m_sumaBledow = 0.0;
	m_poprzedniUchyb = 0.0;
}
double RegulatorPID::symuluj(double aktualnyUchyb)
{
	//P
	m_WartosciSterowaniaP = m_wzmocnienie * aktualnyUchyb;
	//I
	if (m_stalaCalkowania == 0.0)
	{
		m_WartosciSterowaniaI = 0.0;
	}
	else
	{
		if (m_tryb_calkowania == LiczCalk::Zew)
		{
			m_sumaBledow += aktualnyUchyb;
			m_WartosciSterowaniaI = m_sumaBledow * (1.0 / m_stalaCalkowania);
		}
		else
		{
			m_sumaBledow += (aktualnyUchyb / m_stalaCalkowania);
			m_WartosciSterowaniaI = m_sumaBledow;
		}
	}


	//D
	m_WartosciSterowaniaD = m_stalaRozniczkowania * (aktualnyUchyb - m_poprzedniUchyb);
	m_poprzedniUchyb = aktualnyUchyb;
	m_ostatnie_P = m_WartosciSterowaniaP;
	m_ostatnie_I = m_WartosciSterowaniaI;
	m_ostatnie_D = m_WartosciSterowaniaD;
	m_ostatnia_suma = m_ostatnie_P + m_ostatnie_I + m_ostatnie_D;
	return m_WartosciSterowaniaD + m_WartosciSterowaniaP + m_WartosciSterowaniaI;

}




