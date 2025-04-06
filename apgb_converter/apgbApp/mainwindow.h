#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>>
#include <QColor>

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
    QString gethexColor();


private slots:
    void on_btn_gen_file_clicked();
    void on_btn_bg_0_clicked();
    void on_btn_bg_1_clicked();
    void on_btn_bg_2_clicked();
    void on_btn_bg_3_clicked();
    void on_btn_obj0_0_clicked();
    void on_btn_obj0_1_clicked();
    void on_btn_obj0_2_clicked();
    void on_btn_obj0_3_clicked();
    void on_btn_obj1_0_clicked();
    void on_btn_obj1_1_clicked();
    void on_btn_obj1_2_clicked();
    void on_btn_obj1_3_clicked();
    void on_btn_window_0_clicked();
    void on_btn_window_1_clicked();
    void on_btn_window_2_clicked();
    void on_btn_window_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
