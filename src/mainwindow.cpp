#include "mainwindow.h"

QSettings *settings;
MainWindow *MainWindow::pMainWindow = nullptr;

void MainWindow::changeExeName(QString newName) { exeName = newName; }

void MainWindow::showSSLDialog()
{
    QMessageBox msgBox;
    msgBox.setText("SSL library not found. Network features not available.");
    msgBox.setInformativeText("It should have been packaged with the Launcher.\nIf reinstalling does not fix this, try manually installing OpenSSL.\n\nlibssl-1_1-x64.dll + libcrypto-1_1-x64.dll");
    msgBox.addButton(tr("Ok"), QMessageBox::NoRole);
    msgBox.exec();
}

void MainWindow::changeResolutions(QListWidget *list)
{
    ui->resolution_comboBox->clear();
    ui->resolution_comboBox->addItem(" ");
    for (int i = 0; i < list->count(); i++)
    {
        ui->resolution_comboBox->addItem(list->item(i)->text());
    }
}

void MainWindow::changeButtonColor(bool isDark)
{
#ifdef __APPLE__
    if (isDark)
    {
        ui->showCommandLine_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(120, 120, 120); border-radius:7px; background-color: rgb(50, 50, 50); color: rgb(150, 150, 150)}"
                                                      "QPushButton:pressed{border: 1px solid rgb(120, 120, 120); border-radius:7px; background-color: rgb(75, 75, 75); color: rgb(150, 150, 150)}");
        ui->tooltip_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(120, 120, 120); border-radius:7px; background-color: rgb(50, 50, 50); color: rgb(150, 150, 150)}"
                                              "QPushButton:pressed{border: 1px solid rgb(120, 120, 120); border-radius:7px; background-color: rgb(75, 75, 75); color: rgb(150, 150, 150)}");
        ui->additionalArguments_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(120, 120, 120); border-radius:5px; background-color: rgb(50, 50, 50); color: rgb(150, 150, 150)}"
                                                          "QPushButton:pressed{border: 1px solid rgb(120, 120, 120); border-radius:5px; background-color: rgb(75, 75, 75); color: rgb(150, 150, 150)}");
        ui->nextPage_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(120, 120, 120); border-radius:5px; background-color: rgb(50, 50, 50); color: rgb(150, 150, 150)}"
                                               "QPushButton:pressed{border: 1px solid rgb(120, 120, 120); border-radius:5px; background-color: rgb(75, 75, 75); color: rgb(150, 150, 150)}");
        ui->previousPage_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(120, 120, 120); border-radius:5px; background-color: rgb(50, 50, 50); color: rgb(150, 150, 150)}"
                                                   "QPushButton:pressed{border: 1px solid rgb(120, 120, 120); border-radius:5px; background-color: rgb(75, 75, 75); color: rgb(150, 150, 150)}");
    }
    else
    {
        ui->showCommandLine_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(180, 180, 180); border-radius:7px; background-color: rgb(240,240,240); color: rgb(13,13,13)}"
                                                      "QPushButton:pressed{border: 1px solid rgb(180, 180, 180); border-radius:7px; background-color: rgb(223,223,223); color: rgb(13,13,13)}");
        ui->tooltip_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(180, 180, 180); border-radius:7px; background-color: rgb(240,240,240); color: rgb(13,13,13)}"
                                              "QPushButton:pressed{border: 1px solid rgb(180, 180, 180); border-radius:7px; background-color: rgb(223,223,223); color: rgb(13,13,13)}");
        ui->additionalArguments_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(180, 180, 180); border-radius:5px; background-color: rgb(240,240,240); color: rgb(13,13,13)}"
                                                          "QPushButton:pressed{border: 1px solid rgb(180, 180, 180); border-radius:5px; background-color: rgb(223,223,223); color: rgb(13, 13, 13)}");
        ui->nextPage_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(180, 180, 180); border-radius:5px; background-color: rgb(240,240,240); color: rgb(13,13,13)}"
                                               "QPushButton:pressed{border: 1px solid rgb(180, 180, 180); border-radius:5px; background-color: rgb(223,223,223); color: rgb(13, 13, 13)}");
        ui->previousPage_pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(180, 180, 180); border-radius:5px; background-color: rgb(240,240,240); color: rgb(13,13,13)}"
                                                   "QPushButton:pressed{border: 1px solid rgb(180, 180, 180); border-radius:5px; background-color: rgb(223,223,223); color: rgb(13, 13, 13)}");
    }
#endif
}

// Prevents launching the game twice if the button "Launch" is pressed twice quickly
void MainWindow::delayLaunch() { canLaunch = true; }

// MainWindow
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::pMainWindow = this;

    execPath = QCoreApplication::applicationDirPath();
#ifdef _WIN32
    historyPath = QCoreApplication::applicationDirPath() + "\\history.states";
#endif

    // Allow files to be droped in the launcher (*.wad *.lmp *.deh *.bex)
    setAcceptDrops(true);

    ui->reloadLeaderboard_toolButton->hide();

    // Add event filter to the "additional arguments" box
    ui->additionalArguments_textEdit->installEventFilter(this);

    // Add event filter to the Launch button. This will allow you to see the current parameters when you hover your mouse
    ui->launchGame_pushButton->installEventFilter(this);

    enable_disable_skill_comboBox();

    // set the settings and console windows
    settingsWindow = new Settings;
    consoleWindow = new Console;
    historyListWindow = new historyList;
    endoomWindow = new endoom;

    // The "episode" and "level" boxes can only take 2 numbers
    // This approach also prevents a problem where Qt tried to add spaces to those boxes if no numbers were added
    QRegularExpression rgx("[0-9]{2}");
    QValidator *comValidator = new QRegularExpressionValidator(rgx, this);
    ui->episode_lineEdit->setValidator(comValidator);
    ui->level_lineEdit->setValidator(comValidator);

    ui->stackedWidget->setAttribute(Qt::WA_TranslucentBackground);

#ifdef _WIN32
    ui->tooltip_textBrowser->setHtml(
        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'.AppleSystemUIFont'; font-size:8pt; font-weight:400; font-style:normal;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" "
        "font-size:8pt;\">Don't see any IWAD?     ^</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Go to the settings and add the folder you have your IWADs in, to the </span><span style=\" font-size:9pt; "
        "font-weight:700;\">IWAD Folders</span><span style=\" font-size:8pt;\"> list.</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Then restart the Launcher</span></p></body></html>");
#endif

    // Keyboard shortcut
    // Qt::CTRL is the CTRL key for Windows/Linux and is the CMD key for MacOS

    // Closes the active window
    QShortcut *shortcut3 = new QShortcut(QKeySequence(Qt::Key_W | Qt::CTRL), this, SLOT(foo3()));
    shortcut3->setAutoRepeat(false);

    // Set the parameters text correctly
    if (!settings->value("toggle1a").isNull())
    {
        changeToggles(settings->value("toggle1t").toString(), settings->value("toggle1a").toString(), settings->value("toggle2t").toString(), settings->value("toggle2a").toString(), settings->value("toggle3t").toString(), settings->value("toggle3a").toString(), settings->value("toggle4t").toString(), settings->value("toggle4a").toString(), )
    }
    else
    {
        changeToggles(DEFAULT_TOGGLE1ARG, DEFAULT_TOGGLE1TEXT, DEFAULT_TOGGLE2ARG, DEFAULT_TOGGLE2TEXT, DEFAULT_TOGGLE3ARG, DEFAULT_TOGGLE3TEXT, DEFAULT_TOGGLE4ARG, DEFAULT_TOGGLE4TEXT);
    }

    findIwads();

    // If no IWAD found, show a tool tip
    if (ui->iwad_comboBox->count() == 0)
    {
        ui->tooltip_textBrowser->show();
    }
    else
    {
        ui->tooltip_textBrowser->hide();
    }

    // Load settings and apply them
    if (settings->value("remember").toBool())
    {
        ui->complevel_comboBox->setCurrentIndex(settings->value("complevel").toInt());
        ui->difficulty_comboBox->setCurrentIndex(settings->value("skill").toInt());
        ui->episode_lineEdit->setText(settings->value("warp1").toString());
        ui->level_lineEdit->setText(settings->value("warp2").toString());
        int pwadCount = settings->value("pwadCount").toInt();
        for (int i = 0; i < pwadCount; i++)
        {
            QString filePath = settings->value("pwad" + QString::number(i)).toString();

            ui->wads_listWidget->addItem(getFileName(filePath));
            ui->wads_listWidget->item(ui->wads_listWidget->count() - 1)->setToolTip(filePath);
        }
        ui->toggle1_checkBox->setChecked(settings->value("fast").toBool());
        ui->toggle2_checkBox->setChecked(settings->value("nomo").toBool());
        ui->toggle3_checkBox->setChecked(settings->value("respawn").toBool());
        ui->fullscreen_checkBox->setChecked(settings->value("fullscreen").toBool());
        ui->resolution_comboBox->setCurrentIndex(settings->value("geom").toInt());
        if (ui->iwad_comboBox->count() >= settings->value("iwad").toInt())
        {
            ui->iwad_comboBox->setCurrentIndex(settings->value("iwad").toInt());
        }
        ui->toggle4_checkBox->setChecked(settings->value("solonet").toBool());
        ui->additionalArguments_textEdit->append(settings->value("argumentText").toString());

        ui->record_lineEdit->setText(settings->value("recorddemo").toString());
        ui->playback_lineEdit->setText(settings->value("playdemo").toString());
        ui->viddump_lineEdit->setText(settings->value("viddump").toString());
        ui->hud_lineEdit->setText(settings->value("hud").toString());
        ui->config_lineEdit->setText(settings->value("config").toString());

        ui->playback_comboBox->setCurrentIndex(settings->value("demoplaybox").toInt());
    }

    if (ui->playback_comboBox->currentIndex() != 1)
    {
        ui->viddump_lineEdit->setHidden(true);
        ui->viddump_pushButton->setHidden(true);
    }

    if (ui->viddump_lineEdit->text() == "") ui->viddump_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(150, 150, 150); background-color: rgb(255, 255, 255); border-radius:3px");
    else ui->viddump_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(0, 0, 0); background-color: rgb(255, 255, 255); border-radius:3px");

    if (ui->hud_lineEdit->text() == "") ui->hud_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(150, 150, 150); background-color: rgb(255, 255, 255); border-radius:3px");
    else ui->hud_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(0, 0, 0); background-color: rgb(255, 255, 255); border-radius:3px");

    if (ui->config_lineEdit->text() == "") ui->config_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(150, 150, 150); background-color: rgb(255, 255, 255); border-radius:3px");
    else ui->config_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(0, 0, 0); background-color: rgb(255, 255, 255); border-radius:3px");

    if (settings->value("maxskilllevel").toString() != "") setMaxSkillLevel(settings->value("maxskilllevel").toInt());

    if (ui->iwad_comboBox->currentIndex() == -1 && ui->iwad_comboBox->count() != 0) ui->iwad_comboBox->setCurrentIndex(0);

    if (ui->episode_lineEdit->text() == "")
    {
        ui->difficulty_comboBox->setEnabled(false);
        ui->difficulty_label->setEnabled(false);
    }

    QStringList arguments = qApp->arguments();
    if (arguments.count() > 1)
    {
        for (int i = 1; i < arguments.count(); i++)
        {
            QString absPath = arguments.at(i);
            dropFile(absPath);
        }
    }
}

// Drag Event for *.wad *.lmp *.state *.deh *.bex
void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

QString MainWindow::getFileName(QString filePath)
{
#ifdef _WIN32
    return filePath.section(folder_separator, -1, -1);
#else
    return filePath.section(folder_separator, -1, -1);
#endif
}

QString MainWindow::getFilePath(QString fileName)
{
    // Find file in dsda folder
    QString dsda_folder;
#ifdef _WIN32
    dsda_folder = execPath;
#else
    dsda_folder = dotfolder;
#endif
    QDir dsda_folder_path(dsda_folder);
    QStringList dsda_folder_files_list = dsda_folder_path.entryList(QDir::Files);

    foreach (QString file_in_dsda_folder, dsda_folder_files_list)
    {
        if (fileName == file_in_dsda_folder.toLower())
        {
            return (dsda_folder + "/" + file_in_dsda_folder);
        }
    }

    // Find file in DOOMWADPATH
    QString doomwadpath = QString(qgetenv("DOOMWADPATH"));
#ifdef _WIN32
    QChar token = ';';
#else
    QChar token = ':';
#endif

    QStringList doomwadpath_folders_list = doomwadpath.split(token);
    foreach (QString doomwadpath_folder, doomwadpath_folders_list)
    {
        QStringList doomwadpath_folder_files_list = QDir(doomwadpath_folder).entryList(QDir::Files);
        foreach (QString file_in_doomwadpath_folder, doomwadpath_folder_files_list)
        {
            if (fileName == file_in_doomwadpath_folder.toLower())
            {
                return (doomwadpath_folder + "/" + file_in_doomwadpath_folder);
            }
        }
    }

    // Find file in the pwadfolders
    int size = settings->beginReadArray("pwadfolders");
    for (int j = 0; j < size; j++)
    {
        settings->setArrayIndex(j);
        QString folder = settings->value("folder").toString();
        if (folder != "")
        {
            QDir folder_path(folder);
            QStringList folder_files_list = folder_path.entryList(QDir::Files);
            foreach (QString file_in_folder, folder_files_list)
            {
                if (fileName == file_in_folder.toLower())
                {
                    settings->endArray();
                    return (folder + "/" + file_in_folder);
                }
            }
        }
    }
    settings->endArray();

    return "";
}

void MainWindow::dropLmp(QString filePath)
{
    ui->tabs->setCurrentIndex(2);
    ui->playback_lineEdit->setText(filePath);

    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }

    QTextStream stream(&file);
    QString buffer;

    bool found_footer = false;
    while (stream.readLineInto(&buffer))
    {
        if (buffer.mid(0, 5) == "-iwad")
        {
            found_footer = true;
            ui->wads_listWidget->clear();

            QStringList argList;
            QString tmp;
            for (qsizetype i = 0; i < buffer.size(); i++)
            {
                if (buffer[i] != ' ' && buffer[i] != '\"')
                {
                    tmp += buffer[i];
                }
                else if (!tmp.isEmpty())
                {
                    argList.push_back(tmp);
                    tmp.clear();
                }
            }
            if (!tmp.isEmpty())
            {
                argList.push_back(tmp);
                tmp.clear();
            }

            for (int i = 0; i < argList.count() - 1; i++)
            {
                if (argList[i] == "-iwad")
                {
                    int iwad_dot_pos = argList[i + 1].lastIndexOf('.');
                    iwad_dot_pos = iwad_dot_pos == -1 ? argList[i + 1].size() : iwad_dot_pos;

                    int iwad_index = ui->iwad_comboBox->findText(argList[i + 1].left(iwad_dot_pos));
                    if (iwad_index != -1)
                    {
                        ui->iwad_comboBox->setCurrentIndex(iwad_index);
                    }
                }
                else if (argList[i] == "-file" || argList[i] == "-deh")
                {
                    QStringList files;
                    for (int ii = i + 1; ii < argList.count(); ii++)
                    {
                        if (argList[ii].size() < 2 || argList[ii][0] == '-')
                        {
                            break;
                        }

                        QString tmp = argList[ii].toLower();

                        // Some old Woof demos don't have the .wad extension on the footer
                        int file_dot_pos = tmp.lastIndexOf('.');
                        if (file_dot_pos == -1) tmp += ".wad";

                        files.append(tmp);
                    }

                    for (int i = 0; i < files.count(); i++)
                    {
                        QString filePath = getFilePath(files[i]);
                        if (filePath.isEmpty()) continue;

                        ui->wads_listWidget->addItem(getFileName(filePath));
                        ui->wads_listWidget->item(ui->wads_listWidget->count() - 1)->setToolTip(filePath);
                    }
                }
            }
        }
    }

    file.close();

    if (!found_footer)
    {
        QStringList iwad_list;
        for (int i = 0; i < ui->iwad_comboBox->count(); i++)
        {
            iwad_list.push_back(ui->iwad_comboBox->itemText(i));
        }
        demodialog *demoDialogNew = new demodialog(iwad_list, this);
        demoDialogNew->open();

        demoDialog = demoDialogNew;
        connect(demoDialog, SIGNAL(accepted()), this, SLOT(demoDialog_accepted()));
    }
}

void MainWindow::dropFile(QString fileName)
{
    int dot_pos = fileName.lastIndexOf('.');
    if (dot_pos == -1) return;

    QString tmp = fileName.mid(dot_pos + 1).toLower();

    if (tmp == "lmp")
    {
        dropLmp(fileName);
    }
    else if (tmp == "wad" || tmp == "bex" || tmp == "deh" || tmp == "zip")
    {
        QStringList wadsToAdd;
        wadsToAdd.append(fileName);
        addWads(wadsToAdd);
        ui->tabs->setCurrentIndex(1);
    }
    else if (tmp == "state")
    {
        states::loadStateFromFile(fileName);
    }
}

void MainWindow::demoDialog_accepted()
{
    ui->iwad_comboBox->setCurrentIndex(demoDialog->get_iwad_index());
    ui->wads_listWidget->clear();

    QStringList files_list = demoDialog->get_files_list();
    foreach (QString filePath, files_list)
    {
        ui->wads_listWidget->addItem(getFileName(filePath));
        ui->wads_listWidget->item(ui->wads_listWidget->count() - 1)->setToolTip(filePath);
    }
}

// Drop Event for *.wad *.lmp *gfd
void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
        dropFile(fileName);
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Load State"), settings->value("statefile").toString(), tr("state files (*.state)"));
    if (fileNames.length() > 0)
    {
        settings->setValue("statefile", fileNames[0]);
        states::loadStateFromFile(fileNames[0]);
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save State"), settings->value("statefile").toString(), tr("state files (*.state)"));
    if (fileName != "")
    {
        settings->setValue("statefile", fileName);
        states::saveStateToFile(fileName);
    }
}

void MainWindow::on_actionGithub_triggered() { QDesktopServices::openUrl(QUrl(dsdalauncher_url)); }

void MainWindow::on_actionGithub_2_triggered() { QDesktopServices::openUrl(QUrl(dsdadoom_url)); }

void MainWindow::on_actionCheck_for_updates_triggered()
{
    if (!QSslSocket::supportsSsl())
    {
        showSSLDialog();
        return;
    }

    QString launcherV;

    QNetworkRequest req(QUrl("https://api.github.com/repos/Pedro-Beirao/dsda-launcher/releases/latest"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.get(req);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument jsondoc = QJsonDocument::fromJson(response_data);
    QJsonObject jsonobj = jsondoc.object();
    foreach (const QString &key, jsonobj.keys())
    {
        QJsonValue value = jsonobj.value(key);
        if (key == "name")
        {
            if (version != value.toString())
            {
                QMessageBox msgBox;
                msgBox.setText("DSDA-Launcher " + version);
                msgBox.setInformativeText("Available: " + value.toString());
                QPushButton *pButtonYes = msgBox.addButton(tr("Update"), QMessageBox::YesRole);
                msgBox.addButton(tr("Ignore"), QMessageBox::NoRole);
                msgBox.setDefaultButton(pButtonYes);
                msgBox.exec();
                if (msgBox.clickedButton() == pButtonYes)
                {
                    QDesktopServices::openUrl(QUrl("https://github.com/Pedro-Beirao/dsda-launcher/releases" + launcherV));
                }
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText("DSDA-Launcher " + version);
                msgBox.setInformativeText("Up to Date");
                msgBox.addButton(tr("Ignore"), QMessageBox::NoRole);
                msgBox.exec();
            }
        }
    }

    reply->deleteLater();
}

void MainWindow::on_actionCheck_for_Updates_triggered()
{
    if (!QSslSocket::supportsSsl())
    {
        showSSLDialog();
        return;
    }

    QString portversion;

    QString path;

#ifdef __APPLE__
    path = execPath + "/../Resources/" + exeName;
#elif __linux__
    path = execPath + "/" + exeName;
#else
    path = execPath + "/" + exeName + ".exe";
#endif
    QFile port = QFile(path);
    if (port.exists())
    {
        QProcess *process = new QProcess;
        process->setWorkingDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        process->start(path, {"-v"});
        process->waitForStarted();
        QString data;

        while (process->waitForReadyRead())
            data.append(process->readAll());

        QTextStream stream(&data);
        stream >> data >> data;
        portversion = data;
    }

    QNetworkRequest req0(QUrl("https://api.github.com/repos/kraflab/dsda-doom/releases/latest"));
    req0.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json0;
    QNetworkAccessManager nam0;
    QNetworkReply *reply0 = nam0.get(req0);
    while (!reply0->isFinished())
    {
        qApp->processEvents();
    }
    QByteArray response_data = reply0->readAll();
    QJsonDocument jsondoc = QJsonDocument::fromJson(response_data);
    QJsonObject jsonobj = jsondoc.object();
    foreach (const QString &key, jsonobj.keys())
    {
        QJsonValue value = jsonobj.value(key);
        if (key == "name")
        {
            if (portversion != value.toString())
            {
                QMessageBox msgBox;
                msgBox.setText("DSDA-Doom " + portversion);
                msgBox.setInformativeText("Available: " + value.toString());
                QPushButton *pButtonYes;
                pButtonYes = msgBox.addButton(tr("Update"), QMessageBox::YesRole);
                msgBox.addButton(tr("Ignore"), QMessageBox::NoRole);
                msgBox.setDefaultButton(pButtonYes);
                msgBox.exec();
                if (msgBox.clickedButton() == pButtonYes)
                {
                    QDesktopServices::openUrl(QUrl("https://www.doomworld.com/forum/topic/118074-dsda-doom-source-port-v0225/"));
                }
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText("DSDA-Doom " + portversion);
                msgBox.setInformativeText("Up to Date");
                msgBox.addButton(tr("Ignore"), QMessageBox::NoRole);
                msgBox.exec();
            }
        }
    }

    reply0->deleteLater();
}

void MainWindow::on_actionSet_triggered()
{
    settingsWindow->show();
    settingsWindow->activateWindow();
    settingsWindow->raise();
}

void MainWindow::on_actionTips_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("A few tips you should know:");
    msgBox.setInformativeText("- Drag .wad / .deh files on top of the launcher to add them to the loaded files\n\n- Drag .lmp files on top of the launcher to play the demo and autoselect the correct IWAD, PWADs and complevel\n\n- For the mentioned autoselect to work correctly, go to the settings and set the PWADs folders");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}
void MainWindow::on_actionWhat_is_this_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("State files");
    msgBox.setInformativeText("These .state files save all the parameters selected on the launcher, and lets you load them again whenever you need");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_actionCommand_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export command line"), settings->value("batfile").toString(), tr("batch files (*.bat *.sh *.zsh *.command)"));
    if (fileName != "")
    {
        settings->setValue("batfile", fileName);
        on_launchGame_pushButton_clicked(false, true, fileName);
    }
}

void MainWindow::on_actionOpen_IWAD_folder_triggered() { foo(); }

void MainWindow::on_actionOpen_Console_triggered() { on_console_pushButton_clicked(); }

void MainWindow::on_actionHistory_triggered()
{
    if (historyListWindow->isHidden())
    {
        historyListWindow->getHistory();
    }
    historyListWindow->show();
    historyListWindow->activateWindow();
    historyListWindow->raise();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::foo() // CTRL+O runs this function to open the folder where the IWADs should be placed in
{
#ifdef __APPLE__
    QProcess::startDetached("open", {dotfolder});
#elif __linux__
    QProcess::startDetached("xdg-open", {dotfolder});
#else
    QProcess::startDetached("explorer.exe", {execPath});
#endif
}

void MainWindow::foo3() // CTRL+W runs this function close the active window
{
    QWidget *currentWindow = QApplication::activeWindow();
    currentWindow->close();
}

void MainWindow::addWads(QStringList files_list) // Click the + button to add a wad
{
    foreach (QString filePath, files_list)
    {
        ui->wads_listWidget->addItem(getFileName(filePath));
        ui->wads_listWidget->item(ui->wads_listWidget->count() - 1)->setToolTip(filePath);
    }
}

void MainWindow::addToArguments(QString string)
{
    if (ui->additionalArguments_textEdit->toPlainText().length() > 1 && ui->additionalArguments_textEdit->toPlainText().back() != ' ') ui->additionalArguments_textEdit->setText(ui->additionalArguments_textEdit->toPlainText() + " " + string);
    else ui->additionalArguments_textEdit->setText(ui->additionalArguments_textEdit->toPlainText() + string);
}

bool isFast = false;
bool isNoMo = false;
bool isRespawn = false;
bool isSoloNet = false;
QString isFullscreen = "w";

void MainWindow::error(QProcess::ProcessError error) { qDebug() << "Error: " << error; }

void MainWindow::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QProcess *p = (QProcess *)sender();
    delete p;
    running = false;

    if (exitCode != 0)
    {
        consoleWindow->show();
        consoleWindow->activateWindow();
        consoleWindow->raise();
        return;
    }

    if (settings->value("endoom").toBool())
    {
        endoomWindow->showEndoom(endoomString);

        endoomWindow->show();
        endoomWindow->activateWindow();
        endoomWindow->raise();
    }
}

void MainWindow::readyReadStandardError()
{

    QProcess *p = (QProcess *)sender();
    QByteArray buf = p->readAllStandardError();

    consoleWindow->changeText("<div style='color: red;'>" + buf + "</div>");
}

void MainWindow::readyReadStandardOutput()
{
    QProcess *p = (QProcess *)sender();
    QByteArray buf = p->readAllStandardOutput();

    QString qs = buf;

    if (qs.indexOf("\033") != -1 || endoomString != "")
    {
        endoomString += qs;
    }
    else
    {
        consoleWindow->changeText(buf);
    }
}

void MainWindow::started() { running = true; }

void MainWindow::gameIsRunning()
{
    QMessageBox msgBox;
    msgBox.setText(exeName + " is still running.");
    msgBox.addButton("Ok", QMessageBox::YesRole);
    msgBox.exec();
}

void MainWindow::on_launchGame_pushButton_clicked(bool onExit, bool returnTooltip, QString exportCmd) // Runs when you click the launch button or when you close the launcher (When closing, it will not run the game, but actually just save the settings)
{
    if (!canLaunch) // Dont allow 2 launchs in the time of 2 sec
        return;

    if (running && !returnTooltip)
    {
        gameIsRunning();
        return;
    }

    int complevelIndex = ui->complevel_comboBox->currentIndex();
    int diffIndex = ui->difficulty_comboBox->currentIndex();

    if (onExit)
    {
        settings->setValue("iwad", ui->iwad_comboBox->currentIndex());

        // Again, we need to remove the setting if the additional parameters box is empty so that it does not appear when we open the launcher again
        if (ui->additionalArguments_textEdit->toPlainText() != "")
        {
            settings->setValue("argumentText", ui->additionalArguments_textEdit->toPlainText());
        }
        else
        {
            settings->remove("argumentText");
        }
        settings->setValue("fullscreen", ui->fullscreen_checkBox->isChecked());
        settings->setValue("geom", ui->resolution_comboBox->currentIndex());

        settings->setValue("solonet", isSoloNet);
        settings->setValue("respawn", isRespawn);
        settings->setValue("nomo", isNoMo);
        settings->setValue("fast", isFast);

        settings->setValue("complevel", complevelIndex);
        settings->setValue("skill", diffIndex);

        settings->setValue("warp1", ui->episode_lineEdit->text());
        settings->setValue("warp2", ui->level_lineEdit->text());

        // We need to remove the setting if the warp number is deleted so that it does not appear when we open the launcher again
        // gzdoom does not do this for the arguments box (at the time of writing, at least) and it drives me nuts
        if (ui->episode_lineEdit->text() == "")
        {
            settings->remove("warp1");
        }
        if (ui->level_lineEdit->text() == "")
        {
            settings->remove("warp2");
        }

        if (ui->record_lineEdit->text() != "")
        {
            settings->setValue("recorddemo", ui->record_lineEdit->text());
        }
        else
        {
            settings->remove("recorddemo");
        }

        if (ui->playback_lineEdit->text() != "")
        {
            settings->setValue("playdemo", ui->playback_lineEdit->text());
        }
        else
        {
            settings->remove("playdemo");
        }

        if (ui->viddump_lineEdit->text() != "")
        {
            settings->setValue("viddump", ui->viddump_lineEdit->text());
        }
        else
        {
            settings->remove("viddump");
        }

        if (ui->hud_lineEdit->text() != "")
        {
            settings->setValue("hud", ui->hud_lineEdit->text());
        }
        else
        {
            settings->remove("hud");
        }

        if (ui->config_lineEdit->text() != "")
        {
            settings->setValue("config", ui->config_lineEdit->text());
        }
        else
        {
            settings->remove("config");
        }

        settings->setValue("demoplaybox", ui->playback_comboBox->currentIndex());

        settings->setValue("pwadCount", ui->wads_listWidget->count());
        for (int i = 0; i < ui->wads_listWidget->count(); i++)
        {
            settings->setValue("pwad" + QString::number(i), ui->wads_listWidget->item(i)->toolTip());
        }

        settings->setValue("exeName", exeName);

        settings->setValue("version", version);

        settings->sync();

        return;
    }

    QStringList argList;

    /* Complevels:
        Default
        1 - Doom v1.666
        2 - Doom v1.9
        3 - Ultimate Doom & Doom95
        4 - Final Doom
        5 - DOSDoom
        6 - TASDOOM
        7 - Boom's inaccurate vanilla compatibility mode
        8 - Boom v2.01
        9 - Boom v2.02
        10 - LxDoom
        11 - MBF
     12—16 - PrBoom (old versions)
        17 - Current PrBoom
        21 - MBF 21

        If the complevel starts with 'D', then dont use the "-complevel" parameter
        Otherwise, run "-complevel *First+Second char of the string*"
    */
    QString complevelString = ui->complevel_comboBox->currentText();
    if (ui->complevel_comboBox->isEnabled() && complevelString[0] != 'D')
    {
        argList.append("-complevel");
        QString complevelNum;
        complevelNum.push_back(complevelString[0]);
        complevelNum.push_back(complevelString[1]);
        argList.append(complevelNum);
    }

    // Difficulty or Skill
    if (ui->difficulty_comboBox->isEnabled() && ui->difficulty_comboBox->currentIndex() != 0)
    {
        argList.append("-skill");
        argList.append(QString::number(diffIndex));
    }

    // Warping in Doom takes 2 boxes. 1 for the episode, 1 for the mission
    // Warping in Doom 2 takes 1 box, for the map
    if (ui->level_lineEdit->text() != "" && !ui->level_lineEdit->isHidden())
    {
        argList.append("-warp");
        argList.append(ui->episode_lineEdit->text());
        argList.append(ui->level_lineEdit->text());
    }
    else if (ui->episode_lineEdit->text() != "" && ui->level_lineEdit->isHidden())
    {
        argList.append("-warp");
        argList.append(ui->episode_lineEdit->text());
    }

    /* You can load 3 types of files.
     *.wad -file
     *.deh -deh
     *.bex -deh
     */
    if (ui->wads_listWidget->count() > 0)
    {
        argList.append("-file");
        for (int item = 0; item < ui->wads_listWidget->count(); item++)
        {
            QString fileToAdd = ui->wads_listWidget->item(item)->toolTip();
#ifdef _WIN32
            for (int i = 0; i < fileToAdd.length(); i++)
            {
                if (fileToAdd[i] == '/') fileToAdd[i] = '\\';
            }
#endif
            if (fileToAdd.mid(0, 5) == "$DOOM") fileToAdd = fileToAdd.mid(13);
            else if (fileToAdd.mid(0, 5) == "%DOOM") fileToAdd = fileToAdd.mid(14);

            if (returnTooltip) argList.append("\"" + fileToAdd + "\"");
            else argList.append(fileToAdd);
        }
    }

    // Again, these are the parameters available on toggles
    if (isFast)
    {
        QString tmp = "";
        for (int i = 0; i < fastParam.length(); i++)
        {
            if (fastParam.at(i) == ';')
            {
                argList.append(tmp);
                tmp = "";
                continue;
            }
            tmp += fastParam.at(i);
        }
        argList.append(tmp);
    }
    if (isNoMo)
    {
        QString tmp = "";
        for (int i = 0; i < nomoParam.length(); i++)
        {
            if (nomoParam.at(i) == ';')
            {
                argList.append(tmp);
                tmp = "";
                continue;
            }
            tmp += nomoParam.at(i);
        }
        argList.append(tmp);
    }
    if (isRespawn)
    {
        QString tmp = "";
        for (int i = 0; i < respawnParam.length(); i++)
        {
            if (respawnParam.at(i) == ';')
            {
                argList.append(tmp);
                tmp = "";
                continue;
            }
            tmp += respawnParam.at(i);
        }
        argList.append(tmp);
    }
    if (isSoloNet)
    {
        QString tmp = "";
        for (int i = 0; i < solonetParam.length(); i++)
        {
            if (solonetParam.at(i) == ';')
            {
                argList.append(tmp);
                tmp = "";
                continue;
            }
            tmp += solonetParam.at(i);
        }
        argList.append(tmp);
    }

    if (ui->resolution_comboBox->currentIndex() == 0)
    {
        if (isFullscreen == "w")
        {
            argList.append("-window");
        }
        else
        {
            argList.append("-fullscreen");
        }
    }
    else
    {
        argList.append("-geom");
        argList.append(ui->resolution_comboBox->currentText() + isFullscreen);
    }

    if (ui->record_lineEdit->text() != "")
    {
        argList.append("-record");
        if (returnTooltip) argList.append("\"" + ui->record_lineEdit->text() + "\"");
        else argList.append(ui->record_lineEdit->text());
    }

    if (ui->playback_lineEdit->text() != "")
    {
        if (ui->playback_comboBox->currentIndex() == 0)
        {
            argList.append("-playdemo"); // Plays demo at normal speed
            if (returnTooltip) argList.append("\"" + ui->playback_lineEdit->text() + "\"");
            else argList.append(ui->playback_lineEdit->text());
        }
        else if (ui->playback_comboBox->currentIndex() == 1)
        {
            argList.append("-timedemo"); // Used for viddumping
            if (returnTooltip) argList.append("\"" + ui->playback_lineEdit->text() + "\"");
            else argList.append(ui->playback_lineEdit->text());
            if (ui->viddump_lineEdit->text().length() > 2)
            {
                argList.append("-viddump");
                if (returnTooltip) argList.append("\"" + ui->viddump_lineEdit->text() + "\"");
                else argList.append(ui->viddump_lineEdit->text());
            }
        }
        else if (ui->playback_comboBox->currentIndex() == 2)
        {
            argList.append("-fastdemo"); // Used for benchmarks
            if (returnTooltip) argList.append("\"" + ui->playback_lineEdit->text() + "\"");
            else argList.append(ui->playback_lineEdit->text());
        }
    }

    if (ui->hud_lineEdit->text() != "")
    {
        argList.append("-hud");
        if (returnTooltip) argList.append("\"" + ui->hud_lineEdit->text() + "\"");
        else argList.append(ui->hud_lineEdit->text());
    }

    if (ui->config_lineEdit->text() != "")
    {
        argList.append("-config");
        if (returnTooltip) argList.append("\"" + ui->config_lineEdit->text() + "\"");
        else argList.append(ui->config_lineEdit->text());
    }

    if (ui->track_comboBox->currentIndex() == 1) argList.append("-track_pacifist");
    else if (ui->track_comboBox->currentIndex() == 2) argList.append("-track_100k");

    if (ui->time_comboBox->currentIndex() == 1) argList.append("-time_use");
    else if (ui->time_comboBox->currentIndex() == 2) argList.append("-time_keys");
    else if (ui->time_comboBox->currentIndex() == 3) argList.append("-time_secrets");
    else if (ui->time_comboBox->currentIndex() == 4) argList.append("-time_all");

    if (ui->additionalArguments_textEdit->toPlainText() != "")
    {
        QString str = ui->additionalArguments_textEdit->toPlainText() + " ";

        QString strToAdd = "";
        for (qsizetype i = 0; i < str.length(); i++)
        {

            QChar c = str[i];
            if (c == ' ')
            {
                if (strToAdd != "")
                {
                    argList.append(strToAdd);
                    strToAdd = "";
                }
            }
            else if (c == '\"')
            {
                i++;
                while (str[i] != '\"')
                {
                    strToAdd.push_back(str[i]);
                    i++;
                }
            }
            else
            {
                strToAdd.push_back(c);
            }
        }
    }

    if (returnTooltip)
    {
        QString argStr;
        QString argStrComplete;
        foreach (QString p, argList)
        {
            argStrComplete.append((p + " "));

            int lastBar = 0;
            for (qsizetype i = 0; i < p.length(); i++)
            {
                if (p[i] == QDir::separator())
                {
                    lastBar = i + 1;
                }
            }
            p = p.mid(lastBar);

            if (p.length() > 1 && p[0] != '"' && p[p.length() - 1] == '"')
            {
                p = p.mid(0, p.length() - 1);
            }

            argStr.append((p + " "));
        }

        if (exportCmd != "")
        {
            QFile file(exportCmd);
            if (!file.open(QFile::WriteOnly | QFile::Text))
            {
                return;
            }
            QTextStream out(&file);

#ifdef __APPLE__
            out << "\"" + execPath + "/../Resources/" + exeName + "\" -iwad \"" + ui->iwad_comboBox->itemData(ui->iwad_comboBox->currentIndex(), Qt::ToolTipRole).toString() + "\" " + argStrComplete;
#elif __linux__
            out << "\"" + execPath + "/" + exeName + "\" -iwad \"" + ui->iwad_comboBox->itemData(ui->iwad_comboBox->currentIndex(), Qt::ToolTipRole).toString() + "\" " + argStrComplete;
#else
            std::replace(execPath.begin(), execPath.end(), '/', '\\');
            out << "\"" + execPath + "\\" + exeName + ".exe\" -iwad \"" + ui->iwad_comboBox->itemData(ui->iwad_comboBox->currentIndex(), Qt::ToolTipRole).toString() + "\" " + argStrComplete;
#endif
            file.close();

            return;
        }

        QMessageBox msgBox;
        msgBox.setText("Executable: " + exeName + "\nIWAD: " + ui->iwad_comboBox->currentText() + "\nParameters: " + argStr);
        msgBox.addButton(tr("Copy"), QMessageBox::NoRole);
        QPushButton *pButtonYes = msgBox.addButton(tr("Ok"), QMessageBox::YesRole);
        msgBox.setDefaultButton(pButtonYes);
        msgBox.exec();

        if (msgBox.clickedButton() != pButtonYes)
        {
            QClipboard *clip = QApplication::clipboard();
#ifdef __APPLE__
            clip->setText("\"" + execPath + "/../Resources/" + exeName + "\" -iwad \"" + ui->iwad_comboBox->itemData(ui->iwad_comboBox->currentIndex(), Qt::ToolTipRole).toString() + "\" " + argStrComplete);
#elif __linux__
            clip->setText("\"" + execPath + "/" + exeName + "\" -iwad \"" + ui->iwad_comboBox->itemData(ui->iwad_comboBox->currentIndex(), Qt::ToolTipRole).toString() + "\" " + argStrComplete);
#else
            std::replace(execPath.begin(), execPath.end(), '/', '\\');
            clip->setText("\"" + execPath + "\\" + exeName + ".exe\" -iwad \"" + ui->iwad_comboBox->itemData(ui->iwad_comboBox->currentIndex(), Qt::ToolTipRole).toString() + "\" " + argStrComplete);
#endif
        }

        return;
    }

    Launch(ui->iwad_comboBox->currentText(), argList);

    SaveHistory(ui->iwad_comboBox->currentText(), argList);
}

void MainWindow::Launch(QString iwadName, QStringList argList)
{
    if (!canLaunch) return;

    for (int i = 0; i < argList.count(); i++)
    {
        if (argList.at(i) == "")
        {
            argList.removeAt(i);
        }
    }

    consoleWindow->clearText();
    endoomWindow->clearText();
    endoomString = "";

    if (settings->value("endoom").toBool())
    {
        argList.append({"-assign", "ansi_endoom=2"});
    }

#ifdef __APPLE__
    QFile port = QFile(execPath + "/../Resources/" + exeName + "");
    if (port.exists())
    {
        QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        argList.push_front(ui->iwad_comboBox->itemData(ui->iwad_comboBox->currentIndex(), Qt::ToolTipRole).toString());
        argList.push_front("-iwad");
        QProcess *process = new QProcess;
        process->setWorkingDirectory(homePath);
        process->start(execPath + "/../Resources/" + exeName, argList);
        connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
        connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
        connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
        connect(process, SIGNAL(started()), this, SLOT(started()));
    }
    else
    {
        QMessageBox::warning(this, "dsda-launcher", exeName + " was not found in dsda-launcher.app/Contents/Resources/" + exeName);
    }
#elif __linux__
    QFile port = QFile(execPath + "/" + exeName);
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    argList.push_front(ui->iwad_comboBox->itemData(ui->iwad_comboBox->currentIndex(), Qt::ToolTipRole).toString());
    // Run "which" command to check if dsda-doom exists. if it does then no need to specify a path, just run a process with exeName.
    QStringList apar;
    apar << exeName;
    QProcess whichProcess;
    whichProcess.start("which", apar);
    whichProcess.waitForFinished();
    QString processPath;
    // If it finds an executable in the dsda-launcher folder, it will prioritize it over the one installed in a bin folder.
    if (port.exists()) processPath = execPath + "/" + exeName;
    else processPath = exeName;
    if (whichProcess.readAllStandardOutput() != "")
    {
        QProcess *process = new QProcess;
        process->setWorkingDirectory(homePath);
        process->start(processPath, argList);
        connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
        connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
        connect(process, SIGNAL(started()), this, SLOT(started()));
    }
    else QMessageBox::warning(this, "dsda-launcher", ("Failed to launch the application executable.\nMake sure that " + exeName + " is installed correctly through your package manager or installed with the original build instructions.\n\nIf you are sure " + exeName + " exists, symlink it to dsda-launcher's folder."));
#else
    QFile port = QFile(execPath + "/" + exeName + ".exe");
    if (port.exists())
    {
        argList.push_front(ui->iwad_comboBox->itemData(ui->iwad_comboBox->currentIndex(), Qt::ToolTipRole).toString());
        argList.push_front("-iwad");
        QProcess *process = new QProcess;
        process->setWorkingDirectory(execPath);
        process->start(execPath + "/" + exeName + ".exe", argList);
        connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
        connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
        connect(process, SIGNAL(started()), this, SLOT(started()));
    }
    else
    {
        QMessageBox::warning(this, "dsda-launcher", "Failed to launch the application executable.\nMake sure that the launcher is in the same folder as " + exeName + ".exe");
    }
#endif

    // Again, don't allow the launch button to work twice in the space of 2 secs
    canLaunch = false;
    QTimer::singleShot(1000, this, SLOT(delayLaunch()));
}

void MainWindow::SaveHistory(QString iwad, QStringList args)
{
    int checksum = 0;
    int count = 0;
    int needToDelete = 0;

    QString t = iwad;
    for (qsizetype i = 0; i < args.size(); i++)
    {
        t += args.at(i);
    }
    QByteArray arr = t.toLatin1();
    checksum = qChecksum(arr.data(), arr.length());

    QFile file(historyListWindow->historyPath);

    if (!file.isOpen())
    {
        return;
    }

    QTextStream stream(&file);

    QString buffer;

    while (!file.atEnd())
    {
        stream.readLineInto(&buffer);
        if (buffer[0] == '-')
        {
            count++;
        }
    }
    stream.seek(0);

    while (buffer.mid(0, 9) != "checksum=" && !file.atEnd())
    {
        stream.readLineInto(&buffer);
    }

    if (buffer.mid(0, 9) == "checksum=" && buffer.mid(9).length() > 0)
    {
        if (checksum == buffer.mid(9).toInt())
        {
            file.close();
            return;
        }
    }

    file.close();

    int maxhistory = settings->value("maxhistory").toInt();
    if (count >= maxhistory)
    {
        needToDelete = count - maxhistory + 1;
    }

    QString h = "# Do not edit this file manually\n\nchecksum=" + QString::number(checksum) + "\n";
    QString streamstr = stream.readAll();

    QFile file_out(historyListWindow->historyPath);

    if (!file_out.isOpen())
    {
        return;
    }

    QTextStream out(&file_out);

    if (!needToDelete)
    {
        if (streamstr != "")
        {
            h += streamstr + "\n";
        }
        out << h;
    }
    else
    {
        if (streamstr != "")
        {
            int past = 0;
            for (qsizetype i = 0; i < streamstr.length(); i++)
            {
                if (streamstr.mid(i, 2) == "-\n")
                {
                    if (past == needToDelete)
                    {
                        streamstr = streamstr.mid(i);
                    }
                    else
                    {
                        past++;
                    }
                }
            }
            h += streamstr + "\n";
        }
        out << h;
    }
    file_out.close();
    states::saveStateToFile(historyListWindow->historyPath + "s");
    QFile::remove(historyListWindow->historyPath);
    QFile::rename(historyListWindow->historyPath + "s", historyListWindow->historyPath);
}

// Show the console
void MainWindow::on_console_pushButton_clicked()
{
    consoleWindow->show();
    consoleWindow->activateWindow();
    consoleWindow->raise();
}

void MainWindow::changeWadLName()
{
    QString p = ui->wads_listWidget->item(0)->toolTip();
    if (p.mid(p.length() - 4) == ".wad")
    {
        int lastBar = 0;
        for (qsizetype i = 0; i < p.length(); i++)
        {
            if (p[i] == QDir::separator())
            {
                lastBar = i + 1;
            }
        }
        p = p.mid(lastBar);
        ui->wadDSDA_lineEdit->setText(p.mid(0, p.length() - 4));
    }
}

// Add pwads to be loaded
void MainWindow::on_addWads_toolButton_clicked()
{
    QStringList files_list = QFileDialog::getOpenFileNames(this, tr("Select WAD file"), settings->value("primaryPWADFolder").toString(), tr("WAD files (*.wad *.deh *.bex *.zip)"));
    if (files_list.length() > 0)
    {
        foreach (QString filePath, files_list)
        {
            ui->wads_listWidget->addItem(getFileName(filePath));
            ui->wads_listWidget->item(ui->wads_listWidget->count() - 1)->setToolTip(filePath);
        }
        settings->setValue("primaryPWADFolder", files_list[0]); // Make the folder you got this pwad to be the primary folder for pwads
    }
}

// Remove a pwad from the list
void MainWindow::on_removeWads_toolButton_clicked() { ui->wads_listWidget->takeItem(ui->wads_listWidget->currentRow()); }

// These are the parameter toggles
void MainWindow::on_toggle1_checkBox_toggled(bool checked) { isFast = checked; }
void MainWindow::on_toggle2_checkBox_toggled(bool checked) { isNoMo = checked; }
void MainWindow::on_toggle3_checkBox_toggled(bool checked) { isRespawn = checked; }
void MainWindow::on_toggle4_checkBox_toggled(bool checked) { isSoloNet = checked; }

void MainWindow::on_fullscreen_checkBox_toggled(bool checked) { isFullscreen = checked ? "f" : "w"; }
void MainWindow::on_tooltip_pushButton_clicked()
{
    if (ui->tooltip_textBrowser->isHidden()) ui->tooltip_textBrowser->show();
    else ui->tooltip_textBrowser->hide();
}

void MainWindow::on_record_pushButton_clicked() // Record demo
{
    QString demoName = QFileDialog::getSaveFileName(this, tr("Demo file"), settings->value("demofolder").toString(), tr("lmp files (*.lmp)"));
    settings->setValue("demofolder", demoName);
    ui->record_lineEdit->setText(demoName);
}

void MainWindow::on_playback_pushButton_clicked() // Play demo
{
    QString demoName = QFileDialog::getOpenFileName(this, tr("Demo file"), settings->value("demofolder").toString(), tr("lmp files (*.lmp)"));
    settings->setValue("demofolder", demoName);
    ui->playback_lineEdit->setText(demoName);
}

void MainWindow::on_additionalArguments_pushButton_clicked()
{
    if (!canLaunch) return;

    QString path;

#ifdef __APPLE__
    path = execPath + "/../Resources/" + exeName;
#elif __linux__
    path = execPath + "/" + exeName;
#else
    path = execPath + "/" + exeName + ".exe";
#endif

    QFile port = QFile(path);
    if (port.exists())
    {
#ifdef __APPLE__
        QProcess *process = new QProcess;
        process->startDetached("sh", {"-c", "echo \"" + path + " --help ; rm /tmp/dsda-doom-params.sh\" > /tmp/dsda-doom-params.sh ; chmod +x /tmp/dsda-doom-params.sh ; open -a Terminal /tmp/dsda-doom-params.sh"});
        process->deleteLater();
#elif _WIN32
        system(("start cmd.exe /k \"" + path.toStdString() + "\" --help").c_str());
#else
        // xterm is the most reliable terminal to use, but it seems a few distros do not have it
        system(("xterm -e 'bash -c \"" + path.toStdString() + " --help ;bash\"'").c_str());
#endif
    }

    canLaunch = false;
    QTimer::singleShot(2000, this, SLOT(delayLaunch()));
}

QString demoFile;
void MainWindow::get_leaderboards(QString wad, QString level, QString category) // Get the WR for a run from dsdarchive.com
{
    if (!QSslSocket::supportsSsl())
    {
        showSSLDialog();
        return;
    }

    QString player;
    QString time;
    QString engine;
    QString date;

    QNetworkRequest req(QUrl("https://dsdarchive.com/api/demos/records?wad=" + wad + "&level=" + level + "&category=" + category));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json.insert("player", "value1");

    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.get(req);

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument jsondoc = QJsonDocument::fromJson(response_data);

    QJsonObject jsonobj = jsondoc.object();
    foreach (const QString &key, jsonobj.keys())
    {
        QJsonValue value = jsonobj.value(key);
        if (key == "player")
        {
            player = value.toString();
        }
        else if (key == "time")
        {
            time = value.toString();
        }
        else if (key == "engine")
        {
            engine = value.toString();
        }
        else if (key == "date")
        {
            QString dateString = value.toString();
            dateString = dateString.mid(0, dateString.indexOf("T"));
            date = dateString;
        }
        else if (key == "file")
        {
            demoFile = value.toString();
        }
    }
    ui->demoTime->setText(("Time: " + time));
    ui->demoPlayer->setText((player));
    ui->demoPort->setText((date));

    reply->deleteLater();
}

void MainWindow::on_dsda_comboBox_currentIndexChanged(int index) // This is the category box (UV speed, UV max, etc)
{
    reloadLeaderboard(false, false);
}

bool reloadingLeaderboards = false;

void MainWindow::reloadLeaderboard(bool changeWad, bool callApi)
{

    reloadingLeaderboards = true;
    QString arg1 = ui->dsda_comboBox->currentText();
    QString category;

    // We need to change " " with "%20"
    if (ui->iwad_comboBox->currentText() == "heretic")
    {
        if (arg1 == "SM Speed")
        {
            category = "SM%20Speed";
        }
        else if (arg1 == "SM Max")
        {
            category = "SM%20Max";
        }
        else if (arg1 == "BP Speed")
        {
            category = "BP%20Speed";
        }
        else if (arg1 == "BP Max")
        {
            category = "BP%20Max";
        }
        else if (arg1 == "NM Speed")
        {
            category = "NM%20Speed";
        }
        else if (arg1 == "NM 100S")
        {
            category = "NM%20100S";
        }
        else if (arg1 == "Tyson")
        {
            category = "Tyson";
        }
        else if (arg1 == "Pacifist")
        {
            category = "Pacifist";
        }
        else if (arg1 == "Stroller")
        {
            category = "Stroller";
        }
        else if (arg1 == "NoMo")
        {
            category = "NoMo";
        }
        else if (arg1 == "NoMo 100S")
        {
            category = "NoMo%20100S";
        }
        else if (arg1 == "Collector")
        {
            category = "Collector";
        }
    }
    else if (ui->iwad_comboBox->currentText() == "hexen")
    {
        if (arg1 == "Sk4 Speed")
        {
            category = "Sk4%20Speed";
        }
        else if (arg1 == "Sk4 Max")
        {
            category = "Sk4%20Max";
        }
        else if (arg1 == "Sk5 Speed")
        {
            category = "Sk5%20Speed";
        }
        else if (arg1 == "Sk5 Max")
        {
            category = "Sk5%20Max";
        }
        else if (arg1 == "Tyson")
        {
            category = "Tyson";
        }
        else if (arg1 == "Pacifist")
        {
            category = "Pacifist";
        }
        else if (arg1 == "Stroller")
        {
            category = "Stroller";
        }
        else if (arg1 == "NoMo")
        {
            category = "NoMo";
        }
        else if (arg1 == "NoMo 100S")
        {
            category = "NoMo%20100S";
        }
        else if (arg1 == "Collector")
        {
            category = "Collector";
        }
    }
    else
    {
        if (arg1 == "UV Speed")
        {
            category = "UV%20Speed";
        }
        else if (arg1 == "UV Max")
        {
            category = "UV%20Max";
        }
        else if (arg1 == "UV Fast")
        {
            category = "UV%20Fast";
        }
        else if (arg1 == "UV Respawn")
        {
            category = "UV%20Respawn";
        }
        else if (arg1 == "NM Speed")
        {
            category = "NM%20Speed";
        }
        else if (arg1 == "NM 100S")
        {
            category = "NM%20100S";
        }
        else if (arg1 == "Tyson")
        {
            category = "Tyson";
        }
        else if (arg1 == "Pacifist")
        {
            category = "Pacifist";
        }
        else if (arg1 == "Stroller")
        {
            category = "Stroller";
        }
        else if (arg1 == "NoMo")
        {
            category = "NoMo";
        }
        else if (arg1 == "NoMo 100S")
        {
            category = "NoMo%20100S";
        }
        else if (arg1 == "Collector")
        {
            category = "Collector";
        }
    }

    QString wad;
    QString level;

    if (changeWad)
    {
        wad = ui->iwad_comboBox->currentText().toLower();
        if (wad == "doomu" || wad == "doom1")
        {
            wad = "doom";
        }
    }
    else
    {
        wad = ui->wadDSDA_lineEdit->text().toLower();
    }

    if (ui->levelDSDA_lineEdit->text() == "" || changeWad)
    {
        if (!ui->level_lineEdit->isHidden())
        {
            level = "E1M1";
        }
        else
        {
            level = "Map01";
        }
        ui->levelDSDA_lineEdit->setText(level);
    }

    level = ui->levelDSDA_lineEdit->text();

    wad.erase(std::remove(wad.begin(), wad.end(), ' '), wad.end());
    level.erase(std::remove(level.begin(), level.end(), ' '), level.end());

    if (level.mid(0, 2).toLower() == "ep")
    {
        level = "Episode%20" + level.mid(2);
    }
    else if (level[0].toLower() == 'e')
    {
        level = "E" + level.mid(1, 1) + "M" + level.mid(3);
    }
    else if (level.mid(0, 3).toLower() == "map")
    {
        if (level.mid(3).length() == 1) level = "Map%200" + level.mid(3);
        else level = "Map%20" + level.mid(3);
    }
    else if (level.toLower() == "d2all")
    {
        level = "D2All";
    }

    ui->wadDSDA_lineEdit->setText(wad);

    if (callApi)
    {
        reloadingLeaderboards = false;
        get_leaderboards(wad, level, category);
    }
    else
    {
        clearLeaderboard();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) // ENTER makes the game start
{
    if (event->key() == 0x01000005 || event->key() == 0x01000004) // Key is either ENTER or RETURN
    {
        on_launchGame_pushButton_clicked(false, false, "");
    }
}

bool MainWindow::eventFilter(QObject *object, QEvent *ev) // ENTER does not work on the additional parameters box
{

    if (object == (QObject *)ui->launchGame_pushButton)
    {
        // if (ev->type() == QEvent::Enter)
        //{
        //     on_launchGame_pushButton_clicked(false, true);
        //     return QWidget::eventFilter(object, ev);
        // }
        if (ev->type() == QEvent::MouseButtonPress)
        {
            on_launchGame_pushButton_clicked(false, false, "");
            return QWidget::eventFilter(object, ev);
        }
    }
    if (ev->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = (QKeyEvent *)ev;

        if (keyEvent->key() == 0x01000005 || keyEvent->key() == 0x01000004) // Key is either ENTER or RETURN
        {
            on_launchGame_pushButton_clicked(false, false, "");
            return true;
        }
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event) // When closing the launcher, save the settings
{
    if (running)
    {
        gameIsRunning();
        event->ignore();
        return;
    }
    on_launchGame_pushButton_clicked(true, false, "");
    QApplication::quit();
}

void MainWindow::on_showLeaderboard_toolButton_clicked()
{
    QString str = ui->dsda_comboBox->currentText();
    std::replace(str.begin(), str.end(), ' ', '+');
    QString lvl = ui->levelDSDA_lineEdit->text();
    lvl.erase(std::remove(lvl.begin(), lvl.end(), ' '), lvl.end());
    if (lvl[0].toLower() == 'e')
    {
        lvl = "E" + lvl.mid(1, 1) + "M" + lvl.mid(3);
    }
    else if (lvl.mid(0, 3).toLower() == "map")
    {
        if (lvl.mid(3).length() == 1) lvl = "Map%200" + lvl.mid(3);
        else lvl = "Map%20" + lvl.mid(3);
    }
    QDesktopServices::openUrl(QUrl("https://dsdarchive.com/wads/" + ui->wadDSDA_lineEdit->text() + "/leaderboard?category=" + str + "&level=" + lvl));
}

void MainWindow::clearLeaderboard()
{
    ui->demoTime->setStyleSheet("color: rgb(150, 150, 150);");
    ui->demoPlayer->setStyleSheet("color: rgb(150, 150, 150);");
    ui->demoPort->setStyleSheet("color: rgb(150, 150, 150);");
    ui->reloadLeaderboard_toolButton->show();
}

void MainWindow::on_reloadLeaderboard_toolButton_clicked()
{
    reloadLeaderboard(false, true);
    ui->demoTime->setStyleSheet("");
    ui->demoPlayer->setStyleSheet("");
    ui->demoPort->setStyleSheet("");
    ui->reloadLeaderboard_toolButton->hide();
}

void MainWindow::on_wadDSDA_lineEdit_textChanged(const QString &arg1) { clearLeaderboard(); }

void MainWindow::on_levelDSDA_lineEdit_textChanged(const QString &arg1) { clearLeaderboard(); }

void MainWindow::on_viddump_lineEdit_textChanged(const QString &arg1)
{
    if (arg1 == "")
    {
        ui->viddump_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(150, 150, 150); background-color: rgb(255, 255, 255); border-radius:3px");
    }
    else
    {
        ui->viddump_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(0, 0, 0); background-color: rgb(255, 255, 255); border-radius:3px");
    }
}

void MainWindow::on_playback_comboBox_currentIndexChanged(int index)
{
    if (index != 1)
    {
        ui->viddump_lineEdit->setHidden(true);
        ui->viddump_pushButton->setHidden(true);
    }
    else
    {
        ui->viddump_lineEdit->setHidden(false);
        ui->viddump_pushButton->setHidden(false);
    }
}

void MainWindow::on_viddump_pushButton_clicked()
{
    QString vidName = QFileDialog::getSaveFileName(this, tr("mp4 / mkv"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr("video files(*.mp4 *.mkv)"));
    ui->viddump_lineEdit->setText(vidName);
}

void MainWindow::on_showCommandLine_pushButton_clicked() { on_launchGame_pushButton_clicked(false, true, ""); }

void MainWindow::on_episode_lineEdit_textChanged(const QString &arg1) { enable_disable_skill_comboBox(); }

void MainWindow::on_nextPage_pushButton_clicked()
{
    ui->nextPage_pushButton->setText("-");
    nextStackedWidget();
}

void MainWindow::on_previousPage_pushButton_clicked()
{
    ui->previousPage_pushButton->setText("-");
    previousStackedWidget();
}

void MainWindow::nextStackedWidget()
{
    ui->stackedWidget->widget(ui->stackedWidget->currentIndex() + 1)->show();

    QPropertyAnimation *anim = new QPropertyAnimation(ui->stackedWidget->currentWidget(), "pos");
    anim->setDuration(350);
    anim->setEasingCurve(QEasingCurve::OutQuart);
    anim->setStartValue(QPoint(0, 0));
    anim->setEndValue(QPoint(-265, 0));

    QPropertyAnimation *animn = new QPropertyAnimation(ui->stackedWidget->widget(ui->stackedWidget->currentIndex() + 1), "pos");
    animn->setDuration(350);
    animn->setEasingCurve(QEasingCurve::OutQuart);
    animn->setStartValue(QPoint(265, 0));
    animn->setEndValue(QPoint(0, 0));

    QParallelAnimationGroup *animgroup = new QParallelAnimationGroup;
    animgroup->addAnimation(anim);
    animgroup->addAnimation(animn);

    animgroup->start(QAbstractAnimation::DeleteWhenStopped);

    connect(animgroup, SIGNAL(finished()), this, SLOT(whenAnimationFinishn()));
}

void MainWindow::previousStackedWidget()
{
    ui->stackedWidget->widget(ui->stackedWidget->currentIndex() - 1)->show();

    QPropertyAnimation *anim = new QPropertyAnimation(ui->stackedWidget->currentWidget(), "pos");
    anim->setDuration(350);
    anim->setEasingCurve(QEasingCurve::OutQuart);
    anim->setStartValue(QPoint(0, 0));
    anim->setEndValue(QPoint(265, 0));

    QPropertyAnimation *animn = new QPropertyAnimation(ui->stackedWidget->widget(ui->stackedWidget->currentIndex() - 1), "pos");
    animn->setDuration(350);
    animn->setEasingCurve(QEasingCurve::OutQuart);
    animn->setStartValue(QPoint(-265, 0));
    animn->setEndValue(QPoint(0, 0));

    QParallelAnimationGroup *animgroup = new QParallelAnimationGroup;
    animgroup->addAnimation(anim);
    animgroup->addAnimation(animn);

    animgroup->start(QAbstractAnimation::DeleteWhenStopped);

    connect(animgroup, SIGNAL(finished()), this, SLOT(whenAnimationFinishp()));
}

void MainWindow::whenAnimationFinishn()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
    ui->nextPage_pushButton->setText(">");
}

void MainWindow::whenAnimationFinishp()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() - 1);
    ui->previousPage_pushButton->setText("<");
}

void MainWindow::on_hud_lineEdit_textChanged(const QString &arg1)
{
    if (arg1 == "")
    {
        ui->hud_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(150, 150, 150); background-color: rgb(255, 255, 255); border-radius:3px");
    }
    else
    {
        ui->hud_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(0, 0, 0); background-color: rgb(255, 255, 255); border-radius:3px");
    }
}

void MainWindow::on_config_lineEdit_textChanged(const QString &arg1)
{
    if (arg1 == "")
    {
        ui->config_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: 20px; color: rgb(150, 150, 150); background-color: rgb(255, 255, 255); border-radius:3px");
    }
    else
    {
        ui->config_lineEdit->setStyleSheet("border: 1px solid rgb(180, 180, 180); padding-left: 6px;height: "
                                           "20px; color: rgb(0, 0, 0); background-color: rgb(255, 255, 255); "
                                           "border-radius:3px");
    }
}

void MainWindow::on_hud_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("DSDAHUD.lmp"), settings->value("hudfolder").toString(), tr("DSDAHUD file (*.lmp *.txt)"));
    if (fileName.size() != 0)
    {
        settings->setValue("hudfolder", fileName);
        ui->hud_lineEdit->setText(fileName);
    }
}

void MainWindow::on_config_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("dsda-doom.cfg"), settings->value("configfolder").toString(), tr("Config file (*.cfg *.txt)"));
    if (fileName.size() != 0)
    {
        settings->setValue("configfolder", fileName);
        ui->config_lineEdit->setText(fileName);
    }
}

void MainWindow::on_playback_lineEdit_textChanged(const QString &arg1)
{
    enable_disable_skill_comboBox();
    enable_disable_complevel_comboBox();
}

QComboBox *MainWindow::iwad_comboBox() { return ui->iwad_comboBox; }
QComboBox *MainWindow::complevel_comboBox() { return ui->complevel_comboBox; }
QLineEdit *MainWindow::episode_lineEdit() { return ui->episode_lineEdit; }
QLineEdit *MainWindow::level_lineEdit() { return ui->level_lineEdit; }
QComboBox *MainWindow::difficulty_comboBox() { return ui->difficulty_comboBox; }
QCheckBox *MainWindow::toggle1_checkBox() { return ui->toggle1_checkBox; }
QCheckBox *MainWindow::toggle2_checkBox() { return ui->toggle2_checkBox; }
QCheckBox *MainWindow::toggle3_checkBox() { return ui->toggle3_checkBox; }
QCheckBox *MainWindow::toggle4_checkBox() { return ui->toggle4_checkBox; }
QComboBox *MainWindow::resolution_comboBox() { return ui->resolution_comboBox; }
QCheckBox *MainWindow::fullscreen_checkBox() { return ui->fullscreen_checkBox; }
QLineEdit *MainWindow::hud_lineEdit() { return ui->hud_lineEdit; }
QLineEdit *MainWindow::config_lineEdit() { return ui->config_lineEdit; }
QComboBox *MainWindow::track_comboBox() { return ui->track_comboBox; }
QComboBox *MainWindow::time_comboBox() { return ui->time_comboBox; }
QListWidget *MainWindow::wads_listWidget() { return ui->wads_listWidget; }
QLineEdit *MainWindow::record_lineEdit() { return ui->record_lineEdit; }
QLineEdit *MainWindow::playback_lineEdit() { return ui->playback_lineEdit; }
QLineEdit *MainWindow::viddump_lineEdit() { return ui->viddump_lineEdit; }
QComboBox *MainWindow::playback_comboBox() { return ui->playback_comboBox; }
QTextEdit *MainWindow::additionalArguments_textEdit() { return ui->additionalArguments_textEdit; }
