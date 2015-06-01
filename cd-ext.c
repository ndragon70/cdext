/** CD_EXT.C
 ** 
 ** Purpose  : Group common CD extension functions.
 ** Language : Unix c
 ** Date     : November 11, 1996
 **
 ** Syntax   : Source Code
 **
 ** History  :
 **            11-11-96   :: J. Newell :: ENHANCE :: Group Common CD functions
 **            12-09-96   :: J. Newell :: BETA    :: Completed Beta Testing
 **            03-04-97   :: J. Newell :: FIX     :: Increased CD_getcwd directory size buffer
 **            03-05-97   :: J. Newell :: FIX     :: Don't capitalize the program name(CD_progname)
 **            04-16-97.1 :: J. Newell :: ADD     :: Add logical and absolute path settings
 **            04-16-97.2 :: J. Newell :: ADD     :: Add CD_set_path_format fucntion
 **
 **/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cd-ext.h"

void  CD_stoupper( char *s )
{
     int x;
     
     for ( x = 0; x < strlen( s ); x++ )
          s[x] = toupper( s[x] );
}


char *CD_is_valid_shortname( char *s )
{
     if ( isalpha( s[0] ) || s[0] == '_' ) 
          if ( strlen( s ) < CD_MAX_SHORTNAME_SIZE )
               if ( strstr( s, "." ) == NULL ) return s;
     
     fprintf( stderr,
              "\n%s ERROR :: Short name first character must be [_|A-Z|a-z], "
              "may not contain a period character, and may not exceed %d characters.\n\n",
              CD_progname, CD_MAX_SHORTNAME_SIZE - 1 );
     
     exit( 0 );
}


char *CD_getenv( char *s, int verbose )
{
     char *env;
     
     if ( ( env = getenv( s ) ) != NULL )
          return env;
     else
     {
          if (verbose)
               fprintf( stderr, "%s :: getenv error :: %s undefined.\n", CD_progname, s );
     }
     

     return NULL;

}


int CD_isnum( char *s )
{
  int x = 0;
  for (x=0; x < strlen(s); x++)
     if ( !isdigit(s[x]) ) if ( s[x] != '-' ) return 0;
  return 1;
}


int CD_getcwd( char *s )
{
     char *cwd;

     if ( CD_path_format == CD_ABSOLUTE_FORMAT )
          cwd = (char *)getcwd(NULL, MAXPATHLEN);
     else
          cwd = (char *)CD_getenv( "PWD", FALSE );
     
     if ( cwd == NULL)
     {
          perror( CD_progname );
          if ( CD_path_format == CD_ABSOLUTE_FORMAT ) free( cwd );
          return CD_ERROR;
     }
     
     strcpy( s, cwd );
     
     if ( CD_path_format == CD_ABSOLUTE_FORMAT ) free( cwd );
     
     return CD_SUCCESS;

}


void CD_usage_error( char *error )
{
     fprintf( stderr, "\nusage %s : %s\n\n", CD_progname, error );
     exit( 0 );
}


FILE *CD_fopen( char *filename, char *attributes )
{
     FILE *file_stream;
     
     if ( ( file_stream = fopen( filename, attributes ) ) == NULL ) 
     {
          fprintf( stderr, "\n%s ERROR :: File %s does not exist.\n\n", CD_progname, CD_directory_list_path  );
          exit( 1 ); 
     }
     return file_stream;
     
}


/* 04-16-97.2 */
void CD_set_path_format( int cd_path_format )
{
     CD_path_format = cd_path_format;
     if ( !CD_getcwd( CD_current_working_directory) ) exit( 0 );
}


void CD_initialize( char *progname )
{
     char *tmp_ptr;
     
     for ( tmp_ptr = strtok( progname, "/\0 "); tmp_ptr != NULL; tmp_ptr = strtok( NULL, "/\0 ") ) 
              strcpy( CD_progname, tmp_ptr  );

     /* CD_stoupper( CD_progname ); */ /* 03-05-97 */

     /* 04-16-97.1 */
     if ( ( tmp_ptr = CD_getenv( "CD_PATH_FORMAT", FALSE ) ) == NULL )
          CD_path_format = CD_LOGICAL_FORMAT;
     else
     {
          CD_stoupper(tmp_ptr);
          
          if ( strncmp( tmp_ptr, "LOGICAL", 3 ) == 0 )
               CD_path_format = CD_LOGICAL_FORMAT;
          else
               CD_path_format = CD_ABSOLUTE_FORMAT;
     }
     /* 04-16-97.1 */ 
     
     if ( !CD_getcwd( CD_current_working_directory) ) exit( 0 );
     if ( ( tmp_ptr = CD_getenv( "HOME", TRUE ) ) == NULL )  exit( 0 );     

     strcpy( CD_home_directory, tmp_ptr );
     
     sprintf( CD_directory_list_path, "%s/%s", CD_home_directory, CD_DIRECTORY_LIST_FILENAME );
}



