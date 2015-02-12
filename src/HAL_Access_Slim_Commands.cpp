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


///////////////////////////////////////////////////////////////////////////////////////////////////////////

int HAL_Access_Slim::setLockOn(QString& command)
{
int retval=0;

    if ((command.isEmpty()) || (command.contains("all")) ) 
        {
    	retval = hal_set_lock(HAL_LOCK_ALL);
        } 
    else if (command.contains("none")) 
        {
    	retval = hal_set_lock(HAL_LOCK_NONE);
        } 
    else if (command.contains("tune")) 
        {
    	retval = hal_set_lock(HAL_LOCK_LOAD & HAL_LOCK_CONFIG);
        } 
    return retval;  // 0 is success
}

int HAL_Access_Slim::setLockOff(QString& command)
{
int retval=0;

    if ((command.isEmpty()) || (command.contains("all")) ) 
        {
    	retval = hal_set_lock(HAL_LOCK_NONE);
        } 
    else if (command.contains("tune")) 
        {
    	retval = hal_set_lock(HAL_LOCK_LOAD & HAL_LOCK_CONFIG);
        }
    return retval;  // 0 is success
}




// types are HAL_BIT, HAL_FLOAT, HAL_U32, HAL_S32

int HAL_Access_Slim::createSignal(QString& name, hal_type_t type)  
{  
    QStrncpy(lname, name, name.length());
    return (hal_signal_new(lname, type)); 
}

int HAL_Access_Slim::linkPin2Signal(QString& pin, QString& sig) 
{ 
    QStrncpy(lname, pin, pin.length());
    QStrncpy(l2name, sig, sig.length());
    return( hal_link(lname, l2name)); 
}

int HAL_Access_Slim::unlinkPin2Signal(QString& name) 
{ 
    QStrncpy(lname, name, name.length());
    return(hal_unlink(lname)); 
}

int HAL_Access_Slim::deleteSignal(QString& name) 
{ 
    QStrncpy(lname, name, name.length());    
    return(hal_signal_delete(lname)); 
}

int HAL_Access_Slim::addFunc2Thread(QString& func, QString& thread) 
{ 
    QStrncpy(lname, func, func.length());
    QStrncpy(l2name, thread, thread.length());
    return(hal_add_funct_to_thread(lname, l2name, -1)); 
}

int HAL_Access_Slim::removeFunc2Thread(QString& func, QString& thread) 
{ 
    QStrncpy(lname, func, func.length());
    QStrncpy(l2name, thread, thread.length());
    return( hal_del_funct_from_thread(lname, l2name)); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int HAL_Access_Slim::setBitParam(QString& name, int value) 
{ 
    QStrncpy(lname, name, name.length());
    return(hal_param_bit_set(lname, value)); 
}

int HAL_Access_Slim::setFloatParam(QString& name, double value) 
{ 
    QStrncpy(lname, name, name.length());
    return(hal_param_float_set(lname, value)); 
}

int HAL_Access_Slim::setS32Param(QString& name, signed long value) 
{ 
    QStrncpy(lname, name, name.length());
    return(hal_param_s32_set(lname, value)); 
}

/////////////////////////  helpers ///////////////////////////////////////////////


int set_common(hal_type_t type, void *d_ptr, QString& value) 
{
// This function assumes that the mutex is held
int retval = 0;
double fval = 0;
long lval = 0;
unsigned long ulval = 0;

    switch (type) 
        {
        case HAL_BIT:
                    if( (value.contains("TRUE") || (value.contains("1"))))
	                    *(hal_bit_t *) (d_ptr) = 1;
	                else if( (value.contains("FALSE") || (value.contains("0"))))
	                    *(hal_bit_t *) (d_ptr) = 0;
                    else
                	    retval = -EINVAL;
	            	break;
        case HAL_FLOAT:
	                fval = value.toFloat();
	               	    *((hal_float_t *) (d_ptr)) = fval;
	            	break;
        case HAL_S32:
	                lval = value.toLong();
	                    *((hal_s32_t *) (d_ptr)) = lval;
	            	break;
        case HAL_U32:
	                ulval = value.toULong();
	                    *((hal_u32_t *) (d_ptr)) = ulval;
	            	break;
	    default:
	                /* Shouldn't get here, but just in case... */
	                retval = -EINVAL;
        }
    return retval;
}


int get_common(hal_type_t type, void *d_ptr, QString& value) 
{
// This function assumes that the mutex is held
int retval = 0;
int bitval = 0;
double fval = 0;
long lval = 0;
unsigned long ulval = 0;
QString str;

    switch (type) 
        {
        case HAL_BIT:
                    bitval = *(hal_bit_t *) (d_ptr);
                    str.sprintf("%d", bitval);
	            	break;
        case HAL_FLOAT:
                    fval = *((hal_float_t *) (d_ptr));
	                str.sprintf("%f", fval);
	            	break;
        case HAL_S32:
	                lval =  *((hal_s32_t *) (d_ptr));
                    str.sprintf("%ld", lval);
	            	break;
        case HAL_U32:
	                ulval = *((hal_u32_t *) (d_ptr));
	                str.sprintf("%lu", ulval);
	            	break;
	    default:
	                /* Shouldn't get here, but just in case... */
	                retval = -EINVAL;
        }
    
        //qDebug() <<  "str string = " << str;
    value = str;
        //qDebug() <<  "value string = " << value;   
    return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////


int HAL_Access_Slim::setPValue(QString& name, QString& value)
{
int retval = 0;
hal_param_t *param = 0;
hal_pin_t *pin = 0;
hal_type_t type;
void *d_ptr = 0;
QString str;

    QStrncpy(lname, name, name.length());
    
    // get mutex before accessing shared data 
    rtapi_mutex_get(&(hal_data->mutex));
    // search param list for name 
    param = halpr_find_param_by_name(lname);
    if (param == 0) 
        {
        pin = halpr_find_pin_by_name(lname);
        if(pin == 0) 
            {
            rtapi_mutex_give(&(hal_data->mutex));
            qDebug() << "pin " << lname << " not found";
            return -EINVAL;
            } 
        else // pin
            {
            type = pin->type;
            if(pin->dir == HAL_OUT) 
                {
                rtapi_mutex_give(&(hal_data->mutex));
                qDebug() << "pin " << lname << " read only";
                return -EINVAL;
                }
            if(pin->signal != 0) 
                {
                rtapi_mutex_give(&(hal_data->mutex));
                qDebug() << "pin " << lname << " connected to signal";
                return -EINVAL;
                }
            // d_ptr = (void*)SHMPTR(pin->dummysig);
            d_ptr = (void*)&pin->dummysig;
            }
        } 
    else  // param
        {
        type = param->type;
        if (param->dir == HAL_RO) 
            {
            rtapi_mutex_give(&(hal_data->mutex));
            qDebug() << "param " << lname << " read only";
            return -EINVAL;
            }
        d_ptr = SHMPTR(param->data_ptr);
    }

    retval = set_common(type, d_ptr, value);
    rtapi_mutex_give(&(hal_data->mutex));
    if (retval != 0) 
        qDebug() << "setPValue() " << lname << " failed";

    return retval;
}

/////////////////////////////////////////////////////////////////////


int HAL_Access_Slim::getPValue(QString& name, QString& value)
{
int retval;
hal_param_t *param;
hal_pin_t *pin;
hal_type_t type;
void *d_ptr;

    QStrncpy(lname, name, name.length());
    
    // get mutex before Accessing shared data 
    rtapi_mutex_get(&(hal_data->mutex));
    // search param list for name 
    param = halpr_find_param_by_name(lname);
    if (param == 0) 
        {
        pin = halpr_find_pin_by_name(lname);
        if(pin == 0) 
            {
            rtapi_mutex_give(&(hal_data->mutex));
            qDebug() << "pin " << lname << " not found";
            return -EINVAL;
            } 
        else // pin
            {
            type = pin->type;
             // d_ptr = (void*)SHMPTR(pin->dummysig);
            d_ptr = (void*)&pin->dummysig;
            }
        } 
    else  // param
        {
        type = param->type;
        d_ptr = SHMPTR(param->data_ptr);
        }

    retval = get_common(type, d_ptr, value);

    rtapi_mutex_give(&(hal_data->mutex));
    if (retval != 0) 
       qDebug() << "getPValue() " << lname << " failed";
    return retval;
}

/////////////////////////////////////////////////////////////////////


int HAL_Access_Slim::setSignalValue(QString& name, QString& value)
{
int retval;
hal_sig_t *sig;
hal_type_t type;
void *d_ptr;

    QStrncpy(lname, name, name.length());
    
    rtapi_mutex_get(&(hal_data->mutex));

    sig = halpr_find_sig_by_name(lname);
    if (sig == 0) 
        {
    	rtapi_mutex_give(&(hal_data->mutex));
	    qDebug() << "signal " << lname << "- not found";
	    return -EINVAL;
        }

    if (sig->writers > 0) 
        {
    	rtapi_mutex_give(&(hal_data->mutex));
   	    qDebug() << "signal " << lname << "- already has writer";
	    return -EINVAL;
        }
    
    type = sig->type;
    d_ptr = SHMPTR(sig->data_ptr);
    
    retval = set_common(type, d_ptr, value);
    
    rtapi_mutex_give(&(hal_data->mutex));
    
    if (retval != 0) 
        qDebug() << "signal " << lname << "- failed, setting to " << value;

    return retval;

}


int HAL_Access_Slim::getSignalValue(QString& name, QString& value)
{
int retval;
hal_sig_t *sig;
hal_type_t type;
void *d_ptr;

    QStrncpy(lname, name, name.length());
    
    rtapi_mutex_get(&(hal_data->mutex));

    sig = halpr_find_sig_by_name(lname);
    if (sig == 0) 
        {
    	rtapi_mutex_give(&(hal_data->mutex));
	    qDebug() << "signal " << lname << "- not found";
	    return -EINVAL;
        }

    if (sig->writers > 0) 
        {
    	rtapi_mutex_give(&(hal_data->mutex));
   	    qDebug() << "signal " << lname << "- already has writer";
	    return -EINVAL;
        }
    
    type = sig->type;
    d_ptr = SHMPTR(sig->data_ptr);
    
    retval = get_common(type, d_ptr, value);
    
    rtapi_mutex_give(&(hal_data->mutex));
    
    if (retval != 0) 
        qDebug() << "signal " << lname << "- failed to get value";

    return retval;

}


///////////////////////////////////////////////////////////////////

int HAL_Access_Slim::netSignalPin(QString& signal, QStringList& pins) 
{
hal_sig_t *sig;
hal_pin_t *pin;
int i, retval;
QString str;

    QStrncpy(lname, signal, signal.length());

    rtapi_mutex_get(&(hal_data->mutex));
    /* see if signal already exists */
    sig = halpr_find_sig_by_name(lname);

    // verify that everything matches up (pin types, etc) error string in str 
    retval = checkNet(signal, sig, pins);
    
    if(retval < 0) 
        {
        rtapi_mutex_give(&(hal_data->mutex));
        return retval;
        }

    pin = halpr_find_pin_by_name(lname);
	if(pin) 
	    {
	    qDebug() << "Signal name " << lname << " must not be the same as a pin. Did you omit the signal name?";
	    rtapi_mutex_give(&(hal_data->mutex));
	    return -ENOENT;
	    }
    
    if(!sig) 
        {
        /* Create the signal with the type of the first pin */
        hal_pin_t *pin = halpr_find_pin_by_name((pins[0]).toLatin1());
        rtapi_mutex_give(&(hal_data->mutex));
        if(!pin)
            {
            qDebug() << "pin not found  " << pins[0]; 
            return -ENOENT;
            }
        retval = hal_signal_new(lname, pin->type);
        } 
    else 
        {
    	/* signal already exists */
        rtapi_mutex_give(&(hal_data->mutex));
        }
    /* add pins to signal */
    for(i=0; retval == 0 && i < pins.size(); i++) 
        {
        retval = linkPin2Signal(pins[i], signal);
        }
    
    return retval;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// check sanity of requested linkage
int HAL_Access_Slim::checkNet(QString& signal, hal_sig_t *sig, QStringList& pins) 
{
int i, type=-1, writers=0, bidirs=0, pincnt=0;
char *writer_name=0, *bidir_name=0;

    /* if signal already exists, use its info */
    if (sig) 
        {
    	type = sig->type;
	    writers = sig->writers;
	    bidirs = sig->bidirs;
        }

    if(writers || bidirs) 
        {
        hal_pin_t *pin;
        int next;
        for(next = hal_data->pin_list_ptr; next; next=pin->next_ptr) 
            {
            pin = (hal_pin_t *)SHMPTR(next);
            if(SHMPTR(pin->signal) == sig && pin->dir == HAL_OUT)
                writer_name = pin->name;
            if(SHMPTR(pin->signal) == sig && pin->dir == HAL_IO)
                bidir_name = writer_name = pin->name;
            }
        }

    for(i=0; i < pins.size() ; i++) 
        {
        hal_pin_t *pin = 0;
        pin = halpr_find_pin_by_name((pins[i]).toLatin1());
        if(!pin) 
            {
            qDebug() << "Pin " << pins[i] <<  " does not exist";
            return -ENOENT;
            }
        if(SHMPTR(pin->signal) == sig) 
            {
	        /* Already on this signal */
    	    pincnt++;
	        continue;
	        } 
	    else if(pin->signal != 0) 
	        {
            hal_sig_t *osig = (hal_sig_t *)SHMPTR(pin->signal);
            qDebug() << "Pin was already linked to signal" <<  pin->name << osig->name;
            return -EINVAL;
        	}
	    if (type == -1) 
	        {
    	    /* no pre-existing type, use this pin's type */
	        type = pin->type;
	        }
        if(type != pin->type) 
            {
            qDebug() << "Signal " << signal  << "cannot add pin " << pin->name;
            return -EINVAL;
            }
        if(pin->dir == HAL_OUT) 
            {
            if(writers || bidirs) 
                {
dir_error:  // I didn't write this - it is shit programming!
                qDebug() << "Signal " << signal  << "cannot add pin " << pin->name << "it already has pin";
                return -EINVAL;
                }
            writer_name = pin->name;
            writers++;
            }
	    if(pin->dir == HAL_IO) 
	        {
            if(writers) 
                goto dir_error;
            bidir_name = pin->name;
            bidirs++;
            }
        pincnt++;
        }
    if(pincnt)
        return 0;
    else
        {
        qDebug() <<  "net requires at least one pin, none given";
        return -EINVAL;
        }
}



