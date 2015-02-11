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

#ifndef _NML_ACCESS_SLIM_H_
#define _NML_ACCESS_SLIM_H_

#include "LCNC_Headers.h"

#define MDI_MAX 64



#define CLOSE(a,b,eps) ((a)-(b) < +(eps) && (a)-(b) > -(eps))
#define LINEAR_CLOSENESS 0.0001
#define ANGULAR_CLOSENESS 0.0001
#define INCH_PER_MM (1.0/25.4)
#define CM_PER_MM 0.1
#define GRAD_PER_DEG (100.0/90.0)
#define RAD_PER_DEG TO_RAD	


#define RETRY_TIME 10.0		// seconds to wait for subsystems to come up
#define RETRY_INTERVAL 1.0	// seconds between wait tries for a subsystem
#define EMC_COMMAND_DELAY   0.1	// how long to sleep between checks



class NML_Access_Slim: public QObject
{
    Q_OBJECT
public:

    NML_Access_Slim();

    // the NML channels to the EMC task
    RCS_CMD_CHANNEL *emcCommandBuffer;
    RCS_STAT_CHANNEL *emcStatusBuffer;
    EMC_STAT *emcStatus;
   // EMC_TASK_STAT *emcTaskStatus;  // use emcStatus->task.????
    
    // the NML channel for errors
    NML *emcErrorBuffer;

    int emcCommandSerialNumber;

    char *mdi_commands[MDI_MAX];
    int num_mdi_commands;
    int have_home_all;
    int num_axes;
    double maxFeedOverride;
    double maxMaxVelocity;
    double minSpindleOverride;
    double maxSpindleOverride;

    EMC_TASK_MODE_ENUM halui_old_mode;
    int halui_sent_mdi;
    
    // default value for timeout, 0 means wait forever
    // use same timeout value as in tkemc & mini
    double receiveTimeout;
    double doneTimeout;
    

    
///////////////  Private NML functions   //////////////////////////////////////////////

    // programStartLine is the saved valued of the line that
    // sendProgramRun(int line) sent
    int programStartLine;
    int programStartLineLast;
    int tryNml();

    int emcTaskNmlGet();
    int emcErrorNmlGet();
    int updateStatus();
    int emcCommandWaitReceived(int );
    int emcCommandWaitDone(int );
    int iniLoad(const char*);
    void cleanUp();
    
    int sendMdi();
    int sendMdiCmd(char*);
    
    int sendManual();
    int sendAbort();
        
    // string for ini file version num
    QString version_string;

    // interpreter parameter file name, from ini file
    QString PARAMETER_FILE;

    // help file name, from ini file
    QString HELP_FILE;

    // the program path prefix
    QString programPrefix;

    // the program name currently displayed
    QString programFile;

    // the program last loaded by the controller
    QString lastProgramFile;

    QString active_g_codes;
    QString active_m_codes;
    QString active_settings;
    
    QString coordinates;
    int coords;
    int posDisplay;

    int axisJogging;

    // current jog speed setting
    int jogSpeed;
    int maxJogSpeed;
    int jogSpeedChange;

    // current jog increment setting, non-positive means continuous
    double jogIncrement;

    // the size of the smallest increment to jog, = 1/INPUT_SCALE
    double stepIncrement;

    // polarities for axis jogging, from ini file
    int jogPol[3];

    int oldFeedOverride;
    int feedOverride;   
    int feedOverrideChange;
    // timer delays until dec/inc appears
    int feedOverrideDelayCount;
    
};

#endif
