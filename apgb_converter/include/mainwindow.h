#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QColor>

#include "fileimporter.h"
#include "save.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Functions
    QString gethexColor();

    // Classes instances
    FileImporter importer;
    FileSave fsave;

private slots:
    // Editor tab
    void on_btn_import_clicked();
    void on_btn_save_clicked();

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

    void on_txt_bg_0_textChanged();
    void on_txt_bg_1_textChanged();
    void on_txt_bg_2_textChanged();
    void on_txt_bg_3_textChanged();
    void on_txt_obj0_0_textChanged();
    void on_txt_obj0_1_textChanged();
    void on_txt_obj0_2_textChanged();
    void on_txt_obj0_3_textChanged();
    void on_txt_obj1_0_textChanged();
    void on_txt_obj1_1_textChanged();
    void on_txt_obj1_2_textChanged();
    void on_txt_obj1_3_textChanged();
    void on_txt_window_0_textChanged();
    void on_txt_window_1_textChanged();
    void on_txt_window_2_textChanged();
    void on_txt_window_3_textChanged();

    // Converter tab
    void on_btn_convert_reset_clicked();
    void on_btn_get_load_clicked();
    void on_btn_get_save_clicked();
    void on_btn_convert_save_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
