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

/** PARSE.C
 ** 
 ** Purpose  : Common Command Line Parse Utilities
 ** Language : ANSI c
 ** Date     : November 18, 1996
 **
 ** Syntax   : NA
 **
 ** History  :
 **            11-18-96   :: J. Newell :: Init   :: Common Command Line Parse Utilities
 **            12-09-96   :: J. Newell :: Beta   :: Completed Beta Testing
 **            01-23-97   :: J. Newell :: UPDATE :: Updated all malloc calls to reflect (data type)malloc.
 **            03-04-97.1 :: J. Newell :: BUG    :: Initialize Data Objects in correct order.
 **            03-04-97.2 :: J. Newell :: BUG    :: Initialize Data Object  for correct strtok parsing.
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

#define MAX_TOKENS   100
#define MAX_COMMANDS  50

#define VALUE    0
#define VARIABLE 1
#define WILD     2

int  number_of_variables;
int  number_of_commands;
char command_value[MAX_COMMANDS][MAXPATHLEN];

struct token_struct
{
     int  token_type;
     char token[MAXPATHLEN];
};

struct command_struct
{
     int    number_of_tokens;
     int    iswild;
     struct token_struct token_array[MAX_TOKENS];
     int    return_code;
};

struct command_struct command_array[MAX_COMMANDS];


void PARSE_initialize( void )
{
     number_of_variables = 0;
     number_of_commands  = 0;
     command_array[number_of_commands].iswild = FALSE; /* 03-04-97.1 */
}


int PARSE_command_line( int argc, char **argv )
{
     int x;
     int y;
     int parse_error;
     int pass_on    = FALSE;
     
     for ( x = 0; x < number_of_commands; x++ )
     {
          if ( command_array[x].iswild  ) pass_on = TRUE;
          
          if ( command_array[x].number_of_tokens == argc - 1 )
          {
               parse_error = FALSE;
               
               for ( y = 1; ( y < argc ) && ( !parse_error); y++ )
               {
                    if ( command_array[x].token_array[y-1].token_type == VARIABLE )
                    {
                         strcpy( command_value[number_of_variables++], argv[y] );
                    }
                    else
                    {
                         if ( strcmp( command_array[x].token_array[y-1].token, argv[y] ) != 0 )
                         {
                              parse_error = TRUE;
                         }
                    }
               }
               if ( !parse_error ) return command_array[x].return_code;
          }
     }

     if ( pass_on )
          return PARSE_WILD;
     else
          return PARSE_USAGE_ERROR;
     
}

int PARSE_set( char *syntax, int return_code )
{
     int  x = 0;
     char *tmp_syntax = (char *)malloc( MAXPATHLEN );
     char *token;

     if (syntax) strcpy( tmp_syntax, syntax );
     else
          memset( (void *)tmp_syntax, 0, MAXPATHLEN ); /* 03-04-97.2 */
     
     for ( x = 0, token = strtok( tmp_syntax, " \t" ); token; token = strtok( NULL, " \t" ), x++ )
     {
          if ( strcmp( token, "<>" ) == 0 )
          {
               command_array[number_of_commands].token_array[x].token_type = VARIABLE;
               strcpy( command_array[number_of_commands].token_array[x].token, "" );
          }
          else if ( strcmp( token, "*" ) == 0 )
          {
               command_array[number_of_commands].iswild = TRUE;
               command_array[number_of_commands].token_array[x].token_type = WILD;
               strcpy( command_array[number_of_commands].token_array[x].token, "" );
          }
          else
          {
               command_array[number_of_commands].token_array[x].token_type = VALUE;
               strcpy( command_array[number_of_commands].token_array[x].token, token );
          }
     }
     
     command_array[number_of_commands].number_of_tokens   = x;
     command_array[number_of_commands++].return_code      = return_code;

     free( tmp_syntax );
     
     return TRUE;
}

int   PARSE_number( void )
{
     return number_of_variables;
}


char *PARSE_value( int position )
{
     char *temp = (char *)malloc( sizeof(command_value[position]) );
     
     strcpy( temp, command_value[position] );
     
     return temp;
}

