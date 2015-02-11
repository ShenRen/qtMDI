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

#include "NML_Access_Slim.h"

static enum {
    LINEAR_UNITS_CUSTOM = 1,
    LINEAR_UNITS_AUTO,
    LINEAR_UNITS_MM,
    LINEAR_UNITS_INCH,
    LINEAR_UNITS_CM
} linearUnitConversion = LINEAR_UNITS_AUTO;

static enum {
    ANGULAR_UNITS_CUSTOM = 1,
    ANGULAR_UNITS_AUTO,
    ANGULAR_UNITS_DEG,
    ANGULAR_UNITS_RAD,
    ANGULAR_UNITS_GRAD
} angularUnitConversion = ANGULAR_UNITS_AUTO;


#define COORD_RELATIVE  1
#define COORD_MACHINE 2
#define POS_DISPLAY_ACT 1
#define POS_DISPLAY_CMD 2

// Establishes initial contact 


NML_Access_Slim::NML_Access_Slim()
{
emcCommandBuffer = 0;
emcStatusBuffer = 0;
emcStatus = 0;

emcErrorBuffer = 0;
emcCommandSerialNumber = 100000;

num_mdi_commands=0;
have_home_all = 0;
num_axes = 3;
maxFeedOverride=1;
maxMaxVelocity=1;
minSpindleOverride=0.0;
maxSpindleOverride=1.0;

halui_old_mode = EMC_TASK_MODE_MANUAL;
halui_sent_mdi = 0;

receiveTimeout = 1.;
doneTimeout = 60.;

programStartLine = 0;
programStartLineLast = 0;

version_string.clear();

PARAMETER_FILE.clear();

HELP_FILE.clear();

programPrefix.clear();

programFile.clear();

lastProgramFile.clear();

maxFeedOverride = 100;

active_g_codes.clear();
active_m_codes.clear();

axisJogging = -1;

jogSpeed = 1;
maxJogSpeed = 1;
jogSpeedChange = 0;

jogIncrement = 0.0;

stepIncrement = 0.0001;

oldFeedOverride = -1;
feedOverride = 0;
feedOverrideChange = 0;
feedOverrideDelayCount = 1;

coords = COORD_RELATIVE;
posDisplay = POS_DISPLAY_ACT;

}



int NML_Access_Slim::emcTaskNmlGet()
{
int retval = 0;

    // try to connect to EMC cmd
    if (emcCommandBuffer == 0) 
        {
	    emcCommandBuffer = new RCS_CMD_CHANNEL(emcFormat, "emcCommand", "xemc",emc_nmlfile);
    	if (!emcCommandBuffer->valid()) 
	        {
	        delete emcCommandBuffer;
	        emcCommandBuffer = 0;
	        retval = -1;
	        }
        }
    // try to connect to EMC status
    if (emcStatusBuffer == 0) 
        {
    	emcStatusBuffer = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "xemc", emc_nmlfile);
    	if (!emcStatusBuffer->valid()) 
    	    {
    	    delete emcStatusBuffer;
	        emcStatusBuffer = 0;
	        emcStatus = 0;
	        retval = -1;
	        } 
	    else 
    	    emcStatus = (EMC_STAT *) emcStatusBuffer->get_address();
	    }

    return retval;
}

//////////////////////////////////////////////////////////////////

int NML_Access_Slim::emcErrorNmlGet()
{
int retval = 0;

    if (emcErrorBuffer == 0) 
        {
	    emcErrorBuffer = new NML(nmlErrorFormat, "emcError", "xemc", emc_nmlfile);
    	if (!emcErrorBuffer->valid()) 
    	    {
    	    delete emcErrorBuffer;
	        emcErrorBuffer = 0;
	        retval = -1;
	        }
        }

    return retval;
}

////////////////////////////////////////////////////////////////////

int NML_Access_Slim::tryNml()
{
double end;
int good;

    if ((emc_debug & EMC_DEBUG_NML) == 0) 
      	set_rcs_print_destination(RCS_PRINT_TO_NULL);	// inhibit diag
    end = RETRY_TIME;
    good = 0;
    
    do 
        {
    	if (0 == emcTaskNmlGet()) 
    	    {
	        good = 1;
	        break;
	        }
    	esleep(RETRY_INTERVAL);
	    end -= RETRY_INTERVAL;
        } while (end > 0.0);
        
    if ((emc_debug & EMC_DEBUG_NML) == 0) 
      	set_rcs_print_destination(RCS_PRINT_TO_STDOUT);	// inhibit diag

    if (!good) 
       	return -1;
    
    if ((emc_debug & EMC_DEBUG_NML) == 0)
    	set_rcs_print_destination(RCS_PRINT_TO_NULL);

    end = RETRY_TIME;
    good = 0;

    do {
    	if (0 == emcErrorNmlGet()) 
    	    {
    	    good = 1;
	        break;
	        }
	    esleep(RETRY_INTERVAL);
	    end -= RETRY_INTERVAL;
        } while (end > 0.0);
    
    if ((emc_debug & EMC_DEBUG_NML) == 0) 
	    set_rcs_print_destination(RCS_PRINT_TO_STDOUT);	

    if (!good) 
    	return -1;
    else    
        return 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////

int NML_Access_Slim::updateStatus()
{
NMLTYPE type;

    if (0 == emcStatus || 0 == emcStatusBuffer || !emcStatusBuffer->valid()) 
    	return -1;
    
    switch (type = emcStatusBuffer->peek()) 
        {
        case 0:			// no new data
        case EMC_STAT_TYPE:	// new data
	            break;
        case -1: 	// error on CMS channel
        default:
            	return -1;
            	break;
        }
    return 0;
}




/*
  emcCommandWaitReceived() waits until the EMC reports that it got
  the command with the indicated serial_number.
  emcCommandWaitDone() waits until the EMC reports that it got the
  command with the indicated serial_number, and it's done, or error.
*/

int NML_Access_Slim::emcCommandWaitReceived(int serial_number)
{
double end = 0.0;

    while (end < receiveTimeout) 
        {
    	updateStatus();

	    if (emcStatus->echo_serial_number == serial_number)
    	    return 0;
		esleep(EMC_COMMAND_DELAY);
	    end += EMC_COMMAND_DELAY;
        }

    return -1;
}

int NML_Access_Slim::emcCommandWaitDone(int serial_number)
{
double end = 0.0;

    // first get it there
    if (0 != emcCommandWaitReceived(serial_number)) 
    	return -1;
    
    // now wait until it, or subsequent command (e.g., abort) is done
    while (end < doneTimeout) 
        {
    	updateStatus();

	    if (emcStatus->status == RCS_DONE) 
    	    return 0;
		if (emcStatus->status == RCS_ERROR) 
    	    return -1;
	
	    esleep(EMC_COMMAND_DELAY);
	    end += EMC_COMMAND_DELAY;
        }
    return -1;
}



///////////////////////////////////////////////////////////////////////////////////////////////////


int NML_Access_Slim::iniLoad(const char *filename)
{
IniFile inifile;
const char *inistring;
char machine[80];
char version[80];
char displayString[80] = "";
QString str;
double d;
int i;
const char *mc;

    if (inifile.Open(filename) == false) 
    	return -1;
    
    if ((inistring = inifile.Find("DEBUG", "EMC")) != NULL) 
        {
    	// copy to global
	    if (1 != sscanf(inistring, "%i", &emc_debug)) 
    	    emc_debug = 0;	    
        } 
    else 
    	emc_debug = 0;
    	
    	
    if ((inistring = inifile.Find("MACHINE", "EMC")) != NULL) 
        {
        strcpy(machine, inistring);
        if (NULL != (inistring = inifile.Find("VERSION", "EMC"))) 
            {
            sscanf(inistring, "$Revision: %s", version);
            version_string.sprintf( "%s EMC Version %s", machine, version);
            }
        }

    if ((inistring = inifile.Find("NML_FILE", "EMC"))!= NULL) 
    	strcpy(emc_nmlfile, inistring);
    	// if not found, use default
    	
    if ((inistring = inifile.Find("TOOL_TABLE", "EMCIO")) != NULL) 
        strcpy(tool_table_file, inistring);
     else 
        strcpy(tool_table_file, "tool.tbl"); // FIXME-- hardcoded

    if ((inistring = inifile.Find("PARAMETER_FILE", "RS274NGC"))!= NULL)
        PARAMETER_FILE = inistring;
    else 
        PARAMETER_FILE = "rs274ngc.var"; // FIXME-- hardcoded

 
    if ((inistring = inifile.Find("DEFAULT_VELOCITY", "TRAJ"))!= NULL) 
        {
        if (1 != sscanf(inistring, "%lf", &traj_default_velocity)) 
            traj_default_velocity = DEFAULT_TRAJ_DEFAULT_VELOCITY;
        }
    else 
        traj_default_velocity = DEFAULT_TRAJ_DEFAULT_VELOCITY;
        // round jogSpeed in display to integer, per-minute
    jogSpeed = (int) (traj_default_velocity * 60.0 + 0.5);        

    
    if ((inistring = inifile.Find("MAX_VELOCITY", "TRAJ"))!= NULL) 
        {
        if (1 != sscanf(inistring, "%lf", &traj_max_velocity))
            traj_max_velocity = DEFAULT_TRAJ_MAX_VELOCITY;
        }
    else
        traj_max_velocity = DEFAULT_TRAJ_MAX_VELOCITY;
        // round maxJogSpeed in display to integer, per-minute
    maxJogSpeed = (int) (traj_max_velocity * 60.0 + 0.5);


    if ((inistring = inifile.Find("HELP_FILE", "DISPLAY"))!= NULL) 
        HELP_FILE = inistring;
 
    if ((inistring = inifile.Find("PROGRAM_PREFIX", "DISPLAY"))!= NULL) 
        programPrefix = inistring;
    else if ((inistring = inifile.Find("PROGRAM_PREFIX", "TASK"))!= NULL) 
        programPrefix = inistring;      
    else
        programPrefix.clear();
        
        
    if ((inistring = inifile.Find("POSITION_FEEDBACK", "DISPLAY"))!= NULL) 
        {
        str = inistring;
        if(str.contains("ACTUAL", Qt::CaseInsensitive))
            posDisplay = POS_DISPLAY_ACT;
        else if(str.contains("COMMANDED", Qt::CaseInsensitive))
            posDisplay = POS_DISPLAY_CMD;
        }
   
   
    if ((inistring = inifile.Find("POSITION_OFFSET", "DISPLAY"))!= NULL) 
        {
        str = inistring;
        if(str.contains("MACHINE", Qt::CaseInsensitive))
            coords = POS_DISPLAY_ACT;
        else if(str.contains("RELATIVE", Qt::CaseInsensitive))
            coords = POS_DISPLAY_CMD;
        }
    
        
        
    if ( (inistring = inifile.Find("MAX_FEED_OVERRIDE", "DISPLAY"))!= NULL) 
        {
    	if (1 == sscanf(inistring, "%lf", &d) && d > 0.0) 
    	    maxFeedOverride =  d;
	    }

    if( (inistring = inifile.Find("MAX_VELOCITY", "TRAJ"))!= NULL)
        {
    	if (1 == sscanf(inistring, "%lf", &d) && d > 0.0) 
    	    maxMaxVelocity =  d;
	    }
    else  if((inistring = inifile.Find("MAX_VELOCITY", "AXIS_0")) != NULL) 
        {
        if (1 == sscanf(inistring, "%lf", &d) && d > 0.0) 
    	    maxMaxVelocity =  d; 
        }
    else
        maxMaxVelocity = 1.0;

    if ( (inistring = inifile.Find("MIN_SPINDLE_OVERRIDE", "DISPLAY"))!= NULL) 
        {
    	if (1 == sscanf(inistring, "%lf", &d) && d > 0.0) 
    	    minSpindleOverride =  d;
	    }

    if ( (inistring = inifile.Find("MAX_SPINDLE_OVERRIDE", "DISPLAY"))!= NULL) 
        {
    	if (1 == sscanf(inistring, "%lf", &d) && d > 0.0) 
    	    maxSpindleOverride =  d;
	    }
    
    if ( (inistring = inifile.Find("AXES", "TRAJ"))!= NULL) 
        {
    	if (1 == sscanf(inistring, "%d", &i) && i > 0) 
	        num_axes =  i;
	    }

    if ( (inistring = inifile.Find("COORDINATES", "TRAJ"))!= NULL) 
        {
    	str = inistring;
       	coordinates = str.simplified();
	    }


    for (int t = 0; t < num_axes; t++) 
        {
        jogPol[t] = 1;              // set to default
        sprintf(displayString, "AXIS_%d", t);
        if (NULL != (inistring = inifile.Find("JOGGING_POLARITY", displayString)) && 1 == sscanf(inistring, "%d", &i) && i == 0) 
            {
            // it read as 0, so override default
            jogPol[t] = 0;
            }
        }
           
    if ( inifile.Find("HOME_SEQUENCE", "AXIS_0")!= NULL) 
        have_home_all = 1;
    

    if ( (inistring = inifile.Find("LINEAR_UNITS", "DISPLAY"))!= NULL) 
        {
    	if (!strcmp(inistring, "AUTO")) 
	        linearUnitConversion = LINEAR_UNITS_AUTO;
    	else if (!strcmp(inistring, "INCH")) 
    	    linearUnitConversion = LINEAR_UNITS_INCH;
	    else if (!strcmp(inistring, "MM")) 
    	    linearUnitConversion = LINEAR_UNITS_MM;
	    else if (!strcmp(inistring, "CM")) 
	        linearUnitConversion = LINEAR_UNITS_CM;
	    }

    if ( (inistring = inifile.Find("ANGULAR_UNITS", "DISPLAY"))!= NULL) 
        {
    	if (!strcmp(inistring, "AUTO")) 
	        angularUnitConversion = ANGULAR_UNITS_AUTO;
	    else if (!strcmp(inistring, "DEG")) 
	        angularUnitConversion = ANGULAR_UNITS_DEG;
	    else if (!strcmp(inistring, "RAD")) 
	        angularUnitConversion = ANGULAR_UNITS_RAD;
	    else if (!strcmp(inistring, "GRAD")) 
	        angularUnitConversion = ANGULAR_UNITS_GRAD;
        }

    while(num_mdi_commands < MDI_MAX && (mc = inifile.Find("MDI_COMMAND", "HALUI", num_mdi_commands+1)))
        mdi_commands[num_mdi_commands++] = strdup(mc);
        
        
     // FIXME-- we're using the first axis scale to set the jog increment.
    // Note that stepIncrement is inited to a reasonable value above, and
    // will only be reset on a good ini file match
    
    if ((inistring = inifile.Find("INPUT_SCALE", "AXIS_0"))!= NULL) 
        {
        if (1 == sscanf(inistring, "%lf", &d)) 
            {
            if (d < 0.0) 
                stepIncrement = -1.0/d; // posify it
            else if (d > 0.0) 
                stepIncrement = 1.0/d;
            }
        }
     // set step increment to be less than 0.0010, the last fixed increment,
     // if it's larger. Set to 0.0001 if so, which will be too small but it
     // can't hurt.
    if (stepIncrement >= 0.0010) 
        stepIncrement = 0.0001;
     
    
    inifile.Close();

    return 0;
}


void NML_Access_Slim::cleanUp()
{
  
    if(emcCommandBuffer) { delete emcCommandBuffer;  emcCommandBuffer = 0; }
    if(emcStatusBuffer) { delete emcStatusBuffer;  emcStatusBuffer = 0; }
    if(emcErrorBuffer) { delete emcErrorBuffer;  emcErrorBuffer = 0; }

}


int NML_Access_Slim::sendMdi()
{
    EMC_TASK_SET_MODE mode_msg;

    mode_msg.mode = EMC_TASK_MODE_MDI;
    mode_msg.serial_number = ++emcCommandSerialNumber;
    emcCommandBuffer->write(mode_msg);
    return emcCommandWaitReceived(emcCommandSerialNumber);
}

int NML_Access_Slim::sendMdiCmd(char *mdi)
{
  EMC_TASK_PLAN_EXECUTE emc_task_plan_execute_msg;

  strcpy(emc_task_plan_execute_msg.command, mdi);
  emc_task_plan_execute_msg.serial_number = ++emcCommandSerialNumber;
  emcCommandBuffer->write(emc_task_plan_execute_msg);
    
  return emcCommandWaitReceived(emcCommandSerialNumber);

//  return 0;
}

int NML_Access_Slim::sendAbort()
{
    EMC_TASK_ABORT task_abort_msg;

    task_abort_msg.serial_number = ++emcCommandSerialNumber;
    emcCommandBuffer->write(task_abort_msg);
    return emcCommandWaitReceived(emcCommandSerialNumber);
}

int NML_Access_Slim::sendManual()
{
    EMC_TASK_SET_MODE mode_msg;

    mode_msg.mode = EMC_TASK_MODE_MANUAL;
    mode_msg.serial_number = ++emcCommandSerialNumber;
    emcCommandBuffer->write(mode_msg);
    return emcCommandWaitReceived(emcCommandSerialNumber);
}



