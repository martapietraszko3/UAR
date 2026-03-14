#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "qcustomplot.h"
#include <QMainWindow>
#include "arxokno.h"
#include "SerwisSymulacji.h"
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_arx_clicked();

    // Nowy slot do odświeżania wykresu na sygnał z serwisu
    void odswiezWykresy();

    void on_interwalslider_valueChanged(int value);

    void on_startstop_clicked();

    void on_amplituda_valueChanged(double arg1);

    void on_okres_valueChanged(int arg1);

    void on_stalaskladowa_valueChanged(double arg1);

    void on_wypelnienie_valueChanged(double arg1);

    void on_wzmocnienie_valueChanged(double arg1);

    void on_stalacalk_2_valueChanged(double arg1);

    void on_typsyg_currentIndexChanged(int index);

    void on_trybpracy_2_currentIndexChanged(int index);

    void on_stalarozn_valueChanged(double arg1);

    void on_resetsc_clicked();

    void on_resets_clicked();

    void on_resetsym_clicked();

    void on_windowWidthSlider_valueChanged(int value);

    void on_zapis_clicked();

    void on_odczyt_clicked();

    void on_stalacalk_2_editingFinished();

    void on_stalarozn_editingFinished();

    void on_amplituda_editingFinished();

    void on_okres_editingFinished();

    void on_stalaskladowa_editingFinished();

    void on_wypelnienie_editingFinished();

    void on_wzmocnienie_editingFinished();

    void on_czas_spin_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    // Serwis jako wskaźnik (dziecko MainWindow)
    SerwisSymulacji *serwis;

    // Zmienne stricte wizualne
    double m_szerokoscOknaWykresu;
    double m_aktualnaSzerokoscWykresu;

    void aktualizujWykres(QCustomPlot *plot, double czas);
};
#endif // MAINWINDOW_H
