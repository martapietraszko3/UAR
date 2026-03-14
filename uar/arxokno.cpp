#include "arxokno.h"
#include "ui_arxokno.h"
#include <QMessageBox>
ARXokno::ARXokno(bool czygran, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ARXokno)
{
    ui->setupUi(this);
    setWindowTitle("Konfiguracja parametrów ARX");
        ui->czygran->setChecked(czygran);

}

ARXokno::~ARXokno()
{
    delete ui;
}
vector<double> ARXokno::get_a()
{
    vector<double> wynik;
    QString tekst=ui->wspA->text();
    QStringList lista=tekst.split(",");
    for(const QString&str:lista)
    {
        bool ok;
        double wartosc=str.toDouble(&ok);
        if(ok) wynik.push_back(wartosc);
    }
    return wynik;
}
double ARXokno::get_y_gora()
{
    return ui->ygora->value();
}
double ARXokno:: get_y_dol()
{
    return ui->ydol->value();
}
double  ARXokno::get_u_dol()
{
    return ui->udol->value();
}
double  ARXokno::get_u_gora()
{
    return ui->ugora->value();
}
vector<double>  ARXokno::get_b()
{
    vector<double> wynik;
    QString tekst=ui->wspB->text();
    QStringList lista=tekst.split(",");
    for(const QString&str:lista)
    {
        bool ok;
        double wartosc=str.toDouble(&ok);
        if(ok) wynik.push_back(wartosc);
    }
    return wynik;
}
int  ARXokno::get_opoznienie()
{
    return ui->opoznienienie->value();
}
double  ARXokno::get_odchylenie()
{
    return ui->odchyelnie->value();
}
void  ARXokno::set_a(const vector<double> &a)
{
    QString tekst;
    for(size_t i=0;i<a.size();i++)
    {
        tekst+=QString::number(a[i]);
        if(i < a.size()-1) tekst += ", ";
    }
    ui->wspA->setText(tekst);
}
void  ARXokno::set_b(const vector<double> &b)
{
    QString tekst;
    for(size_t i=0;i<b.size();i++)
    {
        tekst+=QString::number(b[i]);
        if(i < b.size()-1) tekst += ", ";
    }
    ui->wspB->setText(tekst);
}
void  ARXokno::set_opoznienie(const int &k)
{
    ui->opoznienienie->setValue(k);

}
void  ARXokno::set_odchylenie(const double &e)
{

    ui->odchyelnie->setValue(e);
}

void ARXokno::on_pushButton_clicked()
{
    vector<double> spr_a = get_a();
    vector<double> spr_b = get_b();
    int spr_k = get_opoznienie();

        double y_gora= get_y_gora();
        double y_dol=get_y_dol();
        double u_gora=get_u_gora();
        double u_dol=get_u_dol();

    if(spr_a.size() < 3 || spr_b.size() < 3)
    {
        QMessageBox::warning(this, "Błąd danych", "Wielomiany A i B muszą mieć minimum 3 współczynniki!");
        return;
    }

    if(spr_k < 1)
    {
        QMessageBox::warning(this, "Błąd danych", "Opóźnienie musi być większe lub równe 1!");
        return;
    }
    this->accept();
}


void ARXokno::on_ugora_editingFinished()
{
    u_gora=ui->ugora->value();
}

void ARXokno::set_y_dol(double yd)
{
    ui->ydol->setValue(yd);
}
void ARXokno:: set_y_gora(double yd)
{
    ui->ygora->setValue(yd);
}
void ARXokno::set_u_dol(double ud)
{
    ui->udol->setValue(ud);
}
void ARXokno::set_u_gora(double ug)
{
    ui->ugora->setValue(ug);
}
void ARXokno::on_udol_editingFinished()
{
    u_dol=ui->udol->value();
}


void ARXokno::on_ygora_editingFinished()
{
    y_gora=ui->ygora->value();
}


void ARXokno::on_ydol_editingFinished()
{
    y_dol=ui->ydol->value();
}


void ARXokno::on_czygran_stateChanged(int arg1)
{
    czylimit=arg1;
}

