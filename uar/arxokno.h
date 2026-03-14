#ifndef ARXOKNO_H
#define ARXOKNO_H

#include <QDialog>
using namespace std;
#include<vector>
namespace Ui {
class ARXokno;
}

class ARXokno : public QDialog
{
    Q_OBJECT

public:
    explicit ARXokno(bool czygran, QWidget *parent = nullptr);
    ~ARXokno();
    vector<double> get_a();
    vector<double> get_b();
    int get_opoznienie();
    double get_odchylenie();
    void set_a(const vector<double> &a);
    void set_b(const vector<double> &b);
    void set_opoznienie(const int &k);
    void set_odchylenie(const double &e);
    double y_gora;
    double y_dol;
    double u_gora;
    double u_dol;
    bool czylimit;
    double get_y_gora();
    double get_y_dol();
    double get_u_dol();
    double get_u_gora();
    void set_y_dol(double yd);
    void set_y_gora(double yd);
    void set_u_dol(double ud);
    void set_u_gora(double ug);


private slots:
    void on_pushButton_clicked();

    void on_ugora_editingFinished();

    void on_udol_editingFinished();

    void on_ygora_editingFinished();

    void on_ydol_editingFinished();

    void on_czygran_stateChanged(int arg1);

private:
    Ui::ARXokno *ui;
};

#endif // ARXOKNO_H
