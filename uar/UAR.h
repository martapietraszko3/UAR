#pragma once
#include "regulatorPID.h"
#include "Generator.h"
#include"ModelARX.h"
class UkladRegulacji
{
private:
	RegulatorPID m_regulator;
	Generator m_generator;
	ModelARX m_modelARX;
	double m_ostatnie_e = 0.0;

public:
    UkladRegulacji(ModelARX mod, RegulatorPID reg, Generator gen) :m_modelARX(mod), m_regulator(reg), m_generator(gen) {
	}
    UkladRegulacji(ModelARX mod, RegulatorPID reg) :m_modelARX(mod), m_regulator(reg){
	}
	
	double symuluj(double wartosc_zadana);//dla testow
	ModelARX& getModel() { return m_modelARX; }
	RegulatorPID& getRegulator() { return m_regulator; }
	Generator& getGenerator() { return m_generator; }//zeby mozna bylo to modyfikowac latwo w uslugach
	double get_e() { return m_ostatnie_e; }

};
