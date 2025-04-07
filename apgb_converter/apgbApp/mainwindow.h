#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

using namespace std;

struct APGB_Palette{
    string *bg, *obj0, *obj1, *window;
    int numOfBytes = 56;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString gethexColor();
    APGB_Palette importPaletteFromCSV(string filename);
    APGB_Palette importPaletteFromPAL(string filename);
    APGB_Palette getPalette(string *bg, string *obj0, string *obj1, string *window);
    char* APGBFormat(APGB_Palette p, int size);
    void savePalette(string filename, APGB_Palette p);

    int* fromStrToHexStrArr(string str, int numOfBytes);
    string fromHexStrToStr(string hexStr, int numOfBtyes);
    string trim(string data);
    void clearBuffer(char *buffer, int size);
    bool isKeyword(string word);

    string footer, lcdOff;

private slots:
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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
