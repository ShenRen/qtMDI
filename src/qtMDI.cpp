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

#include "qtMDI.h"


qtMDI::qtMDI(int argc, char** argv)
{
QString str, str2;

    started = false;
    
    // build the main window from ui
    setupUi(this);
    qDebug() << argv[1] << argv[2];
        
    bRun = false;
    bPause = false;
    bStep = false;
    bDirect = false;
    bBigFile = false;
    bMoreBig = false;
    filePos = 0L;
    fileSize = 0L;
    chunkIndex = 0;
    listIndex = 1;

    prefixName = "qtMDI";
    iniFile = argv[2];
    
    _hal = new HAL_Access_Slim(argc, argv, prefixName, false);
	
    setupPins();
    
    _hal->engage();   
        

    setupExtra();
    
    startTimer();   

}

//////////////////////////////////////////////////////////////////////////////////
//
// closeEvent() traps the system event and forces closure through onCloseDown()
// any code needs to be in onCloseDown() because other exit methods go straight there.
void qtMDI::closeEvent(QCloseEvent * event )
{
    Q_UNUSED(event);

    onCloseDown();
}

void qtMDI::onCloseDown()
{
    refreshTimer->stop();
    
    _hal->_nml->cleanUp();
    
    writeSettings();
    
    qApp->exit();

}

//////////////////////////////////////////////////////////////////////////////////

void qtMDI::setupPins()
{
QString pin1 = "inposition", pin2 = "msgdisable";
QString motion_inpos = "motion.in-position";
QString msg_disable = "axisui.msg-disable";
QString signame1 = "InPosSig", signame2 = "MsgDisableSig";
QStringList pinList; 

    _hal->createBitPin(pin1, true);
    _hal->createBitPin(pin2, false);
        
    // another way to create sig and link all in one
    pinList << prefixName + "." + pin1 << motion_inpos;
    _hal->netSignalPin(signame1, pinList);  
        
}

///////////////////////////////////////////////////////////////////////////////////

void qtMDI::setupExtra()
{
QString str;

    te_Code->setReadOnly(true);
    te_Code->setLineWrapMode(QPlainTextEdit::NoWrap);
    te_Code->setCenterOnScroll(true);
    
    QCoreApplication::setOrganizationName("MGWare");
    QCoreApplication::setOrganizationDomain("mgware.co.uk");
    QCoreApplication::setApplicationName("qtMDI");
    
    // qt does not understand ~/ so path must be absolute or it will create ~/ on PWD
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    
    str = homedir;
    str += "/.config/MGWare/qtMDI/qtMDI.ini";
    settings = new QSettings(str, QSettings::IniFormat, this);
    
    makeConnections();

    readSettings();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void qtMDI::makeConnections()
{
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));
    connect(actionRecentFiles, SIGNAL(triggered()), this, SLOT(onRecentFiles()));
    connect(actionLoadLast, SIGNAL(triggered()), this, SLOT(onLoadLast()));

    // actionRun is menu item
    connect(actionRun, SIGNAL(triggered()), this, SLOT(onRunProgram()));
    connect(actionRunProgram, SIGNAL(triggered()), this, SLOT(onRunProgram()));
    connect(te_Code, SIGNAL(runFromSelected(int)), this, SLOT(onRunFromContext(int)));
    connect(actionPauseProgram, SIGNAL(triggered()), this, SLOT(pauseProgram()));
    connect(actionStopProgram, SIGNAL(triggered()), this, SLOT(stopProgram()));
    connect(actionStep, SIGNAL(triggered()), this, SLOT(stepProgram()));

    connect(actionAboutQt, SIGNAL(triggered()), this, SLOT(onAboutQt()));
    connect(actionAboutQtMDI, SIGNAL(triggered()), this, SLOT(onAboutQtMDI()));
    
    connect(actionExit, SIGNAL(triggered()), this, SLOT(onCloseDown())); 

}


////////////////////////////  Refresh loop //////////////////////////////////////

void qtMDI::startTimer()
{
    refreshTimer = new QTimer(this);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    //refreshTimer->start(100);
}


void qtMDI::refresh()
{
QString str;
int ret = 0;

    // If last command produced an error, stop
    if(bRun)
        {
        ret == checkErrors();
        if(ret != 0)
            {
            bRun = false;
            return;
            }
        }
    // running prog and last move completed
    if( bRun && (_hal->getBitPinValue(0) == 1 ) )    
        {
        // allow pause button to work even though the hal command will not
        // allow a psuedo step of one line when in pause
        if( (listIndex < te_Code->blockCount() && !bPause) || (listIndex < te_Code->blockCount() && (bPause && bStep)) )
            {
            te_Code->highlightLine(listIndex + 1);
            str = te_Code->textCursor().block().text().trimmed().toLatin1();
            // don't process comments'
            if((str.at(0) != ';') && (str.at(0) != '(') )
                onMDICommand(str);
            actionStep->setChecked(bStep = false);
            listIndex++;
            }

        // if list nearly finished, load next chunk or exit
        if( listIndex >= (te_Code->blockCount() - 20) && (bBigFile && bMoreBig))
            {
            if(reOpenInBrowser() == -1)
                {
                qDebug() << "Error return from reOpenInBrowser()";
                bRun = false;
                }
            }
        // or if completely finished, tidy up
        else if(listIndex >= te_Code->blockCount() )
            {
            te_Code->highlightLine(1);
            bRun = false;
            actionPauseProgram->setChecked(bPause = false);
            actionRunProgram->setChecked(bRun = false);
            str = QString("%1 - Run completed") .arg(openFile);
            statusbar->showMessage(str, 1000);
            }
        }
 
          

}


int qtMDI::checkErrors()
{
QString str;

    // read the EMC status
    if (0 != _hal->_nml->updateStatus()) 
        _hal->_nml->error_string = "Bad Status";

    // read the EMC errors
    if (0 != _hal->_nml->updateError())
        _hal->_nml->error_string = "Bad Status";
    
    // print any result stored by updateError() in error_string
    if (_hal->_nml->error_string.length()) 
        {
        stopProgram();
        str.sprintf("Error at line %d - %s",listIndex, qPrintable(_hal->_nml->error_string) );
        statusbar->showMessage(str);
        _hal->_nml->error_string = "";
        return -1;        
        }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

//
// helper to save problems translating QStrings to char*
// which NML and HAL functions require
//

void qtMDI::QStrncpy(char* str, QString& qstr, int num)
{
int x;

    for(x = 0; x < num; x++)
        {
        str[x] = (char)qstr.at(x).toLatin1();
        }
    str[x] = 0;  // terminating '\0'

}
