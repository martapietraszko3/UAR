#include "ModelARX.h"
#include <iostream>
#include <vector>
#include <deque>
#include <assert.h>
#include <iomanip>


ModelARX::ModelARX(vector<double> a, vector<double> b, size_t k, double z) :m_a(a), m_b(b), m_k(k), m_z(z)
{
	m_buforU.resize(m_b.size(), 0);
	m_buforY.resize(m_a.size(), 0);
	m_buforOpoznioneU.resize(m_k, 0);
	double bezpieczne_z = (m_z > 0.0) ? m_z : 1.0;//dodanie tej linijki poniewaz podczas testow biblioteka random wyrzucała asercję,
	//ze względu na to, że nie można zrobić normal distribution od 0 do 0
	m_generator_losowy.seed(random_device{}());
	m_rozklad_gaussa = normal_distribution<double>(0.0, bezpieczne_z);

}

double ModelARX::symuluj(double noweU)
{
	if (m_czy_ograniczenia_aktywne)
	{
		if (noweU > m_max_u) {//zapomniane granice
			noweU = m_max_u;
		}
		else if (noweU < m_min_u) {
			noweU = m_min_u;
		}
	}
	
	m_buforOpoznioneU.push_front(noweU);
	double opoznioneU = m_buforOpoznioneU.back();
	m_buforOpoznioneU.pop_back();
	m_buforU.push_front(opoznioneU);
	m_buforU.pop_back();
	double iloczyn_wag_b = 0;
	for (int i = 0; i < m_b.size(); i++)
	{
		iloczyn_wag_b += m_buforU[i] * m_b[i];
	}
	double iloczyn_wag_a = 0;
	for (int i = 0; i < m_a.size(); i++)
	{
		iloczyn_wag_a += m_buforY[i] * m_a[i];
	}
	double noweY = iloczyn_wag_b - iloczyn_wag_a;
	if (m_czy_zaklocenia_aktywne)
	{
		if (m_z > 0.0)
		{
			double zaklocenie = m_rozklad_gaussa(m_generator_losowy);
			noweY += zaklocenie;
		}
	}
	if (m_czy_ograniczenia_aktywne)
	{
		if (noweY > m_max_y) {//zapomniane granice
			noweY = m_max_y;
		}
		else if (noweY < m_min_y) {
			noweY = m_min_y;
		}
	}
		
	m_buforY.push_front(noweY);
	m_buforY.pop_back();
	return noweY;
}





