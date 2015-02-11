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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// pins must be created before hal_ready()


int HAL_Access_Slim::createBitPin(QString& name, bool in)
{
int retval;

    QStrncpy(lname, name, name.length());

    if(bit_index >= ARRAY_SIZE)   
        {
        rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: pin %s creation not possible, local array full\n", lname);
        return -1;
        }
    else
        {
        bit_data_t *data = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    
        if (data == 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR, "ERROR: hal_malloc() for pin creation %s failed\n", lname);
        	cleanUp();
        	}
        else
            {
            bit_data[bit_index] = data;
            strcpy((char*)bit_name[bit_index++], lname);
            }
        retval = hal_pin_bit_newf(in ? HAL_IN : HAL_OUT, &(data->pin), comp_id, "%s.%s", prefix,  lname );
        if (retval < 0) 
            {
    	    rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: pin %s export failed with err=%i\n", lname, retval);
    	    cleanUp();
	        return -1;
            }
        return 0;
        }

}



int HAL_Access_Slim::createS32Pin(QString& name, bool in )
{
int retval;

    QStrncpy(lname, name, name.length());

    if(s32_index >= ARRAY_SIZE)   
        {
        rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: pin %s creation not possible, local array full\n", lname);
        return -1;
        }
    else
        {
        s32_data_t *data = (s32_data_t*)hal_malloc(sizeof(s32_data_t));

        if (data == 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR, "ERROR: hal_malloc() for pin %s creation failed\n", lname);
        	cleanUp();
        	}
        else
            {
            s32_data[s32_index] = data;
            strcpy((char*)s32_name[s32_index++], lname);
            }
            
        retval = hal_pin_s32_newf(in ? HAL_IN : HAL_OUT, &(data->pin), comp_id, "%s.%s", prefix, lname );
        if (retval < 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: pin %s export failed with err=%i\n", lname, retval);
	        cleanUp();
	        return -1;
            }
        return 0;
        }
}




int HAL_Access_Slim::createFloatPin(QString& name, bool in)
{
int retval;

    QStrncpy(lname, name, name.length());

    if(float_index >= ARRAY_SIZE)   
        {
        rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: pin %s creation not possible, local array full\n", lname);
        return -1;
        }
    else
        {
        float_data_t *data = (float_data_t*)hal_malloc(sizeof(float_data_t));

        if (data == 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR, "ERROR: hal_malloc() for pin %s creation failed\n", lname);
        	cleanUp();
        	}
        else
            {
            float_data[float_index] = data;
            strcpy((char*)float_name[float_index++], lname);
            }
        	
        retval = hal_pin_float_newf(in ? HAL_IN : HAL_OUT, &(data->pin), comp_id, "%s.%s", prefix,  lname);
        
        if (retval < 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: pin %s export failed with err=%i\n", lname, retval);
    	    cleanUp();
    	    return -1;
            }
        return 0;
        }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO create params

// r = hal_param_float_newf(HAL_RW, &(inst->testp), comp_id,"%s.testp", prefix);

int HAL_Access_Slim::createBitParam(QString& name, bool rw)
{
int retval;

    QStrncpy(lname, name, name.length());

    if(bit_param_index >= ARRAY_SIZE)   
        {
        rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: param %s creation not possible, local array full\n", lname);
        return -1;
        }
    else
        {
        hal_bit_t *data = (hal_bit_t*)hal_malloc(sizeof(hal_bit_t));

        if (data == 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR, "ERROR: hal_malloc() for param %s creation failed\n", lname);
        	cleanUp();
        	}
        else
            {
            bit_param_data[bit_param_index] = data;
            strcpy((char*)bit_param_name[bit_param_index++], lname);
            }
        	
        retval = hal_param_bit_newf(rw ? HAL_RW : HAL_RO, (data), comp_id, "%s.%s", prefix,  lname);
        
        if (retval < 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: param %s export failed with err=%i\n", lname, retval);
    	    cleanUp();
    	    return -1;
            }
        return 0;
        }
}


int HAL_Access_Slim::createS32Param(QString& name, bool rw)
{
int retval;

    QStrncpy(lname, name, name.length());

    if(s32_param_index >= ARRAY_SIZE)   
        {
        rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: param %s creation not possible, local array full\n", lname);
        return -1;
        }
    else
        {
        hal_s32_t *data = (hal_s32_t*)hal_malloc(sizeof(hal_s32_t));

        if (data == 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR, "ERROR: hal_malloc() for param %s creation failed\n", lname);
        	cleanUp();
        	}
        else
            {
            s32_param_data[s32_param_index] = data;
            strcpy((char*)s32_param_name[s32_param_index++], lname);
            }
        	
        retval = hal_param_s32_newf(rw ? HAL_RW : HAL_RO, (data), comp_id, "%s.%s", prefix,  lname);
        
        if (retval < 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: param %s export failed with err=%i\n", lname, retval);
    	    cleanUp();
    	    return -1;
            }
        return 0;
        }
}




int HAL_Access_Slim::createFloatParam(QString& name, bool rw)
{
int retval;

    QStrncpy(lname, name, name.length());

    if(float_param_index >= ARRAY_SIZE)   
        {
        rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: param %s creation not possible, local array full\n", lname);
        return -1;
        }
    else
        {
        hal_float_t *data = (hal_float_t*)hal_malloc(sizeof(hal_float_t));

        if (data == 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR, "ERROR: hal_malloc() for param %s creation failed\n", lname);
        	cleanUp();
        	}
        else
            {
            float_param_data[float_param_index] = data;
            strcpy((char*)float_param_name[float_param_index++], lname);
            }
        	
        retval = hal_param_float_newf(rw ? HAL_RW : HAL_RO, (data), comp_id, "%s.%s", prefix,  lname);
        
        if (retval < 0) 
            {
        	rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: param %s export failed with err=%i\n", lname, retval);
    	    cleanUp();
    	    return -1;
            }
        return 0;
        }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool HAL_Access_Slim::getBitPinValue(QString& name) // by name
{

    QStrncpy(lname, name, name.length());
    
    for(int y = 0; y < bit_index; y++)
        {
        if(strcmp((char*)bit_name[y], lname) == 0)
            return getBitPinValue(y);
        }
    return -1;  // still here?  wasn't found
}

bool HAL_Access_Slim::getBitPinValue(int index) // overloaded, by index
{
    return *(bit_data[index])->pin;
}

void HAL_Access_Slim::setBitPinValue(QString& name, bool n) // by name
{
    QStrncpy(lname, name, name.length());

    for(int y = 0; y < bit_index; y++)
        {
        if(strcmp((char*)bit_name[y], lname) == 0)
            {
            setBitPinValue(y, n);
            return;
            }
        }
}

void HAL_Access_Slim::setBitPinValue(int index, bool n) // overloaded, by index
{
    *(bit_data[index])->pin = n;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

signed int HAL_Access_Slim::getS32PinValue(QString& name)
{

    QStrncpy(lname, name, name.length());
    
    for(int y = 0; y < s32_index; y++)
        {
        if(strcmp((char*)s32_name[y], lname) == 0)
            return getS32PinValue(y);
        }
    return -1;  // still here?  wasn't found

}


signed int HAL_Access_Slim::getS32PinValue(int index)
{
    return *(s32_data[index])->pin;
}


void HAL_Access_Slim::setS32PinValue(QString& name, signed int n)
{
    QStrncpy(lname, name, name.length());

    for(int y = 0; y < s32_index; y++)
        {
        if(strcmp((char*)s32_name[y], lname) == 0)
            {
            setS32PinValue(y, n);
            return;
            }
        }
}


void HAL_Access_Slim::setS32PinValue(int index, signed int n)
{
   *(s32_data[index])->pin = n;
}



//////////////////////////////////////////////////////////////////////////////////////

float HAL_Access_Slim::getFloatPinValue(QString& name)
{
    QStrncpy(lname, name, name.length());
        
    for(int y = 0; y < float_index; y++)
        {
        if(strcmp((char*)float_name[y], lname) == 0)
            return getFloatPinValue(y);
        }
    return -1;  // still here?  wasn't found

}


float HAL_Access_Slim::getFloatPinValue(int index)
{
    return *(float_data[index])->pin;
}


void HAL_Access_Slim::setFloatPinValue(QString& name, float n)
{
    QStrncpy(lname, name, name.length());
    
    for(int y = 0; y < float_index; y++)
        {
        if(strcmp((char*)float_name[y], lname) == 0)
            {
            setFloatPinValue(y, n);
            return;
            }
        }
}


void HAL_Access_Slim::setFloatPinValue(int index, float n)
{
   *(float_data[index])->pin = n;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool HAL_Access_Slim::getBitParamValue(QString& name) // by name
{
    QStrncpy(lname, name, name.length());
    
    for(int y = 0; y < bit_param_index; y++)
        {
        if(strcmp((char*)bit_param_name[y], lname) == 0)
            return getBitParamValue(y);
        }
    return -1;  // still here?  wasn't found
}

bool HAL_Access_Slim::getBitParamValue(int index) // overloaded, by index
{
    return *(bit_param_data[index]);
}

void HAL_Access_Slim::setBitParamValue(QString& name, bool n) // by name
{
    QStrncpy(lname, name, name.length());
    
    for(int y = 0; y < bit_param_index; y++)
        {
        if(strcmp((char*)bit_param_name[y], lname) == 0)
            {
            setBitParamValue(y, n);
            return;
            }
        }
}

void HAL_Access_Slim::setBitParamValue(int index, bool n) // overloaded, by index
{
    *(bit_param_data[index]) = n;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

signed int HAL_Access_Slim::getS32ParamValue(QString& name)
{
    QStrncpy(lname, name, name.length());
    
    for(int y = 0; y < s32_index; y++)
        {
        if(strcmp((char*)s32_param_name[y], lname) == 0)
            return getS32ParamValue(y);
        }
    return -1;  // still here?  wasn't found

}


signed int HAL_Access_Slim::getS32ParamValue(int index)
{
    return *(s32_param_data[index]);
}


void HAL_Access_Slim::setS32ParamValue(QString& name, signed int n)
{
    QStrncpy(lname, name, name.length());
    
    for(int y = 0; y < s32_param_index; y++)
        {
        if(strcmp((char*)s32_param_name[y], lname) == 0)
            {
            setS32ParamValue(y, n);
            return;
            }
        }
}


void HAL_Access_Slim::setS32ParamValue(int index, signed int n)
{
   *(s32_param_data[index]) = n;
}



//////////////////////////////////////////////////////////////////////////////////////

float HAL_Access_Slim::getFloatParamValue(QString& name)
{
    QStrncpy(lname, name, name.length());
    
    for(int y = 0; y < float_param_index; y++)
        {
        if(strcmp((char*)float_param_name[y], lname) == 0)
            return getFloatParamValue(y);
        }
    return -1;  // still here?  wasn't found

}


float HAL_Access_Slim::getFloatParamValue(int index)
{
    return *(float_param_data[index]);
}


void HAL_Access_Slim::setFloatParamValue(QString& name, float n)
{
    QStrncpy(lname, name, name.length());
    
    for(int y = 0; y < float_param_index; y++)
        {
        if(strcmp((char*)float_param_name[y], lname) == 0)
            {
            setFloatParamValue(y, n);
            return;
            }
        }
}


void HAL_Access_Slim::setFloatParamValue(int index, float n)
{
   *(float_param_data[index]) = n;
}




//////////////////////////////////////////////////////////////////////////////////////


void HAL_Access_Slim::zeroAllBitPin()
{
int x;
        // initialise pins to zero
    for(x = 0; x < bit_index; x++)
        *(bit_data[x])->pin = false;
}


void HAL_Access_Slim::zeroAllS32Pin()
{
int x;
        // initialise pins to zero
    for(x = 0; x < s32_index; x++)
        *(s32_data[x])->pin = 0L;
}


void HAL_Access_Slim::zeroAllFloatPin()
{
int x;
    for(x = 0; x < float_index; x++)
        *(float_data[x])->pin = 0.0;    
}

////////////////////////////////////////////////////////

void HAL_Access_Slim::zeroAllBitParam()
{
int x;
        // initialise pins to zero
    for(x = 0; x < bit_index; x++)
        *(bit_param_data[x]) = false;
}


void HAL_Access_Slim::zeroAllS32Param()
{
int x;
        // initialise pins to zero
    for(x = 0; x < s32_index; x++)
        *(s32_param_data[x]) = 0L;
}


void HAL_Access_Slim::zeroAllFloatParam()
{
int x;
    for(x = 0; x < float_index; x++)
        *(float_param_data[x]) = 0.0;    
}


