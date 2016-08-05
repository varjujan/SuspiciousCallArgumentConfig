#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_levenshteinCheckbox_stateChanged(int arg1);

    void on_prefixCheckbox_stateChanged(int arg1);

    void on_suffixCheckbox_stateChanged(int arg1);

    void on_substringCheckbox_stateChanged(int arg1);

    void on_jaroCheckbox_stateChanged(int arg1);

    void on_diceCheckbox_stateChanged(int arg1);

    void on_runButton_clicked();

    QString getOptions();

    QString getConfig();

    void on_runScriptButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
