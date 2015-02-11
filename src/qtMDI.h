/********************************************************************
* Copyright (C) 2012 - 2015 ArcEye <arceye AT mgware DOT co DOT uk>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
********************************************************************/

#ifndef QTCNC_H
#define QTCNC_H

#include <QtGui>
#include <QSettings>
#include <QCoreApplication>
#include <QtDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QShortcut>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "HAL_Access_Slim.h"
#include "ui_axis.h"
#include "touch-off_dlg.h"
#include "recent_files_dialog.h"
#include "code_editor.h"


#define MAX_AXES 9
// max file size of 25KB
#define MAX_SIZE 25000
// render initial 3K chunk
#define CHUNK_SIZE 25000
// add in 1K bits dynamically
#define ADD_SIZE 5000

class qtMDI  : public QMainWindow , private Ui_QtMDI
{
    Q_OBJECT
public:

    qtMDI(int argc, char** argv);
    void closeEvent(QCloseEvent * event);  // redefine inherited function from QWidget to control exit
    
    void setupExtra();

    
private:

    HAL_Access_Slim *_hal;
    
    QString prefixName;
    
    bool bRun;
    bool bPause;
    bool bStep;
    bool bDirect;
    bool started;
    
    bool bBigFile;
    bool bMoreBig;
    qint64 filePos;
    qint64 fileSize;
    
    void QStrncpy(char* str, QString& qstr, int num);
    int spare;
   
    int chunkIndex;
    int listIndex;
        
    QTimer *refreshTimer;
    void startTimer();
     
    QString iniFile;   
    QString openFile;
    QString lastFile;
    QStringList listStrings;
    
    QSettings *settings;
    void readSettings();
    void writeSettings();
    QStringList listMDITemp;
    QStringList listRecentFiles;
    
    // _nml->sendMdiCmd(char*) is very picky
    // it will accept a const string or a real char array
    // but not QString.data()
    char lastCommand[80];
    
    int openInBrowser(QString& filename);
    int reOpenInBrowser();
    
    
private:
    void makeConnections();
    void setupPins();
               
private slots:
    virtual void onCloseDown();
    virtual void onRunProgram();
    virtual void onRunFromContext(int line = 1);
    virtual void onRunFrom(int line = 1);
    virtual void pauseProgram();
    virtual void stepProgram();
    virtual void stopProgram();
    
    virtual void onMDICommand(QString& str);
    
    virtual void refresh();
    
    virtual void onAboutQtMDI();
    virtual void onAboutQt();

    virtual void onOpenFile();
    virtual void onRecentFiles();
    virtual void onLoadLast();
    
};




#endif
