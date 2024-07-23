#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QShortcut>
#include <QStandardPaths>
#include <QProcess>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QRegularExpression>
#include <unordered_map>

namespace Ui {
class Console;
}

class Console : public QWidget
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);
    ~Console();
    Ui::Console *ui;

public slots:
    void fooo3();
    void changeText(QString s);
    void clearText();
};

#endif // CONSOLE_H
