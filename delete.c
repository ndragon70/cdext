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

/** DELETE
 ** 
 ** Purpose  : Delete directory entry(s) from the cd extension directory list.
 ** Language : ANSI c
 ** Date     : November, 18, 1996
 **
 ** Syntax   : delete [ ALL ] | [ <short name> | # | #-# [, <short name> | # | #-# [ , ... ] ]  
 **
 ** History  :
 **            12-09-96   :: J. Newell :: Beta   :: Completed Beta Testing
 **            03-31-97.1 :: J. Newell :: UPDATE :: Updated memset to ansi c
 **            04-16-97.1 :: J. Newell :: FIX    :: Initialize buffers
 **            07-25-97.1 :: J. Newell :: FIX    :: DELETE_ALL was set 1, so if you delete 1,
 **                                                 delete ALL.  Since, PARSE_WILD is set to
 **                                                 -1(parse.h), I had to use -2 for a new
 **                                                 value.  To bad ansi C didn't support true
 **                                                 enumeration.
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cd-ext.h"
#include "parse.h"

#define ARG_LIMIT 10000

/* FIX 07-25-97.1 */
#define DELETE_ALL -2
#define DELETE      2

struct range_struct
{
     int start;
     int stop;
};


struct list_record
{
     char name[BUFSIZ];
     char path[BUFSIZ];
     int  garbage;
};

struct list_record list[100];

int  dir_list_size = 0;

void  initialize    ( char *progname );
int   isrange       ( struct range_struct *r, char *s );
char *compress_args ( int argc, char **argv );
void  delete        ( char *name, int first, int second );
void  process_delete( int argc, char **argv );
void  load          ( char *filename );
void  save          ( char *filename );


int main( int argc, char **argv )
{
     
     initialize( argv[0] );     

     load( CD_directory_list_path );
     
     switch ( PARSE_command_line( argc, argv ) )
     {
     case DELETE_ALL :
          delete( NULL, 1, DELETE_ALL );
          break;
     case PARSE_USAGE_ERROR :
          CD_usage_error( "[ ALL ] | [ <short name> | # | #-# [, <short name> | # | #-# [ , ... ] ]" );
          break;
     default :
          process_delete( argc, argv );     
     }

     save( CD_directory_list_path );
     
     return 0;
}

void initialize( char *progname )
{
     CD_initialize( progname );
     PARSE_initialize();
     PARSE_set( NULL              , PARSE_USAGE_ERROR );
     PARSE_set( "ALL"             , DELETE_ALL        );
     PARSE_set( "*"               , DELETE            );
}

int isrange( struct range_struct *r, char *s )
{     
     char *token;
     char temp_string[MAXPATHLEN];
     int  found = FALSE;
     int  index = 0;
     
     if ( strstr( s, "-" ) == NULL ) return found; 

     strcpy( temp_string, s );
          
     for ( token = strtok( s, "-" ); token; token = strtok( NULL, "-" ), index++ )
     {
          if      ( ( index == 0 )  && CD_isnum( token ) )
          {
               r->start = atoi( token );
               r->stop  = dir_list_size;
          }
          else if ( ( index == 1 ) && CD_isnum( token ) )
               r->stop  = atoi( token );
          else
          {
               fprintf( stderr, "\n%s ERROR :: Range %s is invalid.\n\n", CD_progname, temp_string );
               exit( 1 );
          }
          
          found = TRUE;
     }
     
     return found;
}

char *compress_args( int argc, char **argv )
{
     char *temp_string;
     int  index = 1;
     int  total_size = 0;
         
     
     for ( index = 1; index < argc; index++ )
          total_size+= strlen( argv[index] );
     
     temp_string = (char *)malloc( total_size + 1 );
     memset( (void *)temp_string, 0, total_size ); /* 03-31-97.1 */
     
     for ( index = 1; index < argc; index++ )
          strcat( temp_string, argv[index] );
     return temp_string;
}

void delete( char *name, int first, int second )
{
     char temp_command[MAXPATHLEN];
     char output_command[MAXPATHLEN];
     int  found = FALSE;
     int  index;
     int csh_shell = 0;
     
     if ( dir_list_size == 0 ) 
     {
          fprintf( stderr, "\n%s ERROR :: No entries in the directory ini file %s.\n\n",  CD_progname, CD_directory_list_path );
          exit( 1 );
     }

     memset( temp_command, 0, MAXPATHLEN );   /* 04-16-97.1 */
     memset( output_command, 0, MAXPATHLEN ); /* 04-16-97.1 */
     
     if ( CD_getenv( "CD_CSH", FALSE ) != NULL )
     {
         csh_shell=1;
     }

     if (name)
     {
          for ( index = 1; index <= dir_list_size; index++ )
          {
               if ( strcmp( list[index].name, name ) == 0 ) 
               {
                    list[index].garbage = TRUE;
                    
                    if (csh_shell) 
                    {                    
                        sprintf( temp_command, ";unset %s; unsetenv %s ", list[index].name, list[index].name );
                    } else
                    {                    
                        sprintf( temp_command, "unset %s ; ", list[index].name );
                    }

                    strcat( output_command, temp_command );

                    found = TRUE;

                    break; 
               } 
          }
          if ( !found )
          {
               fprintf( stderr, "\n%s ERROR :: %s not found.\n\n", CD_progname, name );
               exit( 1 );
          }
     }
     else
     {
          if ( second == DELETE_ALL ) second = dir_list_size;

          for ( index = first; index <= dir_list_size && index <= second; index++ ) 
          {             
               list[index].garbage = TRUE;
                    
               if (csh_shell) 
               {
                   sprintf( temp_command, ";unset %s; unsetenv %s ", list[index].name, list[index].name );
               }
               else
               { 
                   sprintf( temp_command, "unset %s ; ", list[index].name );
               }

               strcat( output_command, temp_command );               
               found = TRUE;
          }
          if ( !found )
          {
               fprintf( stderr, "\n%s ERROR :: Invalid delete range.\n\n", CD_progname );
               exit( 1 );
          }    
     }
     fprintf( stdout, "%s", output_command );
}

void process_delete( int argc, char **argv )
{
     char   *token;     
     char   *combined_arg_list;
     char   *token_list[ARG_LIMIT];
     int    index = 0;
     int    token_list_size = 0;
     struct range_struct range;
     
     combined_arg_list = compress_args( argc, argv );
     for ( token = strtok( combined_arg_list, ","); token; token = strtok(NULL, ",") )
     {
          token_list[index] = (char *)malloc( strlen(token) );
          strcpy( token_list[index++], token );             
          token_list_size = index; 
     }
     for ( index = 0; index < token_list_size; index++ )
     {             
          if      ( isrange( &range, token_list[index] ) )
               delete( NULL, range.start, range.stop );
          else if ( CD_isnum( token_list[index] ) )
               delete( NULL, atoi( token_list[index] ), atoi( token_list[index] ) );
          else 
               delete( token_list[index], 0, 0 );
     }
}

void load( char *filename )
{
     FILE *file_stream;
     char *token1;
     char *token2;
     char  stream[MAXPATHLEN];
     int   index = 1;
     
     if ( ( file_stream = CD_fopen( filename, "r" ) ) != NULL )
     { 
          while ( fgets( stream, MAXPATHLEN, file_stream ) != NULL )
          {  
               token1 = strtok( stream, " \t\n" );
               token2 = strtok( NULL, " \t\n" );
               strcpy( list[index].name, token1 );
               strcpy( list[index].path, token2 );
               list[index].garbage = FALSE;
               index++;
          }
          dir_list_size = index - 1;
          fclose( file_stream );
     }
     else
          dir_list_size = 0;
}

void save( char *filename )
{
     FILE *file_stream;
     int  index = 1;
     
     file_stream = CD_fopen( filename, "w" );
     for ( index = 1; index <= dir_list_size; index++ )
     {  
          if ( ! list[index].garbage )
               fprintf( file_stream, "%s %s\n", list[index].name, list[index].path );
     }
     fclose( file_stream ); 
}

