#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Tworzymy serwis jako dziecko okna
    serwis = new SerwisSymulacji(this);

    // Łączymy sygnał z serwisu z odświeżaniem GUI
    connect(serwis, &SerwisSymulacji::stanZaktualizowany, this, &MainWindow::odswiezWykresy);

    // Konfiguracja szerokości okna (wizualna)
    m_szerokoscOknaWykresu = 10.0;    // Wartość docelowa (zadana)
    m_aktualnaSzerokoscWykresu = 10.0; // Wartość aktualna (startowa)

    ui->windowWidthSlider->setRange(5, 50);
    ui->windowWidthSlider->setValue(10);

    setWindowTitle("Symulator Układu Automatycznej Regulacji");

    // --- Konfiguracja Wykresów ---
    ui->wsyg->addGraph(); ui->wsyg->graph(0)->setPen(QPen(Qt::red));
    ui->wsyg->addGraph(); ui->wsyg->graph(1)->setPen(QPen(Qt::blue));

    ui->wuch->addGraph(); ui->wuch->graph(0)->setPen(QPen(Qt::black));

    ui->wskl->addGraph(); ui->wskl->graph(0)->setPen(QPen(Qt::magenta));

    ui->wpid->addGraph(); ui->wpid->graph(0)->setPen(QPen(Qt::red)); ui->wpid->graph(0)->setName("P");
    ui->wpid->addGraph(); ui->wpid->graph(1)->setPen(QPen(Qt::green)); ui->wpid->graph(1)->setName("I");
    ui->wpid->addGraph(); ui->wpid->graph(2)->setPen(QPen(Qt::blue)); ui->wpid->graph(2)->setName("D");

    ui->wpid->legend->setVisible(true);
    // Legenda poza wykresem
    ui->wpid->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignLeft);

    // Etykiety
    ui->wsyg->xAxis->setLabel("Czas [s]"); ui->wsyg->yAxis->setLabel("Wartość");
    ui->wuch->xAxis->setLabel("Czas [s]"); ui->wuch->yAxis->setLabel("Wchyb");
    ui->wskl->xAxis->setLabel("Czas [s]"); ui->wskl->yAxis->setLabel("Sterowanie");
    ui->wpid->xAxis->setLabel("Czas [s]"); ui->wpid->yAxis->setLabel("PID");

    ui->wypelnienie->setValue(0.5);
    on_typsyg_currentIndexChanged(ui->typsyg->currentIndex());\

}

MainWindow::~MainWindow()
{
    delete ui;
    // Serwis usuwa się sam, bo jest dzieckiem QObject (this)
}

void MainWindow::aktualizujWykres(QCustomPlot *plot, double czas)
{
    // 1. Ustalanie zakresu osi X
    double koniecX = czas + 0.5;
    double poczatekX = czas - m_aktualnaSzerokoscWykresu;

    plot->xAxis->setRange(poczatekX, koniecX);

    // 2. Zapominanie danych
    double limitPamieci = czas - 60.0;
    for(int i=0; i<plot->graphCount(); ++i) {
        plot->graph(i)->data()->removeBefore(limitPamieci);
    }

    // 3. Skalowanie osi Y
    bool znalezionoZakres = false;
    QCPRange zakresY;

    // Pobieramy aktualny zakres osi X
    QCPRange zakresX = plot->xAxis->range();

    for(int i=0; i<plot->graphCount(); ++i) {
        bool found = false;
        QCPRange r = plot->graph(i)->getValueRange(found, QCP::sdBoth, zakresX);

        if (found) {
            if (!znalezionoZakres) {
                zakresY = r;
                znalezionoZakres = true;
            } else {
                zakresY.expand(r);
            }
        }
    }

    if (znalezionoZakres && zakresY.size() > 1e-10) {
        double srodek = zakresY.center();
        double amplituda = zakresY.size() / 2.0;
        double nowaPolowaWysokosci = amplituda / 0.8; // margines 10% góra/dół

        plot->yAxis->setRange(srodek - nowaPolowaWysokosci, srodek + nowaPolowaWysokosci);
    } else {
        double srodek = (znalezionoZakres ? zakresY.center() : 0.0);
        plot->yAxis->setRange(srodek - 1.0, srodek + 1.0);
    }

    plot->replot();
}

void MainWindow::on_arx_clicked()
{
    ARXokno okno(serwis->get_status_limitow(), this);
    okno.set_a(serwis->get_a());
    okno.set_b(serwis->get_b());
    okno.set_opoznienie(serwis->get_opoznienie());
    okno.set_odchylenie(serwis->get_z());
    okno.set_y_dol(serwis->get_limit_y_dol());
    okno.set_y_gora(serwis->get_limit_y_gora());
    okno.set_u_gora(serwis->get_limit_u_gora());
    okno.set_u_dol(serwis->get_limit_u_dol());
    if(okno.exec()==QDialog::Accepted)
    {
        vector<double> nowe_a=okno.get_a();
        vector<double> nowe_b=okno.get_b();
        int nowe_k=okno.get_opoznienie();
        double nowe_z=okno.get_odchylenie();
        serwis->set_a(nowe_a);
        serwis->set_b(nowe_b);
        serwis->set_opoznienie(nowe_k);
        serwis->set_z(nowe_z);
        serwis->set_limity(okno.get_u_gora(),okno.get_u_dol(),okno.get_y_gora(),okno.get_y_dol());
        serwis->przelacz_limit(okno.czylimit);
    }
}

// Ta funkcja jest wywolywana, gdy SerwisSymulacji zglosi "stanZaktualizowany"
void MainWindow::odswiezWykresy()
{
    // Pobieramy dane z serwisu
    double czas = serwis->get_czas_symulacji();
    int interwal = serwis->get_interwal();
    double krok_czasu = interwal / 1000.0;

    double zadana = serwis->get_wartosc_zadana();
    double wyjscie = serwis->get_wartosc_regulowana();
    double sterowanie = serwis->get_wartosc_p() + serwis->get_wartosc_i() + serwis->get_wartosc_d();
    double uchyb = serwis->get_uchyb();

    // --- Logika płynnej zmiany szerokości okna ---
    double roznica = m_szerokoscOknaWykresu - m_aktualnaSzerokoscWykresu;

    if (std::abs(roznica) < krok_czasu) {
        m_aktualnaSzerokoscWykresu = m_szerokoscOknaWykresu;
    }
    else {
        double zmiana = roznica * 0.1;
        if (roznica > 0 && zmiana < krok_czasu) zmiana = krok_czasu;
        m_aktualnaSzerokoscWykresu += zmiana;
    }

    // Dodawanie danych do wykresów
    ui->wsyg->graph(0)->addData(czas, zadana);
    ui->wsyg->graph(1)->addData(czas, wyjscie);

    ui->wskl->graph(0)->addData(czas, sterowanie);

    ui->wuch->graph(0)->addData(czas, uchyb);

    ui->wpid->graph(0)->addData(czas, serwis->get_wartosc_p());
    ui->wpid->graph(1)->addData(czas, serwis->get_wartosc_i());
    ui->wpid->graph(2)->addData(czas, serwis->get_wartosc_d());

    // Aktualizacja wyglądu wykresów
    aktualizujWykres(ui->wsyg, czas);
    aktualizujWykres(ui->wskl, czas);
    aktualizujWykres(ui->wuch, czas);
    aktualizujWykres(ui->wpid, czas);
}

void MainWindow::on_interwalslider_valueChanged(int value)
{
    serwis->set_interwal(value);
}


void MainWindow::on_startstop_clicked()
{
    if(serwis->czyDziala())
    {
        serwis->stop();
        ui->startstop->setText("Start");
    }
    else
    {
        int ms = ui->interwalbox->value();
        serwis->set_interwal(ms);
        serwis->start();
        ui->startstop->setText("Stop");
    }
}


void MainWindow::on_amplituda_valueChanged(double arg1)
{

}


void MainWindow::on_okres_valueChanged(int arg1)
{

}


void MainWindow::on_stalaskladowa_valueChanged(double arg1)
{

}


void MainWindow::on_wypelnienie_valueChanged(double arg1)
{

}


void MainWindow::on_wzmocnienie_valueChanged(double arg1)
{

}


void MainWindow::on_stalacalk_2_valueChanged(double arg1)
{

}



////

void MainWindow::on_stalacalk_2_editingFinished()
{
    serwis->set_stala_calkowania(ui->stalacalk_2->value());
}


void MainWindow::on_wzmocnienie_editingFinished()
{
    serwis->set_wzmocnienie_k(ui->wzmocnienie->value());
}



void MainWindow::on_stalarozn_editingFinished()
{
    serwis->set_stala_rozniczkowania(ui->stalarozn->value());
}


void MainWindow::on_amplituda_editingFinished()
{
    serwis->set_amplituda(ui->amplituda->value());
}


void MainWindow::on_okres_editingFinished()
{
    serwis->set_okres_rzeczywisty(ui->okres->value());
}


void MainWindow::on_stalaskladowa_editingFinished()
{
    serwis->set_stala_skladowa(ui->stalaskladowa->value());
}


void MainWindow::on_wypelnienie_editingFinished()
{
    serwis->set_wypelnienie(ui->wypelnienie->value());
}




void MainWindow::on_typsyg_currentIndexChanged(int index)
{
    if(index == 0)
    {
        // Prostokątny -> Wypełnienie jest potrzebne
        serwis->set_typ_sygnalu(TypSygnalu::prostakatny);
        ui->wypelnienie->setEnabled(true);
    }
    else
    {
        // Sinusoidalny -> Wypełnienie nie ma sensu, blokujemy
        serwis->set_typ_sygnalu(TypSygnalu::sinusoidalny);
        ui->wypelnienie->setEnabled(false);
    }
}


void MainWindow::on_trybpracy_2_currentIndexChanged(int index)
{
    if(index==0) serwis->setL_tryb_calkowania(RegulatorPID::LiczCalk::Zew);
    else serwis->setL_tryb_calkowania(RegulatorPID::LiczCalk::Wew);
}


void MainWindow::on_stalarozn_valueChanged(double arg1)
{
    serwis->set_stala_rozniczkowania(arg1);
}


void MainWindow::on_resetsc_clicked()
{
    serwis->reset_pid_calka();

}


void MainWindow::on_resets_clicked()
{
    serwis->reset_pid_rozniczka();

}


void MainWindow::on_resetsym_clicked()
{
    ui->startstop->setText("Start");

    // Resetujemy cala logike w serwisie
    serwis->reset_calosci();

    // Reset logiki okna
    m_szerokoscOknaWykresu = 10.0;
    m_aktualnaSzerokoscWykresu = 10.0;
    ui->windowWidthSlider->setValue(10);

    ui->interwalslider->setValue(200);
    ui->interwalbox->setValue(200);

    // Reset GUI
    ui->typsyg->setCurrentIndex(0);
    ui->amplituda->setValue(0.0);
    ui->okres->setValue(1);
    ui->stalaskladowa->setValue(0.0);
    ui->wypelnienie->setValue(0.5);
    ui->wzmocnienie->setValue(0.0);
    ui->stalacalk_2->setValue(0.0);
    ui->stalarozn->setValue(0.0);
    ui->trybpracy_2->setCurrentIndex(0);

    // Czyszczenie danych
    ui->wsyg->graph(0)->data()->clear();
    ui->wsyg->graph(1)->data()->clear();
    ui->wuch->graph(0)->data()->clear();
    ui->wskl->graph(0)->data()->clear();
    ui->wpid->graph(0)->data()->clear();
    ui->wpid->graph(1)->data()->clear();
    ui->wpid->graph(2)->data()->clear();

    // Wstępny replot (pusty)
    aktualizujWykres(ui->wsyg, 0);
    aktualizujWykres(ui->wuch, 0);
    aktualizujWykres(ui->wskl, 0);
    aktualizujWykres(ui->wpid, 0);
}


void MainWindow::on_windowWidthSlider_valueChanged(int value)
{
    m_szerokoscOknaWykresu = (double)value;

    // Jeśli symulacja stoi, odświeżamy wykres "statycznie"
    if (!serwis->czyDziala()) {
        m_aktualnaSzerokoscWykresu = m_szerokoscOknaWykresu;
        double czas = serwis->get_czas_symulacji();
        aktualizujWykres(ui->wsyg, czas);
        aktualizujWykres(ui->wskl, czas);
        aktualizujWykres(ui->wuch, czas);
        aktualizujWykres(ui->wpid, czas);
    }


    ui->czas_spin->setValue(value);

}


void MainWindow::on_zapis_clicked()
{
    QString sciezka=QFileDialog::getSaveFileName(this, "Zapisz konfigurację","","Pliki JSON (*.json);;Wszystkie pliki (*)");
    if (sciezka.isEmpty()) {
        return;
    }
    bool sukces = serwis->zapiszKonfiguracje(sciezka);
    if (sukces) {
        QMessageBox::information(this, "Sukces", "Konfiguracja została zapisana.");
    } else {
        QMessageBox::critical(this, "Błąd", "Nie udało się zapisać pliku.");
    }
}


void MainWindow::on_odczyt_clicked()
{
    QString sciezka=QFileDialog::getOpenFileName(this, "Wczytaj konfigurację", "","Pliki JSON (*.json);;Wszystkie pliki (*)" );
    if(sciezka.isEmpty())
        return;
    bool sukces=serwis->odczytajKonfiguracje(sciezka);
    if(sukces)
    {
        bool oldState = this->blockSignals(true);

        ui->typsyg->setCurrentIndex(serwis->get_typ_sygnalu_int());
        ui->amplituda->setValue(serwis->get_amplituda());
        ui->okres->setValue(serwis->get_okres_rzeczywisty());
        ui->stalaskladowa->setValue(serwis->get_stala_skladowa());
        ui->wypelnienie->setValue(serwis->get_wypelnienie());

        // --- PID ---
        ui->wzmocnienie->setValue(serwis->get_wzmocnienie_k());
        ui->stalacalk_2->setValue(serwis->get_stala_calkowania());
        ui->stalarozn->setValue(serwis->get_stala_rozniczkowania());
        ui->trybpracy_2->setCurrentIndex(serwis->get_tryb_calkowania_int());

        // --- Symulacja ---
        ui->interwalbox->setValue(serwis->get_interwal());
        // Slider zaktualizuje się sam, bo jest połączony z boxem
        ui->interwalslider->setValue(serwis->get_interwal());

        ui->windowWidthSlider->setValue(serwis->get_szerokosc_okna());

        this->blockSignals(oldState); // Odblokowujemy sygnały

        // Wymuszenie odświeżenia logiki GUI
        on_typsyg_currentIndexChanged(ui->typsyg->currentIndex());
        QMessageBox::information(this, "Sukces", "Konfiguracja została wczytana.");
    }


    else
        QMessageBox::critical(this, "Błąd", "Nie udało się wczytać pliku.");

}




void MainWindow::on_czas_spin_valueChanged(int arg1)
{
    ui->windowWidthSlider->setValue(arg1);
}

