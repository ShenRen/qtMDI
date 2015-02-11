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
#include <QIODevice>
#include <QByteArray>

///////////////  persistence in MDI command history, files position etc /////////////////


void qtMDI::readSettings()
{
QString str, str2;
int i, lines;

    settings->beginGroup("WINDOW");
    restoreGeometry(settings->value("mainWindowGeometry").toByteArray());
    restoreState(settings->value("mainWindowState").toByteArray());  
    settings->endGroup();

    settings->beginGroup("MDI");
    lines = settings->value("Lines", 0).toInt();
    
    for (i = 0; i < lines; ++i)
        listMDITemp.append(settings->value(str.sprintf("Line%02d", i), "").toString());
    settings->endGroup();
    
    settings->beginGroup("RECENT_FILES");
    lines = settings->value("Files", 0).toInt();
    for (i = 0; i < lines; ++i)
        listRecentFiles.append(settings->value(str.sprintf("Line%02d", i), "").toString());
    settings->endGroup();
    listRecentFiles.removeDuplicates();
    
    settings->beginGroup("LAST_FILE");
    lastFile = settings->value("LastFile", "").toString();
    settings->endGroup();
    
}

void qtMDI::writeSettings()
{
QString str, str2;
int i, lines;
    
    settings->beginGroup("WINDOW");
    settings->setValue("mainWindowGeometry", saveGeometry());
    settings->setValue("mainWindowState", saveState());
    settings->endGroup();
    
    settings->beginGroup("MDI");
    settings->setValue("Lines", lines = listMDITemp.count());
    for (i = 0; i < lines; ++i)
        settings->setValue(str.sprintf("Line%02d", i), listMDITemp.at(i));
    settings->endGroup();

    listRecentFiles.removeDuplicates();
    settings->beginGroup("RECENT_FILES");
    settings->setValue("Files", lines = listRecentFiles.count());
    for (i = 0; i < lines; ++i)
        settings->setValue(str.sprintf("Line%02d", i), listRecentFiles.at(i));
    settings->endGroup();
    
    settings->beginGroup("LAST_FILE");
    settings->setValue("LastFile", openFile);
    settings->endGroup();
}

//////////////////////////////////////////////////////////////////////

void qtMDI::onOpenFile()
{
QString filename;

    if(!bRun)
        {
        filename = QFileDialog::getOpenFileName(this, tr("Open GCode"), _hal->_nml->programPrefix, tr("GCode Files (*.ngc *.nc);; All files (*.*)"));
        if(filename.length())
            openInBrowser(filename);
        }
}


int qtMDI::openInBrowser(QString& filename)
{
QFile file(filename);
QString str, str2;

    if (file.open(QFile::ReadOnly | QFile::Text))
        {
        fileSize = file.size();
        str = "Loading file " + filename;
        statusbar->showMessage(str, 1000);
        te_Code->clear();        

        QTextStream ts(&file);

        while( (!ts.atEnd()) && (ts.pos() < CHUNK_SIZE) )
            {
            str = ts.readLine();
            if(str.length())         
                {
                str = str + "\n";
                te_Code->appendNewPlainText(str);
                }
            }
             
        filePos = ts.pos();
        if(file.size() > MAX_SIZE)
            bBigFile = true;

        if(fileSize >= filePos)
             bMoreBig = true;
        else
             bMoreBig = false;   
                
        file.close();  
              
        str = "QtMDI :- " +  filename;
        setWindowTitle(str);
        listRecentFiles.append(openFile = filename);

        te_Code->highlightLine(1);
       
        openFile = filename;
        return 0;
        }
    else
        {
        str = "Error loading file " + filename ;
        statusbar->showMessage(str, 1000);
        return -1;
        }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// called when further chunks of a large file are added to the text edit widget

int qtMDI::reOpenInBrowser()
{
QString str, str2;
QFile file(openFile);

    if (file.open(QFile::ReadOnly | QFile::Text))
        {
        if(file.size() != fileSize)
            {
            qDebug() << "Error - file size has changed since last load - " << openFile;
            return -1;
            }
        QTextStream ts(&file);
        ts.seek(filePos);
        
        while( (!ts.atEnd()) && (ts.pos() < (filePos + ADD_SIZE) ) )
            {
            str = ts.readLine();
            if(str.length())         
                {
                str = str + "\n";
                te_Code->appendNewPlainText(str);
                }
            }
            
        filePos = ts.pos();
        if(fileSize > filePos)
            bMoreBig = true;
        else
            bMoreBig = false;  
                 
        file.close();  
        return 0;
        }
    else
        {
        str = "Error reOpening file " + openFile;
        statusbar->showMessage(str, 10000);
        return -1;
        }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void qtMDI::onLoadLast()
{
    if(lastFile.length())
        openInBrowser(lastFile);

}


void qtMDI::onRecentFiles()
{
    listRecentFiles.removeDuplicates();
    RecentFilesDialog *dlg = new RecentFilesDialog(this, listRecentFiles);

    dlg->exec();
    
    if(dlg->result())
        openInBrowser(dlg->selection);
        
    listRecentFiles.removeDuplicates();
}

