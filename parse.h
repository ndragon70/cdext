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

/** PARSE.H
 ** 
 ** Purpose  : Common Command Line Parse Utilities
 ** Language : Unix c
 ** Date     : November 18, 1996
 **
 ** Syntax   : Header File
 **
 ** History  :
 **            12-09-96 :: J. Newell :: Beta :: Completed Beta Testing
 **
 **/


#ifndef PARSE_H
#define PARSE_H

/*#define DEBUG*/

#ifndef BOOL
#define BOOL
#define FALSE 0
#define TRUE  1
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif

#define PARSE_USAGE_ERROR 0
#define PARSE_WILD        -1

void  PARSE_initialize  ( void );
int   PARSE_command_line( int argc, char **argv );
int   PARSE_set         ( char *syntax, int return_code );
int   PARSE_number      ( void );
char *PARSE_value       ( int position );

#endif
