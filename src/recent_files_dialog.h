
#ifndef RECENTFILESDIALOG_H
#define RECENTFILESDIALOG_H

#include <QtGui>
#include <QSettings>
#include <QCoreApplication>


#include "ui_recent_files_dialog.h"


class RecentFilesDialog  : public QDialog , private Ui_RecentFilesDialog
{
    Q_OBJECT
public:

    RecentFilesDialog(QWidget *parent, QStringList& list);
    QString selection;
    
private:

private slots:
    virtual void onAcceptLocal();
    virtual void onSelected(QListWidgetItem*);
};


#endif
