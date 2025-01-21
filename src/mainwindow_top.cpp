#include "mainwindow.h"

// If the IWAD selected changes
void MainWindow::on_iwad_comboBox_currentIndexChanged(int index)
{
    QString selection = ui->iwad_comboBox->currentText();

    // These are episode/mission based. They need both warp boxes
    if (exmxIWADS.contains(selection))
    {
        ui->level_label->show();
        ui->level_lineEdit->show();
        ui->episode_label->setText("Episode");
    }
    else // These are map based. They need 1 warp box
    {
        ui->level_label->hide();
        ui->level_lineEdit->hide();
        ui->episode_label->setText("Level");
    }
}

// If the Complevel selected changes
void MainWindow::on_complevel_comboBox_currentIndexChanged(int index)
{
    QString selection = ui->complevel_comboBox->currentText();

    // These are limit-removing compat. They need checkbox
    if(LIMIT_COMPLEVELS.contains(selection))
    {
        ui->limit_checkBox->show();
    }
    else // These are not limit-removing. Deactivate and hide checkbox
    {
        ui->limit_checkBox->setChecked(false);
        ui->limit_checkBox->hide();
    }
}

void MainWindow::on_tooltip_pushButton_clicked()
{
    if (ui->tooltip_textBrowser->isHidden()) ui->tooltip_textBrowser->show();
    else ui->tooltip_textBrowser->hide();
}

void MainWindow::on_episode_lineEdit_textChanged(const QString &arg1) { enable_disable_skill_comboBox(); }

void MainWindow::setComplevelsList(QStringList list)
{
    ui->complevel_comboBox->clear();
    ui->complevel_comboBox->addItems(list);
}

void MainWindow::enable_disable_skill_comboBox()
{
    if (ui->episode_lineEdit->text().isEmpty())
    {
        ui->skill_comboBox->setEnabled(false);
        ui->skill_label->setEnabled(false);
        ui->skill_comboBox->setToolTip("Cannot select a Skill level\n"
                                            "without choosing a Map");
    }
    else if (!ui->playback_lineEdit->text().isEmpty())
    {
        ui->skill_comboBox->setEnabled(false);
        ui->skill_label->setEnabled(false);
        ui->skill_comboBox->setToolTip("Cannot select a Skill level\n"
                                            "during Demo playback");
    }
    else
    {
        ui->skill_comboBox->setEnabled(true);
        ui->skill_label->setEnabled(true);
        ui->skill_comboBox->setToolTip("");
    }
}

void MainWindow::enable_disable_complevel_comboBox()
{
    if (!ui->playback_lineEdit->text().isEmpty())
    {
        ui->complevel_comboBox->setEnabled(false);
        ui->complevel_label->setEnabled(false);
        ui->complevel_comboBox->setToolTip("Cannot select a complevel\n"
                                           "during demo playback");
    }
    else
    {
        ui->complevel_comboBox->setEnabled(true);
        ui->complevel_label->setEnabled(true);
        ui->complevel_comboBox->setToolTip("");
    }
}

void MainWindow::enable_disable_limit_checkBox()
{
    if (!ui->playback_lineEdit->text().isEmpty())
    {
        ui->limit_checkBox->setEnabled(false);
        ui->limit_checkBox->setToolTip("Cannot select limit-removing\n"
                                            "during Demo playback");
    }
    else
    {
        ui->limit_checkBox->setEnabled(true);
        ui->limit_checkBox->setToolTip("");
    }
}
