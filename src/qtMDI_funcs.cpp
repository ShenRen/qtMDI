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

//////////////////////////////////////////////////////////////////////////////
// execute single MDI command
// used per line by runInBackground()

void qtMDI::onMDICommand(QString& str)
{
char lastCommand[80];
QString cstr;

    QStrncpy(lastCommand, str, str.length());
    if((lastCommand)[0] != 0)
       _hal->_nml->sendMdiCmd(lastCommand);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// only called directly from context menu in code editor
void qtMDI::onRunFromContext(int line /* = 1*/)
{
    bDirect = true;
    onRunFrom(line);
    
}

void qtMDI::onRunProgram()
{
    bDirect = false;
    onRunFrom(1);
}

// check if direct command from te_Code first to override the current line number

void qtMDI::onRunFrom(int line /* = 1*/)
{
    if(!openFile.length())
        {
        statusbar->showMessage("Error - no file open - cannot execute Run command", 1000);
        actionRunProgram->setChecked(bRun = false);
        return;
        }
    // if Run-From activated whilst Running and in Pause - change index
    if(bDirect)
        {
        if(bRun && bPause)
            {
            actionPauseProgram->setChecked(bPause = false);
            listIndex = line; 
            bDirect = false;
            return;    
            }
        bDirect = false;
        }
    // if run button toggled off - treat as Stop
    if( bRun && !actionRunProgram->isChecked() )
        {
        bRun = false;
        stopProgram();
        return;
        }
    // shouldn't go here - but just in case
    if(bRun) return;

    // if not running or paused- ie normal start
    if(!bRun && !bPause)
        {
        _hal->_nml->sendMdi(); 
        actionRunProgram->setChecked(bRun = true);
        }
       
    listIndex = line;  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void qtMDI::pauseProgram()
{
    if(!bPause && bRun) // if not already paused and running
        actionPauseProgram->setChecked(bPause = true);
    else if(bPause && bRun)
        actionPauseProgram->setChecked(bPause = false);
    else  // the button itself is checkable even if the action does not fit into above 2 
          // so stop it checking unless desired
        actionPauseProgram->setChecked(bPause = false);
}

void qtMDI::stepProgram()
{
    if(bPause && bRun)  // can only step from paused program
        actionStep->setChecked(bStep = true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//  Even though we are using MDI commands, this will still abort everything

void qtMDI::stopProgram()
{
    bRun = false;
    actionPauseProgram->setChecked(bPause = false);
    actionRunProgram->setChecked(bRun = false);
    _hal->_nml->sendAbort();
    _hal->_nml->sendManual();
    
    // highlight first line to show stopped
    te_Code->highlightLine(1); 
}



/////////////////////////////////////////////////////////////////////////////////////////////////

void qtMDI::onAboutQtMDI()
{
    QMessageBox::about (this, "About qtMDI", 
                        "               qtMDI\n"
                        "      Minimal MDI MachineKitCNC GUI\n"
                        "       written entirely in Qt5 C++\n"
                        "          (c) ArcEye 2012 - 2015\n\n");
}

void qtMDI::onAboutQt()
{
    QMessageBox::aboutQt(this, "QtMDI - About Qt");
}

