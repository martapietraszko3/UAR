#pragma once
#include <QObject>
#include <QTimer>
#include "UAR.h"
#include <vector>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

using namespace std;

class SerwisSymulacji : public QObject
{
    Q_OBJECT

private:
    UkladRegulacji m_uklad;
    QTimer *m_timer;

    // Zmienne stanu symulacji (przeniesione z MainWindow)
    double m_czas_symulacji = 0.0;
    int m_numer_kroku = 0;
    int m_szerokosc_okna = 10; // Przechowujemy to, by moc zapisac do pliku

public:
    explicit SerwisSymulacji(QObject *parent = nullptr);

    // --- STEROWANIE ---
    void start();
    void stop();
    bool czyDziala() const;
    void reset_calosci();

    // --- GETTERY STANU ---
    double get_czas_symulacji() const { return m_czas_symulacji; }
    int get_numer_kroku() const { return m_numer_kroku; }

    // --- ARX ---
    void set_a(vector<double> a) { m_uklad.getModel().set_a(a); }
    void set_b(vector<double> b) { m_uklad.getModel().set_b(b); }
    void set_opoznienie(size_t k) { m_uklad.getModel().set_opoznienie_k(k); }
    void set_z(double z) { m_uklad.getModel().set_z(z); }
    void set_limity(double gora_u, double dol_u, double gora_y, double dol_y) {
        m_uklad.getModel().set_limit_u(gora_u, dol_u);
        m_uklad.getModel().set_limit_y(gora_y, dol_y);
    }
    void przelacz_limit(bool stan) { m_uklad.getModel().przelacz_ograniczenia(stan); }
    void przelacz_szum(bool stan) { m_uklad.getModel().przelacz_zaklocenia(stan); }
    void reset_arx() { m_uklad.getModel().reset_pamieci(); }

    // Gettery ARX (KONIECZNE DO ZAPISU)
    vector<double> get_a() { return m_uklad.getModel().get_a(); }
    vector<double> get_b() { return m_uklad.getModel().get_b(); }
    int get_opoznienie() { return m_uklad.getModel().get_opoznienie_k(); }
    double get_z() { return m_uklad.getModel().get_z(); }
    // Zakladam, ze ModelARX ma te metody - jesli nie, musisz je tam dodac!
    double get_limit_u_gora() { return m_uklad.getModel().get_max_u(); }
    double get_limit_u_dol() { return m_uklad.getModel().get_min_u(); }
    double get_limit_y_gora() { return m_uklad.getModel().get_max_y(); }
    double get_limit_y_dol() { return m_uklad.getModel().get_min_y(); }
    bool get_status_limitow() { return m_uklad.getModel().czy_ograniczenia_aktywne(); }
    bool get_status_szumu() { return m_uklad.getModel().czy_zaklocenia_aktywne(); }

    // --- GENERATOR ---
    void set_typ_sygnalu(TypSygnalu typ) { m_uklad.getGenerator().set_typ_sygnalu(typ); }
    void set_okres_rzeczywisty(int okres) { m_uklad.getGenerator().set_okres_rzeczywisty(okres); }
    void set_amplituda(double amplituda) { m_uklad.getGenerator().set_amplituda(amplituda); }
    void set_stala_skladowa(double stala) { m_uklad.getGenerator().set_stala_skladowa(stala); }
    void set_wypelnienie(double wypelnienie) { m_uklad.getGenerator().set_wypelnienie(wypelnienie); }
    void set_interwal(int interwal); // Aktualizuje tez timer
    void reset_generatora() { m_uklad.getGenerator().Resetuj(); }

    // Gettery Generatora (KONIECZNE DO ZAPISU)
    double get_amplituda() { return m_uklad.getGenerator().get_amplituda(); }
    // Zakladam, ze Generator ma te metody:
    int get_typ_sygnalu_int() { return (int)m_uklad.getGenerator().get_typ(); } // get typ sygnału lepsza nazwa?
    int get_okres_rzeczywisty() { return m_uklad.getGenerator().get_okres_rzeczywisty(); }
    double get_stala_skladowa() { return m_uklad.getGenerator().get_stala_skladowa(); }
    double get_wypelnienie() { return m_uklad.getGenerator().get_wypelnienie(); }
    int get_interwal() { return m_uklad.getGenerator().get_interwal(); }

    // --- REGULATOR PID ---
    void setL_tryb_calkowania(RegulatorPID::LiczCalk nowyTryb) { m_uklad.getRegulator().setLiczCalk(nowyTryb); };
    void set_wzmocnienie_k(double k) { m_uklad.getRegulator().set_wzmocnienie_k(k); }
    void set_stala_calkowania(double stala) { m_uklad.getRegulator().setStalaCalk(stala); }
    void set_stala_rozniczkowania(double stala) { m_uklad.getRegulator().set_stala_rozniczkowania(stala); }
    void reset_pid() { m_uklad.getRegulator().reset(); }
    void reset_pid_calka() { m_uklad.getRegulator().reset_pamieci_calkowania(); }
    void reset_pid_rozniczka() { m_uklad.getRegulator().reset_pamieci_rozniczkowania(); }

    // Gettery PID (KONIECZNE DO ZAPISU)
    double get_wzmocnienie_k() { return m_uklad.getRegulator().get_wzmocnienie(); }
    double get_stala_calkowania() { return m_uklad.getRegulator().get_stala_calkowania(); }
    double get_stala_rozniczkowania() { return m_uklad.getRegulator().get_stala_rozniczkowania(); }
    int get_tryb_calkowania_int() { return (int)m_uklad.getRegulator().get_tryb_calkowania(); }

    // --- SYMULACJA / WYKRESY ---
    double get_wartosc_regulowana() { return m_uklad.getModel().get_yi(); }
    double get_wartosc_zadana() { return m_uklad.getGenerator().get_sygnal(); }
    double get_wartosc_p() { return m_uklad.getRegulator().get_wartosc_p(); }
    double get_wartosc_i() { return m_uklad.getRegulator().get_wartosc_i(); }
    double get_wartosc_d() { return m_uklad.getRegulator().get_wartosc_d(); }
    double get_suma_bledow() { return m_uklad.getRegulator().get_suma_pid(); }
    double get_uchyb() { return m_uklad.get_e(); }

    // Setter i Getter dla szerokosci okna (do zapisu stanu GUI)
    void set_szerokosc_okna(int value) { m_szerokosc_okna = value; }
    int get_szerokosc_okna() { return m_szerokosc_okna; }

    // Metody sterujace
    double wykonajKrok(double wartosc_zadana); // implementacja w .cpp
    double symulujGenerator(int krok) { return m_uklad.getGenerator().Generuj(krok); }

    // --- ZAPIS KONFIGURACJI ---
    bool zapiszKonfiguracje(QString sciezka);
    bool odczytajKonfiguracje(QString sciezka);

signals:
    // Sygnal dla GUI - nowe dane gotowe do narysowania
    void stanZaktualizowany();

private slots:
    void onTimerTick();
};
