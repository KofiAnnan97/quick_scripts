#include "include/mainwindow.h"
#include "./ui/ui_mainwindow.h"
#include <QTextEdit>
#include <QColorDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QColor>
#include <QImage>
#include <QString>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <string>
#include <QMessageBox>
#include <QDir>


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up dynamic GB palette viewer
    scene = new QGraphicsScene(this);
    ui->view_img->setScene(scene);

    // Initialize editor hexadecimal values
    this->initialize_editor_values();

    // Initialize image view
    image = new QImage(this->imgImporter.width, this->imgImporter.height, QImage::Format_RGB666);
    this->update_image_view();

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

void MainWindow::initialize_editor_values(){
    // Set text fields for darkest and brightest hex values on startup
    ui->txt_bg_0->setText(fImporter.black);
    ui->txt_bg_1->setText(fImporter.dark);
    ui->txt_bg_2->setText(fImporter.light);
    ui->txt_bg_3->setText(fImporter.white);
    ui->txt_obj0_0->setText(fImporter.black);
    ui->txt_obj0_1->setText(fImporter.dark);
    ui->txt_obj0_2->setText(fImporter.light);
    ui->txt_obj0_3->setText(fImporter.white);
    ui->txt_obj1_0->setText(fImporter.black);
    ui->txt_obj1_1->setText(fImporter.dark);
    ui->txt_obj1_2->setText(fImporter.light);
    ui->txt_obj1_3->setText(fImporter.white);
    ui->txt_window_0->setText(fImporter.black);
    ui->txt_window_1->setText(fImporter.dark);
    ui->txt_window_2->setText(fImporter.light);
    ui->txt_window_3->setText(fImporter.white);
}

vector<QString> MainWindow::getChosenPalettes(){
    vector<QString> temp;
    temp.push_back(ui->txt_bg_0->toPlainText());
    temp.push_back(ui->txt_bg_1->toPlainText());
    temp.push_back(ui->txt_bg_2->toPlainText());
    temp.push_back(ui->txt_bg_2->toPlainText());
    temp.push_back(ui->txt_obj0_0->toPlainText());
    temp.push_back(ui->txt_obj0_1->toPlainText());
    temp.push_back(ui->txt_obj0_2->toPlainText());
    temp.push_back(ui->txt_obj0_3->toPlainText());
    temp.push_back(ui->txt_obj1_0->toPlainText());
    temp.push_back(ui->txt_obj1_1->toPlainText());
    temp.push_back(ui->txt_obj1_2->toPlainText());
    temp.push_back(ui->txt_obj1_3->toPlainText());
    temp.push_back(ui->txt_window_0->toPlainText());
    temp.push_back(ui->txt_window_1->toPlainText());
    temp.push_back(ui->txt_window_2->toPlainText());
    temp.push_back(ui->txt_window_3->toPlainText());
    return temp;
}

void MainWindow::on_btn_import_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Import"), tr(""));
    APGB_Palette p;
    bool canPopulate = false;
    string fn = filename.toStdString();
    if(filename.endsWith(".csv")){
        p = this->fImporter.importPaletteFromCSV(fn);
        canPopulate = true;
    }
    else if(filename.endsWith(".pal")){
        if(this->fImporter.isJASCFormat(fn)){
            p = this->fImporter.importPaletteJASC(filename.toStdString());
            canPopulate = true;
        }
        else if(this->fImporter.isAPGBFormat(fn)){
            p = this->fImporter.importPaletteFromAPGB(filename.toStdString());
            canPopulate = true;
        }
        else QMessageBox::critical(this, "Format Error", "There was an issue parsing " + filename);
    }
    else QMessageBox::critical(this, "File Type Error", filename + " must use one of the following extensions:\n\t.csv\n\t.pal");

    if(canPopulate == true){
        ui->txt_bg_0->setText(QString::fromStdString(p.bg[0]));
        ui->txt_bg_1->setText(QString::fromStdString(p.bg[1]));
        ui->txt_bg_2->setText(QString::fromStdString(p.bg[2]));
        ui->txt_bg_3->setText(QString::fromStdString(p.bg[3]));
        ui->txt_obj0_0->setText(QString::fromStdString(p.obj0[0]));
        ui->txt_obj0_1->setText(QString::fromStdString(p.obj0[1]));
        ui->txt_obj0_2->setText(QString::fromStdString(p.obj0[2]));
        ui->txt_obj0_3->setText(QString::fromStdString(p.obj0[3]));
        ui->txt_obj1_0->setText(QString::fromStdString(p.obj1[0]));
        ui->txt_obj1_1->setText(QString::fromStdString(p.obj1[1]));
        ui->txt_obj1_2->setText(QString::fromStdString(p.obj1[2]));
        ui->txt_obj1_3->setText(QString::fromStdString(p.obj1[3]));
        ui->txt_window_0->setText(QString::fromStdString(p.window[0]));
        ui->txt_window_1->setText(QString::fromStdString(p.window[1]));
        ui->txt_window_2->setText(QString::fromStdString(p.window[2]));
        ui->txt_window_3->setText(QString::fromStdString(p.window[3]));
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

    if(!errorMsg.isEmpty()){
        errorMsg = "The following values are empty:\n" + errorMsg;
        QMessageBox::critical(this, "Data Error", errorMsg);
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

void MainWindow::update_image_view(){
    //qDebug() << QCoreApplication::applicationDirPath();
    vector<QString> temp = this->getChosenPalettes();
    // Find a more clever way of retrieving this file from a folder
    QString encHeximgPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator() + ".." + QDir::separator() + ".." + QDir::separator() + "img");
    string filename = QDir::cleanPath(encHeximgPath + QDir::separator() + "test.txt").toStdString();
    //string filename = "./../../img/test.txt";
    this->imgImporter.decodeImageTxt(filename, image, temp);
    scene->addPixmap(QPixmap::fromImage(*image));
}

void MainWindow::on_btn_bg_0_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_bg_0->setText(colorData);
}

void MainWindow::on_btn_bg_1_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_bg_1->setText(colorData);
}

void MainWindow::on_btn_bg_2_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_bg_2->setText(colorData);
}

void MainWindow::on_btn_bg_3_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_bg_3->setText(colorData);
}

void MainWindow::on_txt_bg_0_textChanged()
{
    QString colorData = ui->txt_bg_0->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_bg_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_bg_1_textChanged()
{
    QString colorData = ui->txt_bg_1->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_bg_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_bg_2_textChanged()
{
    QString colorData = ui->txt_bg_2->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_bg_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_bg_3_textChanged()
{
    QString colorData = ui->txt_bg_3->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_bg_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_btn_obj0_0_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj0_0->setText(colorData);
}

void MainWindow::on_btn_obj0_1_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj0_1->setText(colorData);
}

void MainWindow::on_btn_obj0_2_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj0_2->setText(colorData);
}

void MainWindow::on_btn_obj0_3_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj0_3->setText(colorData);
}

void MainWindow::on_txt_obj0_0_textChanged()
{
    QString colorData = ui->txt_obj0_0->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_obj0_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_obj0_1_textChanged()
{
    QString colorData = ui->txt_obj0_1->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_obj0_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_obj0_2_textChanged()
{
    QString colorData = ui->txt_obj0_2->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_obj0_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_obj0_3_textChanged()
{
    QString colorData = ui->txt_obj0_3->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_obj0_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_btn_obj1_0_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj1_0->setText(colorData);
}

void MainWindow::on_btn_obj1_1_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj1_1->setText(colorData);
}

void MainWindow::on_btn_obj1_2_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj1_2->setText(colorData);
}

void MainWindow::on_btn_obj1_3_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_obj1_3->setText(colorData);
}

void MainWindow::on_txt_obj1_0_textChanged()
{
    QString colorData = ui->txt_obj1_0->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_obj1_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_obj1_1_textChanged()
{
    QString colorData = ui->txt_obj1_1->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_obj1_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_obj1_2_textChanged()
{
    QString colorData = ui->txt_obj1_2->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_obj1_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_obj1_3_textChanged()
{
    QString colorData = ui->txt_obj1_3->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_obj1_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_btn_window_0_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_window_0->setText(colorData);
}

void MainWindow::on_btn_window_1_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_window_1->setText(colorData);
}

void MainWindow::on_btn_window_2_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_window_2->setText(colorData);
}

void MainWindow::on_btn_window_3_clicked()
{
    QString colorData = this->gethexColor();
    ui->txt_window_3->setText(colorData);
}

void MainWindow::on_txt_window_0_textChanged()
{
    QString colorData = ui->txt_window_0->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_window_0->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_window_1_textChanged()
{
    QString colorData = ui->txt_window_1->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_window_1->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
    }
}

void MainWindow::on_txt_window_2_textChanged()
{
    QString colorData = ui->txt_window_2->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_window_2->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();

    }
}

void MainWindow::on_txt_window_3_textChanged()
{
    QString colorData = ui->txt_window_3->toPlainText();
    if(QColor(colorData).isValid()){
        ui->btn_window_3->setStyleSheet("QPushButton { background-color : " + colorData + "}");
        this->update_image_view();
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
        if(ui->r_csv->isChecked())        p = this->fImporter.importPaletteFromCSV(loadFile.toStdString());
        else if(ui->r_jasc->isChecked())  p = this->fImporter.importPaletteJASC(loadFile.toStdString());

        if(!saveFile.isEmpty()){
            this->fsave.savePalette(saveFile.toStdString(), p);
            ui->txt_convert_load->setText("");
            ui->txt_convert_save->setText("");
        }
        else {
            QMessageBox::critical(this, "File Error", "No destination filepath was given.");
        }
    }
    else {
        QMessageBox::critical(this, "File Error", "No source filepath was given.");
    }
}

