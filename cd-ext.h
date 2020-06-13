/* BSD License
Copyright (c) 2005, James Newell
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following 
conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
in the documentation and/or other materials provided with the distribution. 
Neither the name of the James Newell nor the names of its contributors may be used to endorse or promote products derived from 
this software without specific prior written permission. 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/** CD_EXT.H
 ** 
 ** Purpose  : Group common CD extension functions.
 ** Language : Unix c
 ** Date     : November 11, 1996
 **
 ** Syntax   : Header File
 **
 ** History  :
 **            11-11-96 :: J. Newell :: ENHANCE :: Group Common CD functions
 **            12-09-96 :: J. Newell :: Beta    :: Completed Beta Testing
 **            04-16-97 :: J. Newell :: ADD     :: Add CD_set_path_format function
 **            11-23-01 :: J. Newell :: ADD     :: Add CD_STACK_DELIM to use by modules.
 **                                                Use ; for the delimeter.  This corrects a
 **                                                problem under Cygwin with <drive letter>: paths.
 **
 **/

#ifndef CD_EXT_H
#define CD_EXT_H

#ifndef BOOL
#define BOOL
#define FALSE 0
#define TRUE  1
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif

extern char CD_home_directory[MAXPATHLEN];
extern int  CD_path_format;
extern char CD_current_working_directory[MAXPATHLEN];
extern char CD_directory_list_path[MAXPATHLEN];
extern char CD_progname[MAXPATHLEN];

#define CD_MAX_SHORTNAME_SIZE 16
#define CD_ERROR            0
#define CD_SUCCESS          1
#define CD_DIRECTORY_LIST_FILENAME ".DIR_LIST.INI"
#define CD_ABSOLUTE_FORMAT  0
#define CD_LOGICAL_FORMAT   1
#define CD_STACK_DELIM    ";"

void  CD_stoupper          ( char *s );
char *CD_is_valid_shortname( char *s );
char *CD_getenv            ( char *s, int verbose );
int   CD_isnum             ( char *s );
int   CD_getcwd            ( char *s );
void  CD_usage_error       ( char *error );
FILE *CD_fopen             ( char *filename, char *attributes );
void  CD_set_path_format   ( int   cd_path_format );
void  CD_initialize        ( char *progname );

#endif

