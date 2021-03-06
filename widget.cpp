#include "widget.h"
#include "ui_widget.h"
#include <QProcess>
#include <QDateTime>
#include <QDir>
#include <QClipboard>
#include <QSettings>
#include <QFileDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setFixedSize(this->width(),this->height());

    if (QFile("Settings").exists()){
        QSettings settings("Settings", QSettings::IniFormat);

        settings.beginGroup("Bools");
        ui->equalityCheckbox->setChecked(settings.value("Equality").toBool());
        ui->abbreviationCheckbox->setChecked(settings.value("Abbreviation").toBool());
        ui->levenshteinCheckbox->setChecked(settings.value("Levenshtein").toBool());
        ui->prefixCheckbox->setChecked(settings.value("Prefix").toBool());
        ui->suffixCheckbox->setChecked(settings.value("Suffix").toBool());
        ui->substringCheckbox->setChecked(settings.value("Substring").toBool());
        ui->metaphoneCheckbox->setChecked(settings.value("Metaphone").toBool());
        ui->jaroCheckbox->setChecked(settings.value("Jaro").toBool());
        ui->diceCheckbox->setChecked(settings.value("Dice").toBool());
        settings.endGroup();

        settings.beginGroup("Ints");
        ui->levenshteinSpinbox->setValue(settings.value("LevenshteinUpperBound").toInt());
        ui->prefixSpinbox->setValue(settings.value("PrefixUpperBound").toInt());
        ui->suffixSpinbox->setValue(settings.value("SuffixUpperBound").toInt());
        ui->substringSpinbox->setValue(settings.value("SubstringUpperBound").toInt());
        ui->jaroSpinbox->setValue(settings.value("JaroUpperBound").toInt());
        ui->diceSpinbox->setValue(settings.value("DiceUpperBound").toInt());

        ui->levenshteinLowerSpinbox->setValue(settings.value("LevenshteinLowerBound").toInt());
        ui->prefixLowerSpinbox->setValue(settings.value("PrefixLowerBound").toInt());
        ui->suffixLowerSpinbox->setValue(settings.value("SuffixLowerBound").toInt());
        ui->substringLowerSpinbox->setValue(settings.value("SubstringLowerBound").toInt());
        ui->jaroLowerSpinbox->setValue(settings.value("JaroLowerBound").toInt());
        ui->diceLowerSpinbox->setValue(settings.value("DiceLowerBound").toInt());
        settings.endGroup();
    }


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_levenshteinCheckbox_stateChanged(int arg1)
{
    ui->levenshteinSpinbox->setEnabled(arg1);
    ui->levenshteinLowerSpinbox->setEnabled(arg1);
}

void Widget::on_prefixCheckbox_stateChanged(int arg1)
{
    ui->prefixSpinbox->setEnabled(arg1);
    ui->prefixLowerSpinbox->setEnabled(arg1);
}

void Widget::on_suffixCheckbox_stateChanged(int arg1)
{
    ui->suffixSpinbox->setEnabled(arg1);
    ui->suffixLowerSpinbox->setEnabled(arg1);
}

void Widget::on_substringCheckbox_stateChanged(int arg1)
{
    ui->substringSpinbox->setEnabled(arg1);
    ui->substringLowerSpinbox->setEnabled(arg1);
}

void Widget::on_jaroCheckbox_stateChanged(int arg1)
{
    ui->jaroSpinbox->setEnabled(arg1);
    ui->jaroLowerSpinbox->setEnabled(arg1);
}

void Widget::on_diceCheckbox_stateChanged(int arg1)
{
    ui->diceSpinbox->setEnabled(arg1);
    ui->diceLowerSpinbox->setEnabled(arg1);
}

void Widget::on_runButton_clicked()
{
    QProcess *process = new QProcess();

    QString filename = "";
    if(ui->fileNameEdit->text() == ""){
        filename = "sca_" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm") + getOptions();
    }else{
        filename = ui->fileNameEdit->text();
    }

    process->setStandardOutputFile(filename);

    QString command = "clang-tidy -extra-arg=\"-std=c++11\"" + getConfig() + " -checks='-*,misc-suspicious-call-argument' " + ui->pathEdit->text() + " --";

    //process->setWorkingDirectory("/home/varjujan/Asztal");

    process->start("/bin/bash", QStringList() << "-c" << command);
    process->waitForFinished();
    process->close();

    ui->textBrowser->setText(command);

    process = new QProcess();
    process->start("gedit", QStringList() << filename);
    process->waitForStarted();
    //process->close();
}

QString Widget::getOptions()
{
    QString result = "";    

    if(ui->equalityCheckbox->isChecked())
        result += "_EQ";

    if(ui->abbreviationCheckbox->isChecked())
        result += "_AB";

    if(ui->levenshteinCheckbox->isChecked())
        result += "_LE" + QString::number(ui->levenshteinSpinbox->value())
                + "_" + QString::number(ui->levenshteinLowerSpinbox->value());

    if(ui->prefixCheckbox->isChecked())
        result += "_PR" + QString::number(ui->prefixSpinbox->value())
                + "_" + QString::number(ui->prefixLowerSpinbox->value());

    if(ui->suffixCheckbox->isChecked())
        result += "_SU" + QString::number(ui->suffixSpinbox->value())
                + "_" + QString::number(ui->suffixLowerSpinbox->value());

    if(ui->substringCheckbox->isChecked())
        result += "_SS" + QString::number(ui->substringSpinbox->value())
                + "_" + QString::number(ui->substringLowerSpinbox->value());

    if(ui->metaphoneCheckbox->isChecked())
        result += "_ME";

    if(ui->jaroCheckbox->isChecked())
        result += "_JA" + QString::number(ui->jaroSpinbox->value())
                + "_" + QString::number(ui->jaroLowerSpinbox->value());

    if(ui->diceCheckbox->isChecked())
        result += "_DI" + QString::number(ui->diceSpinbox->value())
                + "_" + QString::number(ui->diceLowerSpinbox->value());

    return result;
}

QString Widget::getConfig()
{
    QString result = " -config=\"{CheckOptions: [";

    result += "{key: misc-suspicious-call-argument.Equality, value: " + QString::number(ui->equalityCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Abbreviation, value: " + QString::number(ui->abbreviationCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Levenshtein, value: " + QString::number(ui->levenshteinCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Prefix, value: " + QString::number(ui->prefixCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Suffix, value: " + QString::number(ui->suffixCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Substring, value: " + QString::number(ui->substringCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Metaphone, value: " + QString::number(ui->metaphoneCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.JaroWinkler, value: " + QString::number(ui->jaroCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Dice, value: " + QString::number(ui->diceCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.LevenshteinUpperBound, value: " + QString::number(ui->levenshteinSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.PrefixUpperBound, value: " + QString::number(ui->prefixSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.SuffixUpperBound, value: " + QString::number(ui->suffixSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.SubstringUpperBound, value: " + QString::number(ui->substringSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.JaroWinklerUpperBound, value: " + QString::number(ui->jaroSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.DiceUpperBound, value: " + QString::number(ui->diceSpinbox->value()) + "}, ";

    result += "{key: misc-suspicious-call-argument.LevenshteinLowerBound, value: " + QString::number(ui->levenshteinLowerSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.PrefixLowerBound, value: " + QString::number(ui->prefixLowerSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.SuffixLowerBound, value: " + QString::number(ui->suffixLowerSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.SubstringLowerBound, value: " + QString::number(ui->substringLowerSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.JaroWinklerLowerBound, value: " + QString::number(ui->jaroLowerSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.DiceLowerBound, value: " + QString::number(ui->diceLowerSpinbox->value()) + "}";

    result += "]}\"";
    return result;
}

void Widget::on_runScriptButton_clicked()
{
    QProcess *process = new QProcess();

    QString filename = "";
    if(ui->scriptFilenameEdit->text() == ""){
        filename = "sca_" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm") + getOptions();
    }else{
        filename = ui->scriptFilenameEdit->text();
    }

    //process->setStandardOutputFile(filename);

    QString command = "cd;cd " + ui->scriptLocationEdit->text() + ";" + "python run-clang-tidy.py " + getConfig() +
            " -checks='-*,misc-suspicious-call-argument' -> " + QDir::currentPath() + "/" + filename;

    /*process->setWorkingDirectory(ui->scriptLocationEdit->text());

    this->setCursor(Qt::WaitCursor);*/
    process->start("gnome-terminal");
    process->waitForStarted();

    ui->textBrowser->setText(command);

    QApplication::clipboard()->setText(command);


    /*process = new QProcess();
    process->start("gedit", QStringList() << ui->scriptLocationEdit->text() + filename);
    process->waitForFinished();
    process->close();
    this->setCursor(Qt::ArrowCursor);*/
}

void Widget::on_saveButton_clicked()
{
    QSettings settings("Settings", QSettings::IniFormat);

    settings.beginGroup("Bools");
    settings.setValue("Equality", ui->equalityCheckbox->isChecked());
    settings.setValue("Abbreviation", ui->abbreviationCheckbox->isChecked());
    settings.setValue("Levenshtein", ui->levenshteinCheckbox->isChecked());
    settings.setValue("Prefix", ui->prefixCheckbox->isChecked());
    settings.setValue("Suffix", ui->suffixCheckbox->isChecked());
    settings.setValue("Substring", ui->substringCheckbox->isChecked());
    settings.setValue("Metaphone", ui->metaphoneCheckbox->isChecked());
    settings.setValue("Jaro", ui->jaroCheckbox->isChecked());
    settings.setValue("Dice", ui->diceCheckbox->isChecked());
    settings.endGroup();

    settings.beginGroup("Ints");
    settings.setValue("LevenshteinUpperBound", ui->levenshteinSpinbox->value());
    settings.setValue("PrefixUpperBound", ui->prefixSpinbox->value());
    settings.setValue("SuffixUpperBound", ui->suffixSpinbox->value());
    settings.setValue("SubstringUpperBound", ui->substringSpinbox->value());
    settings.setValue("JaroUpperBound", ui->jaroSpinbox->value());
    settings.setValue("DiceUpperBound", ui->diceSpinbox->value());

    settings.setValue("LevenshteinLowerBound", ui->levenshteinLowerSpinbox->value());
    settings.setValue("PrefixLowerBound", ui->prefixLowerSpinbox->value());
    settings.setValue("SuffixLowerBound", ui->suffixLowerSpinbox->value());
    settings.setValue("SubstringLowerBound", ui->substringLowerSpinbox->value());
    settings.setValue("JaroLowerBound", ui->jaroLowerSpinbox->value());
    settings.setValue("DiceLowerBound", ui->diceLowerSpinbox->value());
    settings.endGroup();
}

void Widget::on_browseScriptLocationBUtton_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);

    if(dialog.exec()){
        QStringList fodlerName = dialog.selectedFiles();

        ui->scriptLocationEdit->setText(fodlerName[0]);
    }
}
