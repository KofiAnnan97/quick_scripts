#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTextEdit>
#include <QColorDialog>
#include <QString>
#include <QDebug>
#include <QDialog>

using namespace std; 

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString darkest = "#000000";
    QString brightest = "#FFFFFF";

    // Set text fields for darkest and brightest hex values on startup
    ui->txt_bg_0->setText(darkest);
    ui->txt_bg_3->setText(brightest);
    ui->txt_obj0_0->setText(darkest);
    ui->txt_obj0_3->setText(brightest);
    ui->txt_obj1_0->setText(darkest);
    ui->txt_obj1_3->setText(brightest);
    ui->txt_window_0->setText(darkest);
    ui->txt_window_3->setText(brightest);

    // Set button color for darkest and brightest hex values on startup
    ui->btn_bg_0->setStyleSheet("QPushButton { background-color : " + darkest + "}");
    ui->btn_bg_3->setStyleSheet("QPushButton { background-color : " + brightest + "}");
    ui->btn_obj0_0->setStyleSheet("QPushButton { background-color : " + darkest + "}");
    ui->btn_obj0_3->setStyleSheet("QPushButton { background-color : " + brightest + "}");
    ui->btn_obj1_0->setStyleSheet("QPushButton { background-color : " + darkest + "}");
    ui->btn_obj1_3->setStyleSheet("QPushButton { background-color : " + brightest + "}");
    ui->btn_window_0->setStyleSheet("QPushButton { background-color : " + darkest + "}");
    ui->btn_window_3->setStyleSheet("QPushButton { background-color : " + brightest + "}");

    //Set button action for bringing up color picker
    //connect(ui->btn_bg_0, SIGNAL(clicked()), this, SLOT(on_hexBtn_clicked(QString("btn_bg_0"))));

    // Set button action for generating a file
    //connect(ui->btn_gen_file, SIGNAL(clicked()), this, SLOT(on_genBtn_clicked()));
}

QString MainWindow::gethexColor(){
    QColor color = QColorDialog::getColor(Qt::white, this, tr("Select Color"));
    QString hexColor = color.name();
    return hexColor;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_gen_file_clicked()
{
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

    qDebug() << "Retrieved data: \n" << bg_0 << " " << bg_1 << " " << bg_2 << " " << bg_3;
    qDebug() << obj0_0 << " " << obj0_1 << " " << obj0_2 << " " << obj0_3;
    qDebug() << obj1_0 << " " << obj1_1 << " " << obj1_2 << " " << obj1_3;
    qDebug() << window_0 << " " << window_1 << " " << window_2 << " " << window_3;

    QString errorMsg = "The following values are empty:\n";
    if(bg_0.isEmpty()) errorMsg += "bg_0\n";
    if(bg_1.isEmpty()) errorMsg += "bg_1\n";
    if(bg_2.isEmpty()) errorMsg += "bg_2\n";
    if(bg_3.isEmpty()) errorMsg += "bg_3\n";
    if(obj0_0.isEmpty()) errorMsg += "obj0_0\n";
    if(obj0_1.isEmpty()) errorMsg += "obj0_1\n";
    if(obj0_2.isEmpty()) errorMsg += "obj0_2\n";
    if(obj0_3.isEmpty()) errorMsg += "obj0_3\n";
    if(obj1_0.isEmpty()) errorMsg += "obj1_0\n";
    if(obj1_1.isEmpty()) errorMsg += "obj1_1\n";
    if(obj1_2.isEmpty()) errorMsg += "obj1_2\n";
    if(obj1_3.isEmpty()) errorMsg += "obj1_3\n";
    if(window_0.isEmpty()) errorMsg += "window_0\n";
    if(window_1.isEmpty()) errorMsg += "window_1\n";
    if(window_2.isEmpty()) errorMsg += "window_2\n";
    if(window_3.isEmpty()) errorMsg += "obj0_3\n";
    qDebug() << errorMsg;

    //bg_0.remove(0, 1);
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
