
#ifndef TOUCHOFFDLG_H
#define TOUCHOFFDLG_H

#include <QtGui>
#include <QSettings>
#include <QCoreApplication>


#include "ui_touch-off_dlg.h"


class touchOffDialog  : public QDialog , private Ui_TouchOffDlg
{
    Q_OBJECT
public:

    touchOffDialog(QStringList& list, char axis);
    QString cbText;
    float value;
    int index;
    
private:

private slots:
    virtual void onActivated(QString);
    virtual void onAccept();
};


#endif
