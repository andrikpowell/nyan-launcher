#include "updater.h"
#include <mainwindow.h>

QString getLatestLauncherVersion()
{
    QString portversion = getGameVersion();

    QNetworkRequest request((QUrl(LAUNCHER_API)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setTransferTimeout(10000);
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.get(request);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QJsonObject jsonobj = QJsonDocument::fromJson(reply->readAll()).object();
    foreach (const QString &key, jsonobj.keys())
    {
        QJsonValue value = jsonobj.value(key);
        if (key == "name")
        {
            return value.toString();
        }
    }

    reply->deleteLater();
    return "";
}

bool updateLauncherDialog(bool manualReq)
{

    QString current = version;
    QString latest = getLatestLauncherVersion();

    QMessageBox msgBox;
    msgBox.setText("Nyan Launcher " + current);
    msgBox.setWindowTitle("Update");
    msgBox.adjustSize();

    if (latest == "" && manualReq)
    {
        msgBox.setInformativeText("Failed to get information about the latest Nyan Launcher version. Try again later.");
        msgBox.addButton("Close", QMessageBox::NoRole);
        msgBox.exec();
    }
    else if (current != latest && latest != "")
    {
        msgBox.setInformativeText("Available: " + latest +
                                  "\n\n"
                                  "This will open the website where you can download the latest version.");

        QPushButton *buttonYes = msgBox.addButton("Update", QMessageBox::YesRole);
        msgBox.addButton("Close", QMessageBox::NoRole);
        msgBox.setDefaultButton(buttonYes);
        msgBox.exec();
        if (msgBox.clickedButton() == buttonYes)
        {
            updateLauncher();
            return true;
        }
    }
    else if (manualReq)
    {
        msgBox.setInformativeText("Up to Date");
        msgBox.addButton("Close", QMessageBox::NoRole);
        msgBox.exec();
    }
    return false;
}

void updateLauncher() { QDesktopServices::openUrl(QUrl(LAUNCHER_REPO + "/releases/latest")); }

QString getGameVersion()
{
    QString path = getGamePath();

    if (QFile(path).exists())
    {
        QProcess process;
        process.setWorkingDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        process.start(path, {"-v"});
        process.waitForFinished();

        QList<QByteArray> output = process.readAll().split(' ');

        if (output.size() >= 2) return output[1];
        else return "";
    }
    else return "";
}

QString getLatestGameVersion()
{
    QString portversion = getGameVersion();

    QNetworkRequest request((QUrl(GAME_API)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setTransferTimeout(10000);
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.get(request);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QJsonObject jsonobj = QJsonDocument::fromJson(reply->readAll()).object();
    foreach (const QString &key, jsonobj.keys())
    {
        QJsonValue value = jsonobj.value(key);
        if (key == "name")
        {
            return value.toString();
        }
    }

    reply->deleteLater();
    return "";
}

bool updateGameDialog(bool manualReq)
{
    QString current = getGameVersion();
    QString latest = getLatestGameVersion();

    QMessageBox msgBox;
    msgBox.setText("Nyan Doom " + current);
    msgBox.setWindowTitle("Update");
    msgBox.adjustSize();

    if (current == "")
    {
#if defined(Q_OS_WIN)
        msgBox.setInformativeText("It seems you do not have Nyan Doom installed.\n"
                                  "Make sure that the Launcher and Nyan Doom are in the same folder  OR  click 'Install'"
                                  "\n\n"
                                  "This will download and extract the latest version to the current folder.\n"
                                  "A CMD window will appear to perform this action.");
        QPushButton *buttonYes = msgBox.addButton("Install", QMessageBox::YesRole);
#elif defined(Q_OS_MACOS)
        msgBox.setInformativeText("It seems you do not have Nyan Doom installed.\n"
                                  "Click 'Install'"
                                  "\n\n"
                                  "This will download and install the latest version, inside the Launcher's App Bundle.\n"
                                  "A Terminal window will appear to perform this action.");
        QPushButton *buttonYes = msgBox.addButton("Install", QMessageBox::YesRole);
#elif defined(Q_OS_LINUX)
        msgBox.setInformativeText("It seems you do not have Nyan Doom installed.\n"
                                  "Install Nyan Doom using your prefered package manager  OR  Download the official AppImage and put it in your $PATH");
        QPushButton *buttonYes = msgBox.addButton("Go to Downloads", QMessageBox::YesRole);
#endif
        msgBox.addButton("Close", QMessageBox::NoRole);
        msgBox.setDefaultButton(buttonYes);
        msgBox.exec();
        if (msgBox.clickedButton() == buttonYes)
        {
            updateGame();
            return true;
        }
    }
    else if (latest == "" && manualReq)
    {
        msgBox.setInformativeText("Failed to get information about the latest Nyan Doom version. Try again later.");
        msgBox.addButton("Close", QMessageBox::NoRole);
        msgBox.exec();
    }
    else if (current != latest && latest != "")
    {
#if defined(Q_OS_WIN)
        msgBox.setInformativeText("Available: " + latest +
                                  "\n\n"
                                  "This will download and extract the latest version to the current folder.\n"
                                  "A CMD window will appear to perform this action.");
        QPushButton *buttonYes = msgBox.addButton("Update", QMessageBox::YesRole);
#elif defined(Q_OS_MACOS)
        msgBox.setInformativeText("Available: " + latest +
                                  "\n\n"
                                  "This will download and install the latest version, inside the Launcher's App Bundle.\n"
                                  "A Terminal window will appear to perform this action.");
        QPushButton *buttonYes = msgBox.addButton("Update", QMessageBox::YesRole);
#elif defined(Q_OS_LINUX)
        msgBox.setInformativeText("Available: " + latest +
                                  "\n\n"
                                  "You can get the latest version by using your prefered package manager  OR  Downloading the official AppImage and putting it in your $PATH");
        QPushButton *buttonYes = msgBox.addButton("Go to Downloads", QMessageBox::YesRole);
#endif
        msgBox.addButton("Close", QMessageBox::NoRole);
        msgBox.setDefaultButton(buttonYes);
        msgBox.exec();
        if (msgBox.clickedButton() == buttonYes)
        {
            updateGame();
            return true;
        }
    }
    else if (manualReq)
    {
        msgBox.setInformativeText("Up to Date");
        msgBox.addButton("Close", QMessageBox::NoRole);
        msgBox.exec();
    }
    return false;
}

void updateGame()
{
    // clang-format off
#if defined(Q_OS_MAC)
    QProcess process;
    process.startDetached("sh", {"-c", "rm /tmp/nyan-updater-macos.sh;"
                                       "curl -L -o /tmp/nyan-updater-macos.sh " + GAME_UPDATER_MACOS + ";"
                                       "p2=" + launcherfolder + "/../Resources;"
                                       "p2=${p2//\\//\\\\/};"
                                       "sed -i -e s/'$1'/$p2/g /tmp/nyan-updater-macos.sh;"
                                       "chmod +x /tmp/nyan-updater-macos.sh;"
                                       "open -na Terminal --args /tmp/nyan-updater-macos.sh"});
#elif defined(Q_OS_WIN)
    const QString tmpDir = "%TEMP%/nyan-doom-temp";
    const QString batPath = tmpDir + "/nyan-updater-windows.bat";

    QString dest = QDir::toNativeSeparators(launcherfolder);
    while (dest.endsWith('\\') || dest.endsWith('/')) dest.chop(1);

    QProcess process;
    process.setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
                                              {
                                                  args->flags |= CREATE_NEW_CONSOLE;
                                                  args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
                                                  // args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
                                              });

    QString cmd =
        "@echo on"
        " & setlocal EnableExtensions DisableDelayedExpansion"
        " & echo --- begin launcher updater debug ---"
        " & echo raw tmpDir literal = [" + tmpDir + "]"
        " & echo raw batPath literal = [" + batPath + "]"
        " & echo raw dest literal   = [" + dest + "]"
        " & echo."

        " & echo [1] set TMP"
        " & set \"TMP=" + tmpDir + "\""
        " & echo TMP after set = [%TMP%]"
        " & echo errorlevel after set = %errorlevel%"
        " & if not \"%errorlevel%\"==\"0\" (echo FAIL at step 1 & pause & exit /b 1)"
        " & echo."

        " & echo [2] mkdir TMP if missing"
        " & if not exist \"%TMP%\" mkdir \"%TMP%\""
        " & echo errorlevel after mkdir = %errorlevel%"
        " & if not \"%errorlevel%\"==\"0\" (echo FAIL at step 2 & pause & exit /b 2)"
        " & echo."

        " & echo [3] where curl"
        " & where curl"
        " & echo errorlevel after where curl = %errorlevel%"
        " & if not \"%errorlevel%\"==\"0\" (echo FAIL at step 3 & pause & exit /b 3)"
        " & echo."

        " & echo [4] curl download"
        " & echo curl -o \"" + batPath + "\" \"" + GAME_UPDATER_WINDOWS + "\""
        " & curl -L --fail --silent --show-error -o \"" + batPath + "\" \"" + GAME_UPDATER_WINDOWS + "\""
        " & echo errorlevel after curl = %errorlevel%"
        " & if not \"%errorlevel%\"==\"0\" (echo FAIL at step 4 & pause & exit /b 4)"
        " & echo."

        " & echo [5] dir bat"
        " & dir \"" + batPath + "\""
        " & echo errorlevel after dir = %errorlevel%"
        " & if not \"%errorlevel%\"==\"0\" (echo FAIL at step 5 & pause & exit /b 5)"
        " & echo."

        " & echo [6] call updater"
        " & echo call \"" + batPath + "\" \"" + dest + "\""
        " & call \"" + batPath + "\" \"" + dest + "\""
        " & echo errorlevel after call = %errorlevel%"
        " & if not \"%errorlevel%\"==\"0\" (echo FAIL at step 6 & pause & exit /b 6)"

        " & echo."
        " & echo --- done ---"
        " & pause";

    process.setProgram("cmd.exe");
    process.setArguments({"/s", "/k", cmd});
    process.startDetached();
#elif defined(Q_OS_LINUX)
    QDesktopServices::openUrl(QUrl(GAME_REPO + "/releases/latest"));
#endif
    // clang-format on
}
