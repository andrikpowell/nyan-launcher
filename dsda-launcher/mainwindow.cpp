#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QStandardPaths>
#include <QShortcut>
#include <bootstrap.h>
#include "bootstrap.h"

std::string getOsName()
{
    #ifdef _WIN32
    return "Windows";
    #elif _WIN64
    return "Windows";
    #elif __APPLE__ || __MACH__
    return "MacOS";
    #else
    return "Linux";
    #endif
}

QStringList images;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);





    QShortcut * shortcut = new QShortcut(QKeySequence(Qt::Key_O | Qt::CTRL),this,SLOT(foo()));
    shortcut->setAutoRepeat(false);

    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->demoPlayOptions->hide();

    if(getOsName()=="MacOS" || getOsName()=="Linux")
    {
        system(("mkdir "+QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString()+"/.dsda-doom").c_str());
        system(("cp "+QCoreApplication::applicationDirPath().toStdString()+"/../Resources/dsda-doom.wad "+QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString()+"/.dsda-doom").c_str());

        QDir directory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.dsda-doom");
        images = directory.entryList(QStringList() << "*.WAD",QDir::Files);

    }
    else
    {
        QDir directory = QDir::currentPath();
        images = directory.entryList(QStringList() << "*.WAD",QDir::Files);

        ui->pushButton_2->setStyleSheet("color: rgb(50, 50, 50);");
    }

    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="doom" || filename=="DOOM")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="doom1" || filename=="DOOM1")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="doomu"|| filename=="DOOMU")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="doom2"|| filename=="DOOM2")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="doom2f"|| filename=="DOOM2f"|| filename=="DOOM2F")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="tnt"|| filename=="TNT")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="plutonia"|| filename=="PLUTONIA"|| filename=="Plutonia")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="freedoom1"|| filename=="FREEDOOM1"|| filename=="freedoom"|| filename=="FreeDoom1"|| filename=="FreeDoom"|| filename=="Freedoom1"|| filename=="Freedoom")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="freedoom2"|| filename=="FREEDOOM2"|| filename=="FreeDoom2"|| filename=="Freedoom2")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="heretic"|| filename=="HERETIC"|| filename=="Heretic")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="bfgdoom1"|| filename=="BFGDOOM1"|| filename=="bfgDOOM1"|| filename=="BfgDoom1"|| filename=="BFGdoom1")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="bfgdoom2"|| filename=="BFGDOOM2"|| filename=="bfgDOOM2"|| filename=="BfgDoom2"|| filename=="BFGdoom2")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="chex"||filename=="CHEX"||filename=="Chex")
        {
            ui->iwadSelect->addItems({filename});
        }
    }
    foreach(QString filename, images) {
        filename.resize (filename.size () - 4);
        if(filename=="hacx"||filename=="HACX"||filename=="Hacx")
        {
            ui->iwadSelect->addItems({filename});
        }
    }

    if(ui->iwadSelect->count()==0)
    {
        ui->toolTip->show();
    }
    else
    {
        ui->toolTip->hide();
    }

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::foo()
{
    if(getOsName()=="MacOS"|| getOsName()=="Linux")
    {
        system(("open "+QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString()+"/.dsda-doom").c_str());
    }
    else
    {
        system(("open "+QCoreApplication::applicationDirPath().toStdString()).c_str());
    }
}

void MainWindow::addWads(QStringList fileNames)
{
    ui->wadsOnFolder->addItems(fileNames);
}


std::string arguments = " ";
bool isFast=false;
bool noMo =false;
bool isRespawn=false;
std::string isFulscreen="w";
bool isSoloNet = false;


void MainWindow::on_LaunchGameButton_clicked()
{

    int complevelIndex = ui->compLevelSelect->currentIndex();
    if(complevelIndex==1)
    {
        arguments+=" -complevel 0 ";
    }
    else if(complevelIndex==2)
    {
        arguments+=" -complevel 1 ";
    }
    else if(complevelIndex==3)
    {
        arguments+=" -complevel 2 ";
    }
    else if(complevelIndex==4)
    {
        arguments+=" -complevel 3 ";
    }
    else if(complevelIndex==5)
    {
        arguments+=" -complevel 4 ";
    }
    else if(complevelIndex==6)
    {
        arguments+=" -complevel 5 ";
    }
    else if(complevelIndex==7)
    {
        arguments+=" -complevel 6 ";
    }
    else if(complevelIndex==8)
    {
        arguments+=" -complevel 7 ";
    }
    else if(complevelIndex==9)
    {
        arguments+=" -complevel 10 ";
    }
    else if(complevelIndex==10)
    {
        arguments+=" -complevel 11 ";
    }
    else if(complevelIndex==11)
    {
        arguments+=" -complevel 16 ";
    }
    else if(complevelIndex==12)
    {
        arguments+=" -complevel 17 ";
    }

    int diffIndex = ui->diffBox->currentIndex();
    if(diffIndex==1)
    {
        arguments+=" -skill 1 ";
    }
    if(diffIndex==2)
    {
        arguments+=" -skill 2 ";
    }
    if(diffIndex==3)
    {
        arguments+=" -skill 3 ";
    }
    if(diffIndex==4)
    {
        arguments+=" -skill 4 ";
    }
    if(diffIndex==5)
    {
        arguments+=" -skill 5 ";
    }



    if(ui->levelBox->text().toStdString()!= "" && !ui->levelBox->isHidden())
    {
        arguments += " -warp "+ui->episodeBox->text().toStdString()+" "+ui->levelBox->text().toStdString();
    }
    else if(ui->episodeBox->text().toStdString()!= "" && ui->levelBox->isHidden())
    {
        arguments += " -warp "+ui->episodeBox->text().toStdString();
    }

    qDebug() <<  ui->wadsOnFolder->count();

    for(int item=0;item < ui->wadsOnFolder->count(); item++)
    {
        if(ui->wadsOnFolder->item(item)->text().toStdString().back()=='h')
        {
            arguments += " -deh '" + ui->wadsOnFolder->item(item)->text().toStdString()+"' ";
        }
        else
        {
            arguments += " -file '" + ui->wadsOnFolder->item(item)->text().toStdString()+"' ";
        }
    }


    if(isFast)
    {
        arguments += " -fast ";
    }
    if(noMo)
    {
        arguments += " -nomonsters ";
    }
    if(isRespawn)
    {
        arguments += " -respawn ";
    }

    if(ui->comboBox_2->currentIndex()==0)
    {
        if(isFulscreen=="w")
        {
            arguments += " -window ";
        }
        else
        {
            arguments += " -fs ";
        }
    }
    else
    {
        arguments += " -geom "+ ui->comboBox_2->currentText().toStdString()+isFulscreen;
    }
    if(isSoloNet)
    {
        arguments += " -solo-net";
    }
    if(ui->recordDemo->text().size()>5)
    {
        arguments += " -record "+ui->recordDemo->text().toStdString();
    }
    if(ui->recordDemo_2->text().size()>5)
    {
        if(ui->demoPlayOptions->currentIndex()==0)
        {
            arguments += " -playdemo "+ui->recordDemo_2->text().toStdString();
        }
        else if(ui->demoPlayOptions->currentIndex()==1)
        {
            arguments += " -timedemo "+ui->recordDemo_2->text().toStdString();
        }
        else if(ui->demoPlayOptions->currentIndex()==2)
        {
            arguments += " -fastdemo "+ui->recordDemo_2->text().toStdString();
        }
    }



    arguments += " " + ui->argumentText->toPlainText().toStdString();
    std::replace(arguments.begin(), arguments.end(), '\n', ' ');


    if(getOsName()=="MacOS" || getOsName()=="Linux")
    {
        std::string homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString();
        std::string execPath = QCoreApplication::applicationDirPath().toStdString();
        system(("rm "+homePath+"/.dsda-doom/LogFile.txt").c_str());
        system((execPath+"/../Resources/dsda-doom -iwad "+homePath+"/.dsda-doom/"+ui->iwadSelect->currentText().toStdString()+".wad "+arguments+" >> "+homePath+"/.dsda-doom/LogFile.txt").c_str());
        arguments=" ";
    }
    else
    {
        system(("dsda-doom -iwad "+ui->iwadSelect->currentText().toStdString()+".wad "+arguments+" >> LogFile.txt").c_str());
        arguments=" ";
    }


}



void MainWindow::on_iwadSelect_currentIndexChanged(int index)
{
    QString sel = ui->iwadSelect->currentText();
    if(sel=="doom"||sel=="DOOM"||sel=="doomu"||sel=="DOOMU"||sel=="doom1"||sel=="DOOM1"||sel=="freedoom1"||sel=="bfgdoom1"||sel=="heretic"||sel=="HERETIC")
    {
        ui->levelBox->show();
        ui->levelText->show();
        ui->episodeText->setText("Episode");
    }
    else
    {
        ui->levelBox->hide();
        ui->levelText->hide();
        ui->episodeText->setText("Level");
    }

}


void MainWindow::on_pushButton_clicked()
{
    if(getOsName()=="MacOS" || getOsName()=="Linux")
    {
        std::string homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString();
        system(("open "+homePath+"/.dsda-doom/LogFile.txt >> "+homePath+"/.dsda-doom/LogFile.txt").c_str());
    }
    else
    {
        system("start LogFile.txt >> LogFile.txt");
    }
}



void MainWindow::on_plus_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select WAD file"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("WAD files (*.wad *.deh)"));
    ui->wadsOnFolder->addItems(fileNames);
}


void MainWindow::on_minus_clicked()
{
    ui->wadsOnFolder->takeItem(ui->wadsOnFolder->currentRow());
}




void MainWindow::on_fastCheck_toggled(bool checked)
{
    if(checked)
    {
        isFast=true;
    }
    else
    {
        isFast=false;
    }
}


void MainWindow::on_noCheck_toggled(bool checked)
{
    if(checked)
    {
        noMo=true;
    }
    else
    {
        noMo=false;
    }
}


void MainWindow::on_toolButton_toggled(bool checked)
{
    if(checked)
    {
        ui->toolTip->show();
    }
    else
    {
        ui->toolTip->hide();
    }
}


void MainWindow::on_noCheck_4_toggled(bool checked)
{
    if(checked)
    {
        isRespawn=true;
    }
    else
    {
        isRespawn=false;
    }
}


void MainWindow::on_noCheck_3_toggled(bool checked)
{
    if(checked)
    {
        isFulscreen="f";
    }
    else
    {
        isFulscreen="w";
    }
}


void MainWindow::on_soloNetCheck_toggled(bool checked)
{
    if(checked)
    {
        isSoloNet=true;
    }
    else
    {
        isSoloNet=false;
    }
}


void MainWindow::on_tabs_currentChanged(int index)
{
    if(index==0)
    {
        ui->comboBox->show();
        ui->comboBox_2->show();
    }
    else
    {
        ui->comboBox->hide();
        ui->comboBox_2->hide();
    }

    if(index==2)
    {
        ui->pushButton_2->show();
        ui->pushButton_3->show();
        ui->demoPlayOptions->show();
    }
    else
    {
        ui->pushButton_2->hide();
        ui->pushButton_3->hide();
        ui->demoPlayOptions->hide();
    }

}


void MainWindow::on_pushButton_2_clicked()
{
    QString demoName = QFileDialog::getSaveFileName(this, tr("Demo file"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).toStdString().c_str(),tr("lmp files (*.lmp)"));
    ui->recordDemo->setText(demoName);
}


void MainWindow::on_pushButton_3_clicked()
{
    QString demoName = QFileDialog::getOpenFileName(this, tr("Demo file"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).toStdString().c_str(),tr("lmp files (*.lmp)"));
    ui->recordDemo_2->setText(demoName);
}

