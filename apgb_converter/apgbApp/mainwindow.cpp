#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTextEdit>
#include <QColorDialog>
#include <QString>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    darkest = "#000000";
    brightest = "#FFFFFF";

    footer = "8141504742";
    lcdOff = "FFFFFF";

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

int* MainWindow::fromStrToHexStrArr(string str, int numOfBytes){
    int *hexArr = new int[numOfBytes];
    //if(strcmp(str.substr(0, 2).c_str(), "0x") == 0) str.erase(str.begin(), str.begin()+2);
    for(int i = 0; i < numOfBytes;++i){
      string segment = str.substr(i*2, 2);
      stringstream ss;
      ss << std::hex << segment;
      ss >> hexArr[i];
    }
    return hexArr;
}

string MainWindow::fromHexToStr(string hexStr){
    std::ostringstream ss; //(hexStr);
    //ss << static_cast<int>(temp);
    //ss >> std::hex >> i;

    //qDebug() << i;
    //stringstream ss;
    //ss << std::hex << hexStr;
    string str;
    for (size_t i = 0; i < hexStr.length(); i += 1) {
        char byte = hexStr.substr(i, 1).c_str()[0];
        //qDebug() << byte;
        ss << static_cast<int>(byte);
        string temp = ss.str();
       // qDebug() << temp;
        //char ch = (int)strtol(byte.c_str(), nullptr, 16);
        //str.push_back(ch);
    }
    //return str;
    return hexStr.c_str();
}

string MainWindow::trim(string data){
    int startIdx = 0;
    int endIdx = data.length()-1;
    while(data[startIdx] == ' ') startIdx++;
    while(data[endIdx] == ' ') endIdx--;
    return data.substr(startIdx, endIdx-startIdx+1);
}

void MainWindow::clearBuffer(char *buffer, int size){
    for(int j = 0; j < size; j++) buffer[j] = 0x00;
}

bool MainWindow::isKeyword(string word){
    vector<string> keywords = {"BG", "OBJ0", "OBJ1", "WINDOW", "bg", "obj0", "obj1", "window"};
    for(int i = 0; i < keywords.size(); i++){
        if(word == keywords[i]) return true;
    }
    return false;
}

APGB_Palette MainWindow::importPaletteFromCSV(string filename){
    APGB_Palette ap;
    ap.bg = new string[4];
    ap.obj0 = new string[4];
    ap.obj1 = new string[4];
    ap.window = new string[4];
    fstream csv;
    csv.open(filename, ios::in);
    if(csv.is_open()){
        vector<string> row;
        string line, word, temp;
        while(csv >> temp){
            row.clear();
            getline(csv, line);
            stringstream s(line);
            while (getline(s, word, ',')){
                string trimmedWord = this->trim(word);
                if(!this->isKeyword(trimmedWord) && trimmedWord.substr(0, 2) == "0x"){
                    trimmedWord.erase(0, 2);
                    trimmedWord = "#" + trimmedWord;
                }else if(!this->isKeyword(trimmedWord) && trimmedWord[0] != '#' && trimmedWord.size() == 6){
                    trimmedWord = "#" + trimmedWord;
                }
                row.push_back(trimmedWord);
            }
            string *pt;
            auto pType = row[0].c_str();
            if(strcmp(pType , "BG") == 0 || strcmp(pType ,"bg") == 0)              pt = ap.bg;
            else if(strcmp(pType , "OBJ0") == 0 || strcmp(pType ,"obj0") == 0)     pt = ap.obj0;
            else if(strcmp(pType , "OBJ1") == 0 || strcmp(pType ,"obj1") == 0)     pt = ap.obj1;
            else if(strcmp(pType , "WINDOW") == 0 || strcmp(pType ,"window") == 0) pt = ap.window;
            pt[0] = row[1];
            pt[1] = row[2];
            pt[2] = row[3];
            pt[3] = row[4];
        }
    }
    else qDebug() << "Failed to find file: " << filename;
    return ap;
}

APGB_Palette MainWindow::importPaletteFromPAL(string filename){
    APGB_Palette ap;
    ap.bg = new string[4];
    ap.obj0 = new string[4];
    ap.obj1 = new string[4];
    ap.window = new string[4];
    ifstream pal;
    pal.open(filename, ios::binary | ios::in);
    unsigned int x;
    if(pal.is_open()){
        pal.read(reinterpret_cast<char*>(&x), sizeof(unsigned int));
        qDebug() << x;
        /*string line, word, temp;
        while(pal >> temp){
            stringstream ss;
            ss << std::hex << temp;
            ss >> line;
            qDebug() << line << " " << line.size();
            if(line.size() == 56){
                line.erase(48,8);
                qDebug() << line[0];
                unsigned int result = (line[0] << 16) | (line[1] << 8) | (line[2]);
                qDebug() << result;
                ap.bg[0] = "#"+this->fromHexToStr(line.substr(9, 3));
                //qDebug() << ap.bg[0];
                ap.bg[1] = "#"+line.substr(3, 3);
                ap.bg[2] = "#"+line.substr(6, 3);
                ap.bg[3] = "#"+line.substr(9, 3);
                ap.obj0[0] = "#"+line.substr(12,3);
                ap.obj0[1] = "#"+line.substr(15,3);
                ap.obj0[2] = "#"+line.substr(18,3);
                ap.obj0[3] = "#"+line.substr(21,3);
                ap.obj1[0] = "#"+line.substr(24,3);
                ap.obj1[1] = "#"+line.substr(27,3);
                ap.obj1[2] = "#"+line.substr(30,3);
                ap.obj1[3] = "#"+line.substr(33,3);
                ap.window[0] = "#"+line.substr(36,3);
                ap.window[1] = "#"+line.substr(39,3);
                ap.window[2] = "#"+line.substr(42,3);
                ap.window[3] = "#"+line.substr(45,3);
                break;
            }
        }*/
        pal.close();
    }
    else qDebug() << "Failed to find file: " << filename;
    return ap;
}

APGB_Palette MainWindow::importPaletteJASCTxt(string filename){
    APGB_Palette ap;
    ap.bg = new string[4];
    ap.obj0 = new string[4];
    ap.obj1 = new string[4];
    ap.window = new string[4];
    vector<QString> colorVals;
    ifstream txt;
    string jascId, jascVersion,line;
    int numOfPalettes;
    string paletteNumError = "This script only supports 4, 8 or 16 color palettes for the JASC PAL format.";
    txt.open(filename);
    if(txt.is_open()){
        getline(txt, jascId);
        getline(txt, jascVersion);
        getline(txt, line);
        numOfPalettes = stoi(line);
        //qDebug() << jascId.erase(jascId.size()-1, 1) << " " << jascVersion.erase(jascVersion.size()-1, 1) << " " << numOfPalettes;
        if(numOfPalettes != 4 && numOfPalettes != 8 && numOfPalettes != 16){
            qDebug() << paletteNumError;
        }
        else{
            while(getline(txt, line)){
                line.erase(line.size()-1, 1);
                stringstream ss(line);
                string val;
                getline(ss, val, ' ');
                int r = stoi(val);
                getline(ss, val, ' ');
                int g = stoi(val);
                getline(ss, val, ' ');
                int b = stoi(val);
                colorVals.push_back(QColor(r, g, b).name());
            }
            if(numOfPalettes != colorVals.size()){
                qDebug() << "The number of color palettes are configured incorrectly. Please check the file.";
            }
            else if(numOfPalettes == 4){
                ap.bg[0] = this->darkest.toStdString();
                ap.bg[1] = colorVals[0].toStdString();
                ap.bg[2] = colorVals[0].toStdString();
                ap.bg[3] = this->brightest.toStdString();
                ap.obj0[0] = this->darkest.toStdString();
                ap.obj0[1] = colorVals[1].toStdString();
                ap.obj0[2] = colorVals[1].toStdString();
                ap.obj0[3] = this->brightest.toStdString();
                ap.obj1[0] = this->darkest.toStdString();
                ap.obj1[1] = colorVals[2].toStdString();
                ap.obj1[2] = colorVals[2].toStdString();
                ap.obj1[3] = this->brightest.toStdString();
                ap.window[0] = this->darkest.toStdString();
                ap.window[1] = colorVals[3].toStdString();
                ap.window[2] = colorVals[3].toStdString();
                ap.window[3] = this->brightest.toStdString();
            }
            else if(numOfPalettes == 8){
                ap.bg[0] = this->darkest.toStdString();
                ap.bg[1] = colorVals[0].toStdString();
                ap.bg[2] = colorVals[1].toStdString();
                ap.bg[3] = this->brightest.toStdString();
                ap.obj0[0] = this->darkest.toStdString();
                ap.obj0[1] = colorVals[2].toStdString();
                ap.obj0[2] = colorVals[3].toStdString();
                ap.obj0[3] = this->brightest.toStdString();
                ap.obj1[0] = this->darkest.toStdString();
                ap.obj1[1] = colorVals[4].toStdString();
                ap.obj1[2] = colorVals[5].toStdString();
                ap.obj1[3] = this->brightest.toStdString();
                ap.window[0] = this->darkest.toStdString();
                ap.window[1] = colorVals[6].toStdString();
                ap.window[2] = colorVals[7].toStdString();
                ap.window[3] = this->brightest.toStdString();
            }else if(numOfPalettes == 16){
                ap.bg[0] = colorVals[0].toStdString();
                ap.bg[1] = colorVals[1].toStdString();
                ap.bg[2] = colorVals[2].toStdString();
                ap.bg[3] = colorVals[3].toStdString();
                ap.obj0[0] = colorVals[4].toStdString();
                ap.obj0[1] = colorVals[5].toStdString();
                ap.obj0[2] = colorVals[6].toStdString();
                ap.obj0[3] = colorVals[7].toStdString();
                ap.obj1[0] = colorVals[8].toStdString();
                ap.obj1[1] = colorVals[9].toStdString();
                ap.obj1[2] = colorVals[10].toStdString();
                ap.obj1[3] = colorVals[11].toStdString();
                ap.window[0] = colorVals[12].toStdString();
                ap.window[1] = colorVals[13].toStdString();
                ap.window[2] = colorVals[14].toStdString();
                ap.window[3] = colorVals[15].toStdString();
            }
        }
        txt.close();
    }
    else qDebug() << "Failed to find file: " << filename;
    return ap;
}

APGB_Palette MainWindow::importPaletteJASCHex(string filename){
    APGB_Palette ap;
    return ap;
}

char* MainWindow::APGBFormat(APGB_Palette p, int size){
    string data = "";
    for(int a = 0; a < 4; a++){
      if(strcmp(p.bg[a].substr(0, 2).c_str(), "0x") == 0){
        p.bg[a].erase(p.bg[a].begin(), p.bg[a].begin()+2);
      }
      data += p.bg[a];
    } 
    for(int c = 0; c < 4; c++){
      if(strcmp(p.obj0[c].substr(0, 2).c_str(), "0x") == 0){
        p.obj0[c].erase(p.obj0[c].begin(), p.obj0[c].begin()+2);
      }
      data += p.obj0[c];
    } 
    for(int e = 0; e < 4; e++){
      if(strcmp(p.obj1[e].substr(0, 2).c_str(), "0x") == 0){
        p.obj1[e].erase(p.obj1[e].begin(), p.obj1[e].begin()+2);
      }
      data += p.obj1[e];
    }
    for(int g = 0; g < 4; g++) {
      if(strcmp(p.window[g].substr(0, 2).c_str(), "0x") == 0){
        p.window[g].erase(p.window[g].begin(), p.window[g].begin()+2);
      }
      data += p.window[g];
    }
    data += lcdOff + footer;
    qDebug() << data;

    int *temp = fromStrToHexStrArr(data, size);
    char *buffer = new char[size];
    clearBuffer(buffer, size);
    for(int i = 0; i < size; i++) buffer[i] = temp[i];
    return buffer;
} 

APGB_Palette MainWindow::getPalette(string *bg, string *obj0, string *obj1, string *window){
    APGB_Palette pal;
    pal.bg = bg;
    pal.obj0 = obj0;
    pal.obj1 = obj1;
    pal.window = window;
    return pal;
}

void MainWindow::savePalette(string filename, APGB_Palette p){
    std::ofstream df(filename, std::ios_base::binary | std::ios_base::out);
    if(df.is_open()){
        char *buffer = this->APGBFormat(p, p.numOfBytes);
        df.write(buffer, p.numOfBytes);
        df.close();
    }
    else qDebug() << "Failed to create file: " << filename;
}

void MainWindow::on_btn_import_clicked()
{
    //QString filename = QFileDialog::getOpenFileName(this, tr("Import"), tr(""));
    QString filename = "/home/eglinux/Github/quick_scripts/apgb_converter/jasc_text8.txt";
    qDebug() << "Opening file: " << filename;
    APGB_Palette p;
    bool canPopulate = false;
    if(filename.endsWith(".csv")){
        p = this->importPaletteFromCSV(filename.toStdString());
        canPopulate = true;
    }

    else if(filename.endsWith(".txt")){
        //p = this->importPaletteFromPAL(filename.toStdString());
        p = this->importPaletteJASCTxt(filename.toStdString());
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
    else bg_0.remove(0,1);
    if(bg_1.isEmpty()) errorMsg += "bg_1\n";
    else bg_1.remove(0,1);
    if(bg_2.isEmpty()) errorMsg += "bg_2\n";
    else bg_2.remove(0,1);
    if(bg_3.isEmpty()) errorMsg += "bg_3\n";
    else bg_3.remove(0,1);
    if(obj0_0.isEmpty()) errorMsg += "obj0_0\n";
    else obj0_0.remove(0,1);
    if(obj0_1.isEmpty()) errorMsg += "obj0_1\n";
    else obj0_1.remove(0,1);
    if(obj0_2.isEmpty()) errorMsg += "obj0_2\n";
    else obj0_2.remove(0,1);
    if(obj0_3.isEmpty()) errorMsg += "obj0_3\n";
    else obj0_3.remove(0,1);
    if(obj1_0.isEmpty()) errorMsg += "obj1_0\n";
    else obj1_0.remove(0,1);
    if(obj1_1.isEmpty()) errorMsg += "obj1_1\n";
    else obj1_1.remove(0,1);
    if(obj1_2.isEmpty()) errorMsg += "obj1_2\n";
    else obj1_2.remove(0,1);
    if(obj1_3.isEmpty()) errorMsg += "obj1_3\n";
    else obj1_3.remove(0,1);
    if(window_0.isEmpty()) errorMsg += "window_0\n";
    else window_0.remove(0,1);
    if(window_1.isEmpty()) errorMsg += "window_1\n";
    else window_1.remove(0,1);
    if(window_2.isEmpty()) errorMsg += "window_2\n";
    else window_2.remove(0,1);
    if(window_3.isEmpty()) errorMsg += "window_3\n";
    else window_3.remove(0,1);

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
        APGB_Palette p = this->getPalette(bg, obj0, obj1, window);

        QString filename = QFileDialog::getSaveFileName(this, tr("Save GB Palette"), tr("Analogue Pocket GB Palette (*.pal)"));
        this->savePalette(filename.toStdString(), p);
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
    ui->r_csv->clicked(true);
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
        if(ui->r_csv->isChecked())           p = this->importPaletteFromCSV(loadFile.toStdString());
        else if(ui->r_jasc_hex->isChecked()) p = this->importPaletteJASCHex(loadFile.toStdString());
        else if(ui->r_jasc_txt->isChecked()) p = this->importPaletteJASCTxt(loadFile.toStdString());

        if(!saveFile.isEmpty()) this->savePalette(saveFile.toStdString(), p);
        else qDebug() << "No save path was given.";
    }
    else qDebug() << "No load path was given.";
}

