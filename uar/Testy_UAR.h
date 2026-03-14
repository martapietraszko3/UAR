// TU includy:
/*
#pragma once

#include <iostream>
#include <iomanip>
#include "ModelARX.h"

#define DEBUG  // ustaw na MAIN aby skompilować program docelowy / ustaw na DEBUG aby skompilować program testujacy 

#ifdef DEBUG

// Declarations only — definitions must live in a single .cpp (Testy_UAR.cpp)
void raportBleduSekwencji(std::vector<double>& spodz, std::vector<double>& fakt);
bool porownanieSekwencji(std::vector<double>& spodz, std::vector<double>& fakt);
void myAssert(std::vector<double>& spodz, std::vector<double>& fakt);

// testy dla samego ARX (declarations)
namespace TESTY_ModelARX
{
    void wykonaj_testy();
    void test_brakPobudzenia();
    void test_skokJednostkowy_1();
    void test_skokJednostkowy_2();
    void test_skokJednostkowy_3();
}

#endif

*/
#pragma once
#include <vector> 


void raportBleduSekwencji(std::vector<double>& spodz, std::vector<double>& fakt);
bool porownanieSekwencji(std::vector<double>& spodz, std::vector<double>& fakt);
void myAssert(std::vector<double>& spodz, std::vector<double>& fakt);

namespace TESTY_ModelARX
{
    void wykonaj_testy();
    void test_brakPobudzenia();
    void test_skokJednostkowy_1();
    void test_skokJednostkowy_2();
    void test_skokJednostkowy_3();
}
namespace TESTY_RegulatorPID
{
    void wykonaj_testy();
    void test_P_brakPobudzenia();
    void test_P_skokJednostkowy();
    void test_PI_skokJednostkowy_1();
    void test_PI_skokJednostkowy_2();
    void test_PID_skokJednostkowy();
    void test_PI_skokJednostkowy_3();
}
namespace TESTY_ProstyUAR
{
    void wykonaj_testy();
    void test_UAR_1_brakPobudzenia();
    void test_UAR_1_skokJednostkowyPID();
    void test_UAR_2_skokJednostkowyPID();
    void test_UAR_3_skokJednostkowyPID();
    void test_UAR_Generator_Prostokatny();
    void test_UAR_Generator_Sinusoidalny();

}
