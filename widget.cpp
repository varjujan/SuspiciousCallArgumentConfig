#include "widget.h"
#include "ui_widget.h"
#include <QProcess>
#include <QDateTime>
#include <QDir>
#include <QClipboard>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setFixedSize(this->width(),this->height());


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_levenshteinCheckbox_stateChanged(int arg1)
{
    ui->levenshteinSpinbox->setEnabled(arg1);
}

void Widget::on_prefixCheckbox_stateChanged(int arg1)
{
    ui->prefixSpinbox->setEnabled(arg1);
}

void Widget::on_suffixCheckbox_stateChanged(int arg1)
{
    ui->suffixSpinbox->setEnabled(arg1);
}

void Widget::on_substringCheckbox_stateChanged(int arg1)
{
    ui->substringSpinbox->setEnabled(arg1);
}

void Widget::on_jaroCheckbox_stateChanged(int arg1)
{
    ui->jaroSpinbox->setEnabled(arg1);
}

void Widget::on_diceCheckbox_stateChanged(int arg1)
{
    ui->diceSpinbox->setEnabled(arg1);
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
    process->waitForFinished();
    process->close();
}

QString Widget::getOptions()
{
    QString result = "";

    if(ui->distHeurCheckbox->isChecked())
        result += "_DH";

    if(ui->equalityCheckbox->isChecked())
        result += "_EQ";

    if(ui->levenshteinCheckbox->isChecked())
        result += "_LE" + QString::number(ui->levenshteinSpinbox->value());

    if(ui->prefixCheckbox->isChecked())
        result += "_PR" + QString::number(ui->prefixSpinbox->value());

    if(ui->suffixCheckbox->isChecked())
        result += "_SU" + QString::number(ui->suffixSpinbox->value());

    if(ui->substringCheckbox->isChecked())
        result += "_SS" + QString::number(ui->substringSpinbox->value());

    if(ui->metaphoneCheckbox->isChecked())
        result += "_ME";

    if(ui->jaroCheckbox->isChecked())
        result += "_JA" + QString::number(ui->jaroSpinbox->value());

    if(ui->diceCheckbox->isChecked())
        result += "_DI" + QString::number(ui->diceSpinbox->value());

    return result;
}

QString Widget::getConfig()
{
    QString result = " -config=\"{CheckOptions: [";

    result += "{key: misc-suspicious-call-argument.DistHeur, value: " + QString::number(ui->distHeurCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Equality, value: " + QString::number(ui->equalityCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Levenshtein, value: " + QString::number(ui->levenshteinCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Prefix, value: " + QString::number(ui->prefixCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Suffix, value: " + QString::number(ui->suffixCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Substring, value: " + QString::number(ui->substringCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Metaphone, value: " + QString::number(ui->metaphoneCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.JaroWinkler, value: " + QString::number(ui->jaroCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.Dice, value: " + QString::number(ui->diceCheckbox->isChecked()) + "}, ";
    result += "{key: misc-suspicious-call-argument.LevenshteinLimit, value: " + QString::number(ui->levenshteinSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.PrefixLimit, value: " + QString::number(ui->prefixSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.SuffixLimit, value: " + QString::number(ui->suffixSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.SubstringLimit, value: " + QString::number(ui->substringSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.JaroWinklerLimit, value: " + QString::number(ui->jaroSpinbox->value()) + "}, ";
    result += "{key: misc-suspicious-call-argument.DiceLimit, value: " + QString::number(ui->diceSpinbox->value()) + "}";

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
