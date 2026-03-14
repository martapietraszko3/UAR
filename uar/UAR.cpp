#include"UAR.h"
double UkladRegulacji::symuluj(double wartosc_zadana)
{
    double w_i = wartosc_zadana;
    double y_i = m_modelARX.get_yi(); 
    double e_i = w_i - y_i;  
	m_ostatnie_e = e_i;
    double u_i = m_regulator.symuluj(e_i);
   return m_modelARX.symuluj(u_i);          
   
}
