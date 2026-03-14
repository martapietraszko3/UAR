#include "SerwisSymulacji.h"

SerwisSymulacji::SerwisSymulacji(QObject *parent)
    : QObject(parent),
    m_uklad(ModelARX({ 0 }, { 1 }, 1), RegulatorPID(), Generator())
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SerwisSymulacji::onTimerTick);
}

void SerwisSymulacji::start()
{
    int ms = m_uklad.getGenerator().get_interwal();
    if(ms <= 0) ms = 100;
    m_timer->start(ms);
}

void SerwisSymulacji::stop()
{
    m_timer->stop();
}

bool SerwisSymulacji::czyDziala() const
{
    return m_timer->isActive();
}

void SerwisSymulacji::reset_calosci()
{
    stop();
    m_czas_symulacji = 0.0;
    m_numer_kroku = 0;

    m_uklad.getGenerator().Resetuj();
    m_uklad.getRegulator().reset();
    m_uklad.getModel().reset_pamieci();
}

void SerwisSymulacji::set_interwal(int interwal)
{
    m_uklad.getGenerator().set_interwal(interwal);
    if(m_timer->isActive()) {
        m_timer->setInterval(interwal);
    }
}

void SerwisSymulacji::onTimerTick()
{
    int ms = m_uklad.getGenerator().get_interwal();
    double krok_czasu = ms / 1000.0;

    // Aktualizacja czasu
    m_czas_symulacji += krok_czasu;
    m_numer_kroku++;

    // 1. Wyliczenie wartosci zadanej
    double zadana = symulujGenerator(m_numer_kroku);

    // 2. Wykonanie kroku symulacji (PID + Obiekt)
    wykonajKrok(zadana);

    // 3. Powiadomienie widoku
    emit stanZaktualizowany();
}

double SerwisSymulacji::wykonajKrok(double wartosc_zadana)
{
    return m_uklad.symuluj(wartosc_zadana);
}

bool SerwisSymulacji::zapiszKonfiguracje(QString sciezka){

    QJsonObject root;

    // --- SEKJA ARX ---
    QJsonObject arxJson;
    QJsonArray arrayA;
    for(double val : get_a()) arrayA.append(val);

    QJsonArray arrayB;
    for(double val : get_b()) arrayB.append(val);

    arxJson["wielomian_A"] = arrayA;
    arxJson["wielomian_B"] = arrayB;
    arxJson["opoznienie_k"] = get_opoznienie();
    arxJson["odchylenie_szumu"] = get_z();
    arxJson["szum_aktywny"] = get_status_szumu();
    arxJson["limity_aktywne"] = get_status_limitow();
    arxJson["limit_u_max"] = get_limit_u_gora();
    arxJson["limit_u_min"] = get_limit_u_dol();
    arxJson["limit_y_max"] = get_limit_y_gora();
    arxJson["limit_y_min"] = get_limit_y_dol();

    root["ARX"] = arxJson;

    // --- SEKCJA GENERATOR ---
    QJsonObject genJson;
    genJson["typ_sygnalu"] = get_typ_sygnalu_int();
    genJson["amplituda"] = get_amplituda();
    genJson["okres_rzeczywisty_s"] = get_okres_rzeczywisty();
    genJson["stala_skladowa"] = get_stala_skladowa();
    genJson["wypelnienie"] = get_wypelnienie();

    root["Generator"] = genJson;

    // --- SEKCJA PID ---
    QJsonObject pidJson;
    pidJson["wzmocnienie_k"] = get_wzmocnienie_k();
    pidJson["stala_Ti"] = get_stala_calkowania();
    pidJson["stala_Td"] = get_stala_rozniczkowania();
    pidJson["tryb_calkowania"] = get_tryb_calkowania_int();

    root["PID"] = pidJson;

    // --- SEKCJA SYMULACJA ---
    QJsonObject simJson;
    simJson["interwal_ms"] = get_interwal();
    simJson["szerokosc_okna_s"] = get_szerokosc_okna();

    root["Symulacja"] = simJson;

    QFile file(sciezka);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();

    return true;
}

bool SerwisSymulacji::odczytajKonfiguracje(QString sciezka) {
    QFile file(sciezka);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray dane = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(dane));
    if (doc.isNull()) {
        return false;
    }

    QJsonObject root = doc.object();

    // --- ODCZYT ARX ---
    if (root.contains("ARX")) {
        QJsonObject arxJson = root["ARX"].toObject();

        vector<double> vecA;
        QJsonArray arrA = arxJson["wielomian_A"].toArray();
        for (const auto &val : arrA) {
            vecA.push_back(val.toDouble());
        }
        set_a(vecA);

        vector<double> vecB;
        QJsonArray arrB = arxJson["wielomian_B"].toArray();
        for (const auto &val : arrB) {
            vecB.push_back(val.toDouble());
        }
        set_b(vecB);

        set_opoznienie(arxJson["opoznienie_k"].toInt());
        set_z(arxJson["odchylenie_szumu"].toDouble());
        przelacz_szum(arxJson["szum_aktywny"].toBool());

        double u_max = arxJson["limit_u_max"].toDouble();
        double u_min = arxJson["limit_u_min"].toDouble();
        double y_max = arxJson["limit_y_max"].toDouble();
        double y_min = arxJson["limit_y_min"].toDouble();

        set_limity(u_max, u_min, y_max, y_min);
        przelacz_limit(arxJson["limity_aktywne"].toBool());
    }

    // --- ODCZYT GENERATOR ---
    if (root.contains("Generator")) {
        QJsonObject genJson = root["Generator"].toObject();

        int typInt = genJson["typ_sygnalu"].toInt();
        set_typ_sygnalu((TypSygnalu)typInt);

        set_amplituda(genJson["amplituda"].toDouble());
        set_okres_rzeczywisty(genJson["okres_rzeczywisty_s"].toInt());
        set_stala_skladowa(genJson["stala_skladowa"].toDouble());
        set_wypelnienie(genJson["wypelnienie"].toDouble());
    }

    // --- ODCZYT PID ---
    if (root.contains("PID")) {
        QJsonObject pidJson = root["PID"].toObject();

        set_wzmocnienie_k(pidJson["wzmocnienie_k"].toDouble());
        set_stala_calkowania(pidJson["stala_Ti"].toDouble());
        set_stala_rozniczkowania(pidJson["stala_Td"].toDouble());

        int trybInt = pidJson["tryb_calkowania"].toInt();
        setL_tryb_calkowania((RegulatorPID::LiczCalk)trybInt);
    }

    // --- ODCZYT SYMULACJA ---
    if (root.contains("Symulacja")) {
        QJsonObject simJson = root["Symulacja"].toObject();

        set_interwal(simJson["interwal_ms"].toInt());
        set_szerokosc_okna(simJson["szerokosc_okna_s"].toInt());
    }

    return true;
}
