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

#include "HAL_Access_Slim.h"


HAL_Access_Slim::HAL_Access_Slim(int argc, char **argv, QString& compname, bool inifile)
{
bit_index = 0;
s32_index = 0;
float_index = 0;
bit_param_index = 0;
s32_param_index = 0;
float_param_index = 0;
QString str;

QStrncpy(prefix, compname, compname.length());
    
    if(inifile)  // we don't always want to call an inifile'
        {
        // process command line args
        if (0 != emcGetArgs(argc, argv)) 
            {
        	qDebug() << "error in commandline argument list";
            qDebug() << "argv[1] = " << argv[1];
            qDebug() << "argv[2] = " << argv[2];
            qDebug() << "argv[3] = " << argv[3];
            qDebug() << "argv[4] = " << argv[4];
            qDebug() << "argv[5] = " << argv[5]; 
            qDebug() << "argv[6] = " << argv[6];
            qDebug() << "argv[7] = " << argv[7];                                     
          	exit(1);
            }
        }
        
    _nml = new NML_Access_Slim();
    
    if(inifile)
        {
        if (0 != _nml->iniLoad(emc_inifile))
            { 
            qDebug() << "iniLoad error" << str;
            qDebug() << "argv[1] = " << argv[1];
            qDebug() << "argv[2] = " << argv[2];
            qDebug() << "argv[3] = " << argv[3];
            qDebug() << "argv[4] = " << argv[4];
            qDebug() << "argv[5] = " << argv[5];  // INI_FILE_NAME=
            qDebug() << "argv[6] = " << argv[6];
            qDebug() << "argv[7] = " << argv[7];                                     
            exit(1);
            }
        else
            printf("%s read OK\n",emc_inifile);
        }
            
  // init NML
    if (0 != _nml->tryNml()) 
        {
    	qDebug() << "can't connect to emc";
	    exit(1);
        }
	else
        qDebug() << "EMC contacted OK";   
        
        
    // This single call inserts the component into the running system
    comp_id = hal_init(prefix);

    if(comp_id < 0)
        {
        rtapi_print_msg(RTAPI_MSG_ERR,"Error hal_init failed with error code %d", comp_id);
        exit(1);
        }
    
}


////////////////////////////////////////////////////////////////////////////


// separate out so that pins can be created before finalising

void HAL_Access_Slim::engage(){ hal_ready(comp_id); }


// these allow removal of component from emc, altering it then re-insertion
// without triggering full cleanUp() and shutdown

void HAL_Access_Slim::disEngage() { hal_exit(comp_id); }

int HAL_Access_Slim::reEngage(QString& compname) 
{ 
    QStrncpy(prefix, compname, compname.length());
    
    return( hal_init(prefix) ); 
    
}  // comp_id = reEngage(prefix / comp name);


/////////////////////////////////////////////////////////////////////////////


char* HAL_Access_Slim::getIniFileName()
{
    return(emc_inifile);
}

int HAL_Access_Slim::getID()
{
    return comp_id;
}

void HAL_Access_Slim::cleanUp()
{
    // need to call NML_Access_Slim::cleanup() first
    _nml->cleanUp();
    hal_exit(comp_id);
    
}

////////////////////////////////////////////////////////////////
//
// helper to save problems translating QStrings to char*
// which NML and HAL functions require
//

void HAL_Access_Slim::QStrncpy(char* str, QString& qstr, int num)
{
int x;

    for(x = 0; x < num; x++)
        {
        str[x] = (char)qstr.at(x).toLatin1();
        }
    str[x] = 0;

}


