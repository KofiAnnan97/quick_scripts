#include "mainwindow.h"
#include "./ui/ui_mainwindow.h"
#include <QTextEdit>
#include <QColorDialog>
#include <QString>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set text fields for darkest and brightest hex values on startup
    ui->txt_bg_0->setText(importer.black);
    ui->txt_bg_3->setText(importer.white);
    ui->txt_obj0_0->setText(importer.black);
    ui->txt_obj0_3->setText(importer.white);
    ui->txt_obj1_0->setText(importer.black);
    ui->txt_obj1_3->setText(importer.white);
    ui->txt_window_0->setText(importer.black);
    ui->txt_window_3->setText(importer.white);

    // Set button color for black and white hex values on startup
    ui->btn_bg_0->setStyleSheet("QPushButton { background-color : " + importer.black + "}");
    ui->btn_bg_3->setStyleSheet("QPushButton { background-color : " + importer.white + "}");
    ui->btn_obj0_0->setStyleSheet("QPushButton { background-color : " + importer.black + "}");
    ui->btn_obj0_3->setStyleSheet("QPushButton { background-color : " + importer.white + "}");
    ui->btn_obj1_0->setStyleSheet("QPushButton { background-color : " + importer.black + "}");
    ui->btn_obj1_3->setStyleSheet("QPushButton { background-color : " + importer.white + "}");
    ui->btn_window_0->setStyleSheet("QPushButton { background-color : " + importer.black + "}");
    ui->btn_window_3->setStyleSheet("QPushButton { background-color : " + importer.white + "}");

    //Set button action for bringing up color picker
    //connect(ui->btn_bg_0, SIGNAL(clicked()), this, SLOT(on_hexBtn_clicked(QString("btn_bg_0"))));

    // Set button action for generating a file
    //connect(ui->btn_gen_file, SIGNAL(clicked()), this, SLOT(on_genBtn_clicked()));
}

MainWindow::~MainWindow(){
    delete ui;
}

QString MainWindow::gethexColor(){
    QColor color = QColorDialog::getColor(Qt::white, this, tr("Select Color"));
    QString hexColor = color.name();
    return hexColor;
}

void MainWindow::on_btn_import_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Import"), tr(""));
    qDebug() << "Opening file: " << filename;
    APGB_Palette p;
    bool canPopulate = false;
    if(filename.endsWith(".csv")){
        p = this->importer.importPaletteFromCSV(filename.toStdString());
        canPopulate = true;
    }
    else if(filename.endsWith(".pal")){
        p = this->importer.importPaletteFromAPGB(filename.toStdString());
        canPopulate = true;
    }
    else if(filename.endsWith(".txt")){
        p = this->importer.importPaletteJASC(filename.toStdString());
        canPopulate = true;
    }
    else canPopulate = false;

    if(canPopulate == true){
        ui->txt_bg_0->setText(QString::fromStdString(p.bg[0]));
        ui->btn_bg_0->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.bg[0]) + "}");
        ui->txt_bg_1->setText(QString::fromStdString(p.bg[1]));
        ui->btn_bg_1->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.bg[1]) + "}");
        ui->txt_bg_2->setText(QString::fromStdString(p.bg[2]));
        ui->btn_bg_2->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.bg[2]) + "}");
        ui->txt_bg_3->setText(QString::fromStdString(p.bg[3]));
        ui->btn_bg_3->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.bg[3]) + "}");
        ui->txt_obj0_0->setText(QString::fromStdString(p.obj0[0]));
        ui->btn_obj0_0->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.obj0[0]) + "}");
        ui->txt_obj0_1->setText(QString::fromStdString(p.obj0[1]));
        ui->btn_obj0_1->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.obj0[1]) + "}");
        ui->txt_obj0_2->setText(QString::fromStdString(p.obj0[2]));
        ui->btn_obj0_2->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.obj0[2]) + "}");
        ui->txt_obj0_3->setText(QString::fromStdString(p.obj0[3]));
        ui->btn_obj0_3->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.obj0[3]) + "}");
        ui->txt_obj1_0->setText(QString::fromStdString(p.obj1[0]));
        ui->btn_obj1_0->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.obj1[0]) + "}");
        ui->txt_obj1_1->setText(QString::fromStdString(p.obj1[1]));
        ui->btn_obj1_1->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.obj1[1]) + "}");
        ui->txt_obj1_2->setText(QString::fromStdString(p.obj1[2]));
        ui->btn_obj1_2->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.obj1[2]) + "}");
        ui->txt_obj1_3->setText(QString::fromStdString(p.obj1[3]));
        ui->btn_obj1_3->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.obj1[3]) + "}");
        ui->txt_window_0->setText(QString::fromStdString(p.window[0]));
        ui->btn_window_0->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.window[0]) + "}");
        ui->txt_window_1->setText(QString::fromStdString(p.window[1]));
        ui->btn_window_1->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.window[1]) + "}");
        ui->txt_window_2->setText(QString::fromStdString(p.window[2]));
        ui->btn_window_2->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.window[2]) + "}");
        ui->txt_window_3->setText(QString::fromStdString(p.window[3]));
        ui->btn_window_3->setStyleSheet("QPushButton { background-color : " + QString::fromStdString(p.window[3]) + "}");
    }
}

void MainWindow::on_btn_save_clicked()
{
    QString errorMsg;
    QString bg_0 = ui->txt_bg_0->toPlainText();
    QString bg_1 = ui->txt_bg_1->toPlainText();
    QString bg_2 = ui->txt_bg_2->toPlainText();
    QString bg_3 = ui->txt_bg_3->toPlainText();
    QString obj0_0 = ui->txt_obj0_0->toPlainText();
    QString obj0_1 = ui->txt_obj0_1->toPlainText();
    QString obj0_2 = ui->txt_obj0_2->toPlainText();
    QString obj0_3 = ui->txt_obj0_3->toPlainText();
    QString obj1_0 = ui->txt_obj1_0->toPlainText();
    QString obj1_1 = ui->txt_obj1_1->toPlainText();
    QString obj1_2 = ui->txt_obj1_2->toPlainText();
    QString obj1_3 = ui->txt_obj1_3->toPlainText();
    QString window_0 = ui->txt_window_0->toPlainText();
    QString window_1 = ui->txt_window_1->toPlainText();
    QString window_2 = ui->txt_window_2->toPlainText();
    QString window_3 = ui->txt_window_3->toPlainText();

    if(bg_0.isEmpty()) errorMsg += "bg_0\n";
    else if(bg_0.startsWith("#")) bg_0.remove(0,1);
    if(bg_1.isEmpty()) errorMsg += "bg_1\n";
    else if(bg_1.startsWith("#")) bg_1.remove(0,1);
    if(bg_2.isEmpty()) errorMsg += "bg_2\n";
    else if(bg_2.startsWith("#")) bg_2.remove(0,1);
    if(bg_3.isEmpty()) errorMsg += "bg_3\n";
    else if(bg_3.startsWith("#")) bg_3.remove(0,1);
    if(obj0_0.isEmpty()) errorMsg += "obj0_0\n";
    else if(obj0_0.startsWith("#")) obj0_0.remove(0,1);
    if(obj0_1.isEmpty()) errorMsg += "obj0_1\n";
    else if(obj0_1.startsWith("#")) obj0_1.remove(0,1);
    if(obj0_2.isEmpty()) errorMsg += "obj0_2\n";
    else if(obj0_2.startsWith("#")) obj0_2.remove(0,1);
    if(obj0_3.isEmpty()) errorMsg += "obj0_3\n";
    else if(obj0_3.startsWith("#")) obj0_3.remove(0,1);
    if(obj1_0.isEmpty()) errorMsg += "obj1_0\n";
    else if(obj1_0.startsWith("#")) obj1_0.remove(0,1);
    if(obj1_1.isEmpty()) errorMsg += "obj1_1\n";
    else if(obj1_1.startsWith("#")) obj1_1.remove(0,1);
    if(obj1_2.isEmpty()) errorMsg += "obj1_2\n";
    else if(obj1_2.startsWith("#")) obj1_2.remove(0,1);
    if(obj1_3.isEmpty()) errorMsg += "obj1_3\n";
    else if(obj1_3.startsWith("#")) obj1_3.remove(0,1);
    if(window_0.isEmpty()) errorMsg += "window_0\n";
    else if(window_0.startsWith("#")) window_0.remove(0,1);
    if(window_1.isEmpty()) errorMsg += "window_1\n";
    else if(window_0.startsWith("#")) window_1.remove(0,1);
    if(window_2.isEmpty()) errorMsg += "window_2\n";
    else if(window_0.startsWith("#")) window_2.remove(0,1);
    if(window_3.isEmpty()) errorMsg += "window_3\n";
    else if(window_0.startsWith("#")) window_3.remove(0,1);

    qDebug() << "Retrieved data: \n" << bg_0 << " " << bg_1 << " " << bg_2 << " " << bg_3;
    qDebug() << obj0_0 << " " << obj0_1 << " " << obj0_2 << " " << obj0_3;
    qDebug() << obj1_0 << " " << obj1_1 << " " << obj1_2 << " " << obj1_3;
    qDebug() << window_0 << " " << window_1 << " " << window_2 << " " << window_3;
    if(!errorMsg.isEmpty()){
        errorMsg = "The following values are empty:\n" + errorMsg;
        qDebug() << errorMsg;
    }else{
        string *bg = new string[4]{bg_0.toStdString(), bg_1.toStdString(), bg_2.toStdString(), bg_3.toStdString()};
        string *obj0 = new string[4]{obj0_0.toStdString(),obj0_1.toStdString(),obj0_2.toStdString(),obj0_3.toStdString()};
        string *obj1 = new string[4]{obj1_0.toStdString(),obj1_1.toStdString(),obj1_2.toStdString(),obj1_3.toStdString()};
        string *window = new string[4]{window_0.toStdString(), window_1.toStdString(), window_2.toStdString(), window_3.toStdString()};
        APGB_Palette p = this->fsave.getPalette(bg, obj0, obj1, window);

        QString filename = QFileDialog::getSaveFileName(this, tr("Save GB Palette"), tr("Analogue Pocket GB Palette (*.pal)"));
        this->fsave.savePalette(filename.toStdString(), p);
    }
 }

void MainWindow::on_btn_bg_0_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_bg_0->setText(colorData);
    ui->btn_bg_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_bg_1_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_bg_1->setText(colorData);
    ui->btn_bg_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_bg_2_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_bg_2->setText(colorData);
    ui->btn_bg_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_bg_3_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_bg_3->setText(colorData);
    ui->btn_bg_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_txt_bg_0_textChanged()
{
    QString colorData = ui->txt_bg_0->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_bg_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_bg_1_textChanged()
{
    QString colorData = ui->txt_bg_1->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_bg_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_bg_2_textChanged()
{
    QString colorData = ui->txt_bg_2->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_bg_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_bg_3_textChanged()
{
    QString colorData = ui->txt_bg_3->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_bg_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_btn_obj0_0_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj0_0->setText(colorData);
    ui->btn_obj0_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_obj0_1_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj0_1->setText(colorData);
    ui->btn_obj0_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_obj0_2_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj0_2->setText(colorData);
    ui->btn_obj0_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_obj0_3_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj0_3->setText(colorData);
    ui->btn_obj0_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_txt_obj0_0_textChanged()
{
    QString colorData = ui->txt_obj0_0->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_obj0_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_obj0_1_textChanged()
{
    QString colorData = ui->txt_obj0_1->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_obj0_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_obj0_2_textChanged()
{
    QString colorData = ui->txt_obj0_2->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_obj0_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_obj0_3_textChanged()
{
    QString colorData = ui->txt_obj0_3->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_obj0_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_btn_obj1_0_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj1_0->setText(colorData);
    ui->btn_obj1_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_obj1_1_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj1_1->setText(colorData);
    ui->btn_obj1_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_obj1_2_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj1_2->setText(colorData);
    ui->btn_obj1_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_obj1_3_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj1_3->setText(colorData);
    ui->btn_obj1_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_txt_obj1_0_textChanged()
{
    QString colorData = ui->txt_obj1_0->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_obj1_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_obj1_1_textChanged()
{
    QString colorData = ui->txt_obj1_1->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_obj1_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_obj1_2_textChanged()
{
    QString colorData = ui->txt_obj1_2->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_obj1_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_obj1_3_textChanged()
{
    QString colorData = ui->txt_obj1_3->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_obj1_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_btn_window_0_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_window_0->setText(colorData);
    ui->btn_window_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_window_1_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_window_1->setText(colorData);
    ui->btn_window_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_window_2_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_window_2->setText(colorData);
    ui->btn_window_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_btn_window_3_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_window_3->setText(colorData);
    ui->btn_window_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
}

void MainWindow::on_txt_window_0_textChanged()
{
    QString colorData = ui->txt_window_0->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_window_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_window_1_textChanged()
{
    QString colorData = ui->txt_window_1->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_window_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_window_2_textChanged()
{
    QString colorData = ui->txt_window_2->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_window_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_txt_window_3_textChanged()
{
    QString colorData = ui->txt_window_3->toPlainText();
    if(QColor::isValidColorName(colorData)){
        ui->btn_window_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
    }
}

void MainWindow::on_btn_convert_reset_clicked()
{
    ui->txt_convert_load->setText("");
    ui->txt_convert_save->setText("");
}

void MainWindow::on_btn_get_load_clicked()
{
    QString loadFilename = QFileDialog::getOpenFileName(this, tr("Load Palette"), tr(""));
    ui->txt_convert_load->setText(loadFilename);
}

void MainWindow::on_btn_get_save_clicked()
{
    QString saveFilename = QFileDialog::getSaveFileName(this, tr("Save Palette"), tr(""));
    ui->txt_convert_save->setText(saveFilename);
}

void MainWindow::on_btn_convert_save_clicked()
{
    APGB_Palette p;
    QString loadFile = ui->txt_convert_load->toPlainText();
    QString saveFile = ui->txt_convert_save->toPlainText();

    if(!loadFile.isEmpty()){
        if(ui->r_csv->isChecked())        p = this->importer.importPaletteFromCSV(loadFile.toStdString());
        else if(ui->r_jasc->isChecked())  p = this->importer.importPaletteJASC(loadFile.toStdString());

        if(!saveFile.isEmpty()){
            this->fsave.savePalette(saveFile.toStdString(), p);
            ui->txt_convert_load->setText("");
            ui->txt_convert_save->setText("");
        }
        else qDebug() << "No save path was given.";
    }
    else qDebug() << "No load path was given.";
}

