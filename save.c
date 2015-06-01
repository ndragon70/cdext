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

/** SAVE
 ** 
 ** Purpose  : Save current working or specified directory to the cd extension's directory table.
 ** Language : ANSI c
 ** Date     : November 11, 1996
 **
 ** Syntax   : save [ as <short name> [ to [ user ] <path/username> ] ] | [  <path> as <short name> ] 
 **
 ** History  :
 **            12-09-96   :: J. Newell :: Beta   :: Completed Beta Testing
 **            04-16-97.1 :: J. Newell :: UPDATE :: Add logical and absolute path settings
 **            12-22-97   :: J. Newell :: ENHANCE:: Warning/Skip bad environment variable names
 **     
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cd-ext.h"
#include "parse.h"

#define SAVE              1
#define SAVE_ABS          2
#define SAVE_LOG          3
#define SAVE_AS           4
#define SAVE_AS_ABS       5
#define SAVE_AS_LOG       6
#define SAVE_VAR_AS       7
#define SAVE_AS_USER      8
#define SAVE_AS_USER_ABS  9
#define SAVE_AS_USER_LOG 10

#define DEFAULT_SHORT_NAME "_"

void initialize  ( char *progname );
void save_as_user( char *cwd, char *name, char *user );
void save        ( char *cwd, char *name );

int main( int argc, char **argv )
{
     
     initialize( argv[0] );     
     
     switch ( PARSE_command_line( argc, argv ) )
     {
     case SAVE :
          save( CD_current_working_directory, DEFAULT_SHORT_NAME );
          break;
     case SAVE_ABS :
          CD_set_path_format( CD_ABSOLUTE_FORMAT );
          save( CD_current_working_directory, DEFAULT_SHORT_NAME );
          break;
     case SAVE_LOG :
          CD_set_path_format( CD_LOGICAL_FORMAT );
          save( CD_current_working_directory, DEFAULT_SHORT_NAME );
          break;
     case SAVE_AS :
          save( CD_current_working_directory, CD_is_valid_shortname( PARSE_value(0) ) );
          break;
     case SAVE_AS_ABS :
          CD_set_path_format( CD_ABSOLUTE_FORMAT );
          save( CD_current_working_directory, CD_is_valid_shortname( PARSE_value(0) ) );
          break;
     case SAVE_AS_LOG :
          CD_set_path_format( CD_LOGICAL_FORMAT );
          save( CD_current_working_directory, CD_is_valid_shortname( PARSE_value(0) ) );
          break;
     case SAVE_AS_USER :
          save_as_user( CD_current_working_directory, CD_is_valid_shortname( PARSE_value(0) ), PARSE_value(1) ); 
          break;
     case SAVE_AS_USER_ABS :
          CD_set_path_format( CD_ABSOLUTE_FORMAT );
          save_as_user( CD_current_working_directory, CD_is_valid_shortname( PARSE_value(0) ), PARSE_value(1) ); 
          break;
     case SAVE_AS_USER_LOG :
          CD_set_path_format( CD_LOGICAL_FORMAT );
          save_as_user( CD_current_working_directory, CD_is_valid_shortname( PARSE_value(0) ), PARSE_value(1) ); 
          break;
     case SAVE_VAR_AS :
          save( PARSE_value(0), CD_is_valid_shortname( PARSE_value(1) ) );
          break;
     default :
          CD_usage_error( "[ as <short name> [ to [ user ] <path/username> ] [absolute | logical ]] | [ <path> as <short name> ]" );
     }
     

     exit( 1 );
}


void initialize( char *progname )
{
     CD_initialize( progname );
     PARSE_initialize();
     PARSE_set( NULL                       , SAVE         );
     PARSE_set( "absolute"                 , SAVE_ABS     );
     PARSE_set( "logical"                  , SAVE_LOG     );
     PARSE_set( "as <>"                    , SAVE_AS      );
     PARSE_set( "as <> absolute"           , SAVE_AS_ABS  );
     PARSE_set( "as <> logical"            , SAVE_AS_LOG  );
     PARSE_set( "as <> to <>"              , SAVE_AS_USER );
     PARSE_set( "as <> to <> absolute"     , SAVE_AS_USER_ABS );
     PARSE_set( "as <> to <> logical"      , SAVE_AS_USER_LOG );
     PARSE_set( "as <> to user <>"         , SAVE_AS_USER );
     PARSE_set( "as <> to user <> absolute", SAVE_AS_USER_ABS );
     PARSE_set( "as <> to user <> logical" , SAVE_AS_USER_LOG );
     PARSE_set( "<> as <>"                 , SAVE_VAR_AS  );
}

void save_as_user( char *cwd, char *name, char *user )
{
     FILE *file_stream;
     char stream[BUFSIZ];
     char *token;
     char directory_list_path[MAXPATHLEN];

     sprintf( directory_list_path, "%s/%s", user, CD_DIRECTORY_LIST_FILENAME );

     if ( (file_stream = fopen( directory_list_path, "r" ) ) != NULL ) 
     {  
          while ( fgets( stream, BUFSIZ, file_stream ) != NULL )
          {  
               token = strtok( stream, " \t\n" );
               if ( ( strcmp( token, DEFAULT_SHORT_NAME ) != 0 && ( strcmp( stream, name ) == 0 )  ) ) 
               {
                    fprintf( stderr, "\n%s ERROR : Duplicate short name.\n\n", CD_progname );
                    fclose( file_stream );
                    exit( 0 );
               }
          }

          fclose( file_stream );
     }
     
     if ( (file_stream = fopen( directory_list_path, "a+" )) == NULL )
     {
          fprintf( stderr, "\n%s ERROR : File %s cannot be created.\n\n", CD_progname, directory_list_path );
          exit( 0 );          
     }
     
     fprintf( file_stream, "%s %s\n", name, cwd );

     fclose( file_stream );

     if ( strstr( name, ".") == NULL ) /* 12-22-07 */
     {
         if ( CD_getenv( "CD_CSH", FALSE ) != NULL ) 
         {
             fprintf( stdout, "set %s=%s ; setenv %s %s", name, cwd, name, cwd );
         } else
         {        
     
             fprintf( stdout, "%s=\"%s\" ; export %s", name, cwd, name );
         }
     }
     else
     {
          fprintf( stderr,
                   "%s WARNING :: Short name %s is an invalid name for an environment "
                   "variable.  Enivornment variable %s will not be set.\n",
                   CD_progname, name, name );
     }   
  
}


void save( char *cwd, char *name )
{
     save_as_user( cwd, name, CD_home_directory );     
}

