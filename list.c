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

/** LIST 
 ** 
 ** Purpose  : View the save directories under the <directory>.ini file.
 **            View the user directory stack.
 ** Language : ANSI c
 ** Date     : November 11, 1996
 **
 ** Syntax   : list [ stack | by name ]
 **
 ** History  : 
 **            12-09-96   :: J. Newell :: Beta   :: Completed Beta Testing
 **            03-31-97.1 :: J. Newell :: UPDATE :: Updated memset to ansi c.
 **            04-16-97.1 :: J. Newell :: UPDATE :: Add logical and absolute settings
 **            07-01-00.1 :: J. Newell :: Warning:: Fixed cast warning
 **            11-23-01   :: J. Newell :: Fix    :: Use CD_STACK_DELIM for delimeter token.
 **             
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cd-ext.h"
#include "parse.h"

#define DIRECTORY 0
#define STACK     1

#define COLUMN_WIDTH 80
#define MARGIN_WIDTH  10

#define MAX_LIST_SIZE 100

#define LIST        1
#define LIST_STACK  2
#define SORT_LIST   3

int  list_array_size = 0;

struct list_record {
    char name[MAXPATHLEN];
    char path[MAXPATHLEN];
    };

struct list_record list_array[MAX_LIST_SIZE];


void initialize    ( char *progname );
void list_directory( void );
void list_stack    ( void );
void load          ( char *filename );
void save          ( char *filename );
int  listcmp       ( const void *x, const void *y );
void sort_list     ( void );

int main( int argc, char **argv )
{
     initialize( argv[0] ); 

     switch ( PARSE_command_line( argc, argv ) )
     {
     case LIST :
          list_directory();
          break;
     case SORT_LIST :
          sort_list();
          list_directory();
          break;
     case LIST_STACK :
          list_stack();
          break;
     default :
          CD_usage_error( "[ stack | by name ]" );
     }
     
     return 0;
}

void initialize( char *progname )
{
     CD_initialize( progname );
     PARSE_initialize();
     PARSE_set( NULL     , LIST       );
     PARSE_set( "stack"  , LIST_STACK );
     PARSE_set( "by name", SORT_LIST  );
     
}

void list_directory( void )
{
     FILE *file_stream;
     int  column = 1;
     char stream[BUFSIZ];
     char *token1;
     char *token2;
     char format_type[MAXPATHLEN];
     char spaces[COLUMN_WIDTH];
     char temp_string[COLUMN_WIDTH];
     char margin_adjustment;
     
     memset( (void *)spaces, 0       , sizeof(spaces) );   /* 03-31-97.1 */
     memset( (void *)spaces, (int)' ', sizeof(spaces)-1 ); /* 03-31-97.1 */
     
     if ( ( file_stream = fopen( CD_directory_list_path, "r" ) ) == NULL ) 
     {
          fprintf( stderr, "\n%s ERROR :: File %s does not exist.\n\n", CD_progname, CD_directory_list_path);
          exit( 1 ); 
     }

     if ( CD_path_format == CD_ABSOLUTE_FORMAT )
          strcpy( format_type, "ABSOLUTE" );
     else
          strcpy( format_type, "LOGICAL" );

     /* 04-16-97.1 */
     fprintf( stdout, "\n*** CD Extension's Directory Entries: User = %s, Format = %s ***\n\n", CD_home_directory, format_type );
     fprintf( stdout, "     Short Name      Path Name\n" );
     fprintf( stdout, "     ----------      ---------\n" );

     while ( fgets( stream, BUFSIZ, file_stream ) != NULL )
     {  
          memset( (void *)temp_string, 0, sizeof(temp_string) ); /* 03-31-97.1 */
          token1 = strtok( stream, " \t\n" );
          token2 = strtok( NULL, "\t\n" );

          if ( column < MARGIN_WIDTH )
               margin_adjustment = ' ';
          else
	    margin_adjustment = '\0'; /* 07-01-00.1 */

          fprintf( stdout, "%c%d.  %s%s%s\n",
                   margin_adjustment,
                   column++,
                   token1,
                   strncpy( temp_string, spaces, CD_MAX_SHORTNAME_SIZE - strlen( token1 ) ),
                   token2 );
     }
     
     fprintf( stdout, "\n");
     
     fclose( file_stream );
}

void list_stack( void )
{
     char *token;
     char *stack_list_ptr;
     char *stack_style_ptr;
     char *stack_size_ptr;
     char  stack_style[CD_MAX_SHORTNAME_SIZE] = "NONDESTRUCTIVE";
     int   stack_size = 1;
     int   index = 1;
     
     stack_list_ptr  = CD_getenv( "STACK", FALSE );
     
     if ( (stack_size_ptr  = CD_getenv( "STACK_SIZE", FALSE )) != NULL ) stack_size = atoi( stack_size_ptr );
     if ( (stack_style_ptr = CD_getenv( "STACK_STYLE", FALSE)) != NULL ) strcpy( stack_style, stack_style_ptr );
     
     printf( "\n*** cd Extension's Stack ( User Stack Size: %d   Stack Style: %s ) ***\n\n", stack_size, stack_style );

     if ( stack_list_ptr == NULL )
          fprintf( stdout, "\tIs Empty\n" );
     else
          for ( token = strtok (stack_list_ptr, CD_STACK_DELIM); token && index <= stack_size; token = strtok(NULL, CD_STACK_DELIM), index++)
               fprintf( stdout, " %d.\t %s\n", index, token );
     
     printf("\n");
}


void load( char *filename )
{
     FILE *file_stream;   
     char stream[BUFSIZ];
     char *token1;
     char *token2;
     int x = 1;


     file_stream = CD_fopen( filename, "r" );
     
     while ( fgets( stream, BUFSIZ, file_stream ) != NULL )
     {  
        token1 = strtok( stream, " \t\n" );
        token2 = strtok( NULL  , " \t\n" );
        strcpy( list_array[x].name, token1 );
        strcpy( list_array[x++].path, token2 );
     }
     
     list_array_size = x - 1;
     
     fclose( file_stream );
}

void save( char *filename )
{
     FILE *file_stream;   
     int x = 1;
     

     file_stream = CD_fopen( filename, "w" );
  
     for ( x = 1; x <= list_array_size; x++ )
          fprintf( file_stream, "%s %s\n", list_array[x].name, list_array[x].path );
     
     fclose( file_stream );
}


int listcmp( const void *x, const void *y )      
{
     return ( strcmp( ((struct list_record *)x)->name, ((struct list_record *)y)->name ) );
}
     
void sort_list   ( void )
{
     int a;
     int b;
     struct list_record temp_list_array[MAX_LIST_SIZE];
     
     load( CD_directory_list_path );
     
     for ( a = 1, b = 0; a <= list_array_size; a++, b++ )
          temp_list_array[b] = list_array[a];
     
     qsort( (void *)temp_list_array, list_array_size, sizeof(temp_list_array[0]), listcmp );
     
     for ( a = 1, b = 0; a <= list_array_size; a++, b++ )
          list_array[a] = temp_list_array[b];
     
     save( CD_directory_list_path );
}
