#ifndef READERSDIALOG_H
#define READERSDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>


class ReadersDialog : public QDialog {
    Q_OBJECT
public:
    ReadersDialog(QWidget* parent=nullptr);
    void setup();
    void accept_();
    void addItem();
    void delItem();
    void itemUp();
    void itemDown();
    void browse();
    bool added(const QString& ed);

private:
    QListWidget* listWidget;
    QLineEdit* editorLine;
    QDialogButtonBox* validateDialog;
};

#endif