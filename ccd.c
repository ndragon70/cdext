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

/** CD
 ** 
 ** Purpose  : Change Directory using short name notation and push/pop operations.
 ** Language : ANSI c
 ** Date     : November 18, 1996
 **
 ** Syntax   : cd [ [ <path>] [ to <short name> | <entry #> [<path>] ] [ - [ <stack #> ] ] ]
 **
 ** History  : 
 **            12-09-96   :: J. Newell :: Beta   :: Completed Beta Testing
 **            01-22-97.1 :: J. Newell :: BUG    :: AIX dynamic allocation sizof() problem when calling malloc to declare char *.
 **            01-23-97.1 :: J. Newell :: UPDATE :: Updated all malloc calls to reflect (data type)malloc.
 **            03-31-97.1 :: J. Newell :: UPDATE :: Updated memset to ansi c.
 **            04-16-97.1 :: J. Newell :: FIX    :: The output Path is terminated by a 'lf', rather than 'ht' character.
 **            01-05-98   :: J. Newell :: FIX    :: Clean up malloc
 **            07-01-00   :: J. Newell :: FIX    :: Rewrote expand.
 **            07-01-00   :: J. Newell :: FIX    :: Rewrote pop_stack.
 **            07-01-00   :: J. Newell :: BUG    :: Destructive stack parsing does not function.
 **            09-09-01.1 :: J. Newell :: FIX    :: Handling path's with spaces.
 **            09-09-01.2 :: J. Newell :: UPDATE :: Removed all calls to malloc.
 **            11-23-01   :: J. Newell :: ADD    :: Use CD_STACK_DELIM for stack delimeter.
 **                                                 Do not place duplicate paths on the stack.
 **
 **/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cd-ext.h"
#include "parse.h"

#define CD           1
#define CD_PATH      2
#define CD_TO        3
#define CD_TO_PATH   4
#define CD_STACK     5
#define CD_STACK_NO  6

#define CHDIR "\\cd"
#define STACK_ENV_NAME "STACK"
#define COMMAND( cd_args, second_command ) fprintf( stdout, "%s \"%s\";%s", CHDIR, cd_args, second_command )
#define MAXSIZE MAXPATHLEN*6
#define MAX_STACK_SIZE 100 
#define DESTRUCTIVE    0
#define NONDESTRUCTIVE 1
#define YES 1
#define NO  0

struct stack_struct_type
{
     char path_string[MAXSIZE];
     char path[MAX_STACK_SIZE][MAXPATHLEN];
     int  purge[MAX_STACK_SIZE];
     int  max_size;
     int  style;
     int  top;
     int  bottom;
};

struct stack_struct_type stack;

char *expand          ( char *path, char *newpath );
void  initialize      ( char *progname );
char *get_shortname   ( char *name, char *path, char *rbuffer );
int   verify_stack_arg( char *arg );
char *stacktos        ( char path[][MAXPATHLEN], char *s );
void  create_stack    ( void );
char *pop_stack       ( int pop );
char *set_stack       ( char *command );


int main_ccd( int argc, char **argv )
{
     char netpath[MAXSIZE];
     char shortpath[MAXSIZE];
     char command[MAXSIZE];

     initialize( argv[1] );

     switch ( PARSE_command_line( argc, argv ) )
     {
     case CD :
          COMMAND( CD_home_directory, set_stack(command) );
          break;
     case CD_PATH :
          COMMAND( expand( PARSE_value(0), netpath ), set_stack(command) );
          break;
     case CD_TO :
          COMMAND( get_shortname( PARSE_value(0), NULL, shortpath ), set_stack(command) );
          break;
     case CD_TO_PATH :
          COMMAND( get_shortname( PARSE_value(0), PARSE_value(1), shortpath ), set_stack(command) );
          break;
     case CD_STACK :
          COMMAND( pop_stack( 1 ), set_stack(command) );
          break;
     case CD_STACK_NO :
          COMMAND( pop_stack( verify_stack_arg( PARSE_value(0) ) ), set_stack(command) );
          break;
     default :
		  printf("MAXPATHLEN: %d", MAXPATHLEN);
          CD_usage_error( "[ [ <path>] [ to <short name> | <entry #> [<path>] ] [ - [ <stack #> ] ] ]" );   
     }
     
     return 0;
}

char *expand(char *path, char *newpath)
{
  char *position = path;  
  int index;  
  int newIndex = 0;
  int periodCount = 0;

  memset( (void *)newpath, 0, MAXPATHLEN );
  
  /* iterate through entire path, note!  I have no maximum path length error checking here! */
  for (index = 0; index < strlen(position); index++)
  {
    if (position[index] == '.') 
      {
	periodCount++;

	if (periodCount>2) /* replace extra dot's with /.. */
	  {
	    newpath[newIndex    ] = '/';
	    newpath[newIndex + 1] = '.';
	    newpath[newIndex + 2] = '.';
	    newIndex = newIndex + 3;
	  }
	else
	  newpath[newIndex++] = '.';
      }
    else
      {
	periodCount = 0;
	newpath[newIndex++] = position[index];
      }
  } /* end for loop */       

  return newpath;
}

void initialize( char *progname )
{
     create_stack();
     
     CD_initialize( progname );
     PARSE_initialize();
     PARSE_set( NULL       , CD          );
     PARSE_set( "-"        , CD_STACK    );
     PARSE_set( "<>"       , CD_PATH     );
     PARSE_set( "to <>"    , CD_TO       );
     PARSE_set( "to <> <>" , CD_TO_PATH  );
     PARSE_set( "- <>"     , CD_STACK_NO );
}

char *get_shortname( char *name, char *path, char *rbuffer )
{
     FILE *file_stream;
     char *token1;
     char *token2 = NULL;
     char *temp_path = rbuffer;
     char stream[MAXPATHLEN];
     int  found = FALSE;
     int  index;
     
     file_stream = CD_fopen( CD_directory_list_path, "r" );

     if ( CD_isnum(name) )
     {
          for ( index = 1; index < atoi( name ) && fgets( stream, MAXPATHLEN, file_stream ) != NULL; index++ );
          if ( fgets( stream, MAXPATHLEN, file_stream ) != NULL && atoi( name ) > 0 ) 
          { 
               token1 = strtok( stream, " \t\n");
               token2 = strtok( NULL, "\t\n");
               found = TRUE;
          }
     }
     else
     {
          while (  (!found) && (fgets( stream, MAXPATHLEN, file_stream ) != NULL) )
          {
               token1 = strtok( stream, " \t\n");
               token2 = strtok( NULL, "\t\n");
               if ( strcmp( token1, name ) == 0 ) 
               {
                    found = TRUE;
               }  
          }
     }
     
     fclose( file_stream );
     
     if ( found )
     {
          if ( path )
               sprintf( temp_path, "%s/%s", token2, path );
          else
               sprintf( temp_path, "%s/", token2 );
          
          return temp_path;
     }
     else
     {     
          fprintf( stderr, "\n%s ERROR :: Short name %s not found.\n\n", CD_progname, name );
          exit( 1 );
     }
     
}

int verify_stack_arg( char *arg )
{
     if ( CD_isnum( arg ) )
          return atoi( arg );
     else
     {
          fprintf( stderr, "%s ERROR :: Stack arg must be an integer value.\n", CD_progname );
          exit( 1 );
     }
     return -1; /* should never reach here */
}

char *stacktos( char path[][MAXPATHLEN], char *s )
{
     int x;
 
     char temp[MAXSIZE];
     
     memset( (void *)s, 0, sizeof(MAXSIZE) ); /* 03-31-97.1 */
     
     for ( x = stack.top; x >= stack.bottom && stack.top != 0 ; x-- )
     {
       if (stack.purge[x]==NO)
	 {
          sprintf( temp, "%s"CD_STACK_DELIM, path[x] );
          strcat( s, temp );
	 }
     }
     
     return s;
}

void create_stack( void )
{
     char *tmp_ptr;
     char *token;
     char tmp_string[MAXSIZE];
     int x;

     if ( ( tmp_ptr = getenv( "STACK_SIZE" ))  == NULL ) stack.max_size = 5; else stack.max_size = atoi( tmp_ptr );
     if ( ( tmp_ptr = getenv( "STACK_STYLE" )) == NULL ) stack.style = NONDESTRUCTIVE;
     else
     {
          if ( strncmp( tmp_ptr, "DEST", 4 ) == 0 )
               stack.style = DESTRUCTIVE;
          else
               stack.style = NONDESTRUCTIVE;
     }

     if ( (tmp_ptr = getenv( "STACK" )) != NULL )
     
          strcpy( stack.path_string, tmp_ptr );
     
     memset( (void *)stack.path, 0, sizeof(stack.path) ); /* 03-31-97.1 */

     strcpy( tmp_string, stack.path_string );
     
     for ( x = 1, token = strtok( tmp_string, CD_STACK_DELIM); token; token = strtok( NULL, CD_STACK_DELIM), x++);

     stack.top    = x - 1;
     stack.bottom = 1;

     strcpy( tmp_string, stack.path_string );
     
     for ( x = stack.top, token = strtok( tmp_string, CD_STACK_DELIM); token; token = strtok( NULL, CD_STACK_DELIM), x--)
       {
          strcpy( stack.path[x], token );
	  stack.purge[x] = NO;
       }
}

char *pop_stack       ( int count )
{
  int index;

  if (count>stack.max_size) count = stack.max_size;
  if (count<1) count = 1;

  index = stack.top - count;
     
  if ( stack.style == DESTRUCTIVE )
    stack.top = index - 1;

  return stack.path[index]; 
}

char *set_stack( char *command )
{
     char buffer[MAXSIZE];     
     char current_path[MAXSIZE];
     int x;

     if ( CD_getcwd( current_path ) == CD_ERROR )
     {
          fprintf( stderr, "%s ERROR :: Current working directory invalid.\n", CD_progname );
          exit( -1 );
     }

     /* purge all duplicates */
     for (x=stack.bottom;x<=stack.top;x++)
       if (strcmp(stack.path[x],current_path)==0) stack.purge[x]=YES;
     
     strcpy( stack.path[++stack.top], current_path );
     stack.purge[stack.top]=NO;
     
     if ( stack.top + stack.bottom - 1 > stack.max_size )
          stack.bottom++;

     if ( CD_getenv( "CD_CSH", FALSE ) != NULL )
     {
         sprintf( command, "setenv %s '%s'", STACK_ENV_NAME, stacktos( stack.path, buffer ) );         
     } else
     {
         sprintf( command, "%s='%s'; export %s;", STACK_ENV_NAME, stacktos( stack.path, buffer ), STACK_ENV_NAME );
     }

     return command;
}



