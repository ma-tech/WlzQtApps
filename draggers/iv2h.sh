#!/bin/sh
############################################################################
# iv2h.sh
#
# This script generates a C header file from an Inventor file.  The header
# just contains the inventor file as a static char buffer.
#
# Authors:
#   Morten Eriksen <mortene@sim.no>
#   Lars J. Aas <larsa@sim.no>
#   Zsolt.Husz <zsolt.husz@hgu.mrc.ac.uk>
#

UPCASEBASE=`basename $1 .iv | tr '[a-z]' '[A-Z]'`
BASE=`basename $1`
DATE=`date`

cat <<HERE_DOC_END
/*!
* \file         $2Geom.h
* \author       Zsolt Husz
* \date         March 2009
* \version      \$Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2008 Medical research Council, UK.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be
* useful but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public
* License along with this program; if not, write to the Free
* Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA  02110-1301, USA.
* \brief        $1 geometry 
* \ingroup      Control
*
* Automatically generated on $DATE with 
*    iv2h.sh $1 $2
* 
*/

#ifndef ${UPCASEBASE}_IV_H
#define ${UPCASEBASE}_IV_H

 const char $2::geomBuffer[] =
HERE_DOC_END

cat $1 | sed -e \
's/\\/\\\\/g
s/"/\\"/g
3,$ s/^[ \t]*#.*//
s/^/  "/
s/$/\\n"/
$ s/$/;/'

# ATTN: the file did not just get corrupted ;-)

cat <<HERE_DOC_END

#endif /* ! ${UPCASEBASE}_IV_H */
HERE_DOC_END
