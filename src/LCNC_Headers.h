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


#ifndef _LCNC_HEADERS_H_
#define _LCNC_HEADERS_H_

#define _CORRECT_ISO_CPP_STRING_H_PROTO
// Qt
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QDebug>

// Std C / C++
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>   /* Standard types */
#include <signal.h>
#include <math.h>
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <ctype.h>
#include <values.h>             // DBL_MAX, maybe
#include <limits.h>             // DBL_MAX, maybe
#include <stdarg.h>
#include <sys/stat.h>           // struct stat, stat()
#include <unistd.h>

#include <fnmatch.h>

// /usr/include/emc2
#include <rtapi.h>
#include <hal.h>		
#include <hal_priv.h>	// need to get this from source and add it
#include <rcs.hh>
#include <posemath.h>		// PM_POSE, TO_RAD
#include <emc.hh>		// EMC NML
#include <emc_nml.hh>
#include <emcglb.h>		// EMC_NMLFILE, TRAJ_MAX_VELOCITY, etc.
#include <emccfg.h>		// DEFAULT_TRAJ_MAX_VELOCITY
#include <inifile.hh>		// INIFILE
#include <rcs_print.hh>
#include <nml_oi.hh>
#include <timer.hh>

// Can't rely on bool working otherwise
#undef TRUE
#define TRUE (1)
#undef FALSE
#define FALSE (0)
#undef true
#define true (1)
#undef false
#define false (0)

#endif

