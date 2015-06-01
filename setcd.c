/** SETCD
 ** 
 ** Purpose  : Set up the cd extension environment variables.
 ** Language : ANSI c
 ** Date     : November 18, 1996
 **
 ** Syntax   : setcd [ all ] | [ user to ( <user> | home ) ] | 
 **                  [ stack style <dest[ructive] | nond[estructive]> ] | [ stack size <#> ]
 **
 ** History  :
 **            12-09-96   :: J. Newell :: Beta   :: Completed Beta Testing
 **            03-31-97.1 :: J. Newell :: UPDATE :: Updated memset to ansi c.
 **            04-16-97.1 :: J. Newell :: UPDATE :: Add logical and absolute path settings
 **            08-21-97.1 :: J. Newell :: BUG    :: Fix Directory Listing file creation
 **            12-22-97   :: J. Newell :: ENHANCE:: Warning/Skip bad environment variable names
 **
 **/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cd-ext.h"
#include "parse.h"

#define SETCD_ALL           1
#define SETCD_USER          2
#define SETCD_SIZE          3
#define SETCD_STYLE_NON     4
#define SETCD_STYLE_DES     5
#define SETCD_ABSOLUTE_PATH 6
#define SETCD_LOGICAL_PATH  7

#define MSIZE 6*MAXPATHLEN
#define STACK_LIMIT 36

static int csh_shell = 0;
                 
void initialize    ( char *progname );
void set_cd_all    ( void );
void set_cd_user   ( void );
void set_cd_style  ( int stack_style );
void set_cd_size   ( void );
void set_cd_path_format( int cd_path_format );

int main( int argc, char **argv )
{
     initialize( argv[0] );

     switch ( PARSE_command_line( argc, argv ) )
     {
     case SETCD_ALL :
          set_cd_all();
          break;
     case SETCD_USER :
          set_cd_user();
          break;
     case SETCD_STYLE_NON :
          set_cd_style( SETCD_STYLE_NON );
          break;
     case SETCD_STYLE_DES :
          set_cd_style( SETCD_STYLE_DES );
          break;
     case SETCD_SIZE :
          set_cd_size();
          break;
     case SETCD_ABSOLUTE_PATH :
          set_cd_path_format( SETCD_ABSOLUTE_PATH );
          break;
     case SETCD_LOGICAL_PATH :
          set_cd_path_format( SETCD_LOGICAL_PATH );
          break;          
     default :
          CD_usage_error("[ all ] | [ user to ( <user> | HOME ) ] | [ stack style <dest[ructive] | nond[estructive]> ] | [ stack size < # > 0 < 36 > ] | [ path format <abs[olute] | log[ical]> ]" );
          
     }
     
     return 0;
}

void initialize( char *progname )
{
     CD_initialize( progname );
     PARSE_initialize();
     PARSE_set( NULL                        , SETCD_ALL   );
     PARSE_set( "all"                       , SETCD_ALL   );
     PARSE_set( "user to <>"                , SETCD_USER  );
     PARSE_set( "stack style dest"          , SETCD_STYLE_DES );
     PARSE_set( "stack style destructive"   , SETCD_STYLE_DES );
     PARSE_set( "stack style nond"          , SETCD_STYLE_NON );
     PARSE_set( "stack style nondestructive", SETCD_STYLE_NON );
     PARSE_set( "stack size <>"             , SETCD_SIZE  );
     PARSE_set( "path format absolute"      , SETCD_ABSOLUTE_PATH );
     PARSE_set( "path format abs"           , SETCD_ABSOLUTE_PATH );
     PARSE_set( "path format logical"       , SETCD_LOGICAL_PATH  );
     PARSE_set( "path format log"           , SETCD_LOGICAL_PATH  );

     if ( CD_getenv( "CD_CSH", FALSE ) != NULL )
     {
         csh_shell=1;
     }
}

void set_cd_all    ( void )
{
     FILE *file_stream;
     char *token_variable;
     char *token_value;
     char  stream[MAXPATHLEN];
     char  environment_string[MSIZE];
     char  set_string[MAXPATHLEN];
     
     memset( (void *)environment_string, 0, sizeof(environment_string) ); /* 03-31-97.1*/
     strcpy( environment_string, " " );

     if ((file_stream = fopen( CD_directory_list_path, "r" )) == NULL)
     {    /* 08-21-97.1 */
          /* CD_directory_list_path does not exist, so create it */
          file_stream = fopen( CD_directory_list_path, "w" );
          exit( 1 );
     }

     while ( fgets( stream, MAXPATHLEN, file_stream ) != NULL )
     {  
          token_variable = strtok( stream, " \t\n" );
          token_value = strtok (NULL, "\t\n");

          if ( isalpha( token_variable[0] ) || token_variable[0] == '_' ) /* 12-22-97 */
          {
               if ( strlen( token_variable ) < CD_MAX_SHORTNAME_SIZE )
               {
                    if ( strstr( token_variable, "." ) == NULL )
                    {
                        if (csh_shell)
                            sprintf( set_string, "set %s='%s' ; setenv %s %s ;", token_variable, token_value, token_variable, token_value );
                        else
                            sprintf( set_string, "%s='%s' ; export %s ;", token_variable, token_value, token_variable );

                        strcat( environment_string, set_string );
          
                    } else
                         fprintf( stderr,
                                  "%s WARNING :: A Short name can not contain a period character. "
                                  "Enivornment variable %s will not be set.\n",
                                  CD_progname, token_variable ); 
                    
               } else
                    fprintf( stderr,
                             "%s WARNING :: A Short name can not exceed %d characters. "
                             "Enivornment variable %s will not be set.\n",
                             CD_progname, CD_MAX_SHORTNAME_SIZE - 1, token_variable );               
          } else
               fprintf( stderr,
                        "%s WARNING :: Short name first character must be [_|a-z|A-Z]. "
                        "Enivornment variable %s will not be set.\n",
                        CD_progname, token_variable );         
     }
     
     fclose( file_stream );
     
     printf( "%s", environment_string );
}

void set_cd_user( void )
{
     char *cd_home;
     
     if ( strcmp( PARSE_value(0), "home" ) == 0 )
     {
          if ( (cd_home = CD_getenv( "CD_HOME", TRUE )) == NULL ) exit( 1 );
                                                    
          if (csh_shell)                    
              fprintf( stdout, "setenv HOME %s", cd_home );            
          else
              fprintf( stdout, "HOME=%s ; export HOME", cd_home );
     }
     else
     {
         if (csh_shell)
             fprintf( stdout, "setenv HOME %s", PARSE_value(0) );
         else
             fprintf( stdout, "HOME=~%s ; export HOME", PARSE_value(0) );
     }
                        
}

void set_cd_style( int stack_style )
{

     if      ( stack_style == SETCD_STYLE_DES )
     {    
         if (csh_shell)
             fprintf( stdout, "setenv STACK_STYLE DESTRUCTIVE" );
         else
             fprintf( stdout, "STACK_STYLE=DESTRUCTIVE ; export STACK_STYLE" );
     }
     else if ( stack_style == SETCD_STYLE_NON )
     {          
         if (csh_shell)
             fprintf( stdout, "setenv STACK_STYLE NONDESTRUCTIVE" );
         else
             fprintf( stdout, "STACK_STYLE=NONDESTRUCTIVE ; export STACK_STYLE" );
     }     
}

void set_cd_size( void )
{
     if ( ( CD_isnum( PARSE_value(0) ) ) && ( atoi( PARSE_value(0) ) > 0 ) && ( atoi( PARSE_value(0) ) < STACK_LIMIT ) )
     {          
         if (csh_shell)
             fprintf( stdout, "setenv STACK_SIZE %s", PARSE_value(0) );
         else
             fprintf( stdout, "STACK_SIZE=%s ; export STACK_SIZE  ", PARSE_value(0) );
     }
     else
          fprintf( stderr, "\n%s ERROR :: Stack size must be an integer in the range 1 to %d.\n\n", CD_progname, STACK_LIMIT - 1 ); 
}


void set_cd_path_format( int cd_path_format )
{
     if ( cd_path_format == SETCD_LOGICAL_PATH )
     {          
         if (csh_shell)
             fprintf( stdout, "setenv CD_PATH_FORMAT LOGICAL" );
         else
             fprintf( stdout, "CD_PATH_FORMAT=LOGICAL;export CD_PATH_FORMAT" );
     }
     else
     {
         if (csh_shell)
             fprintf( stdout, "setenv CD_PATH_FORMAT ABSOLUTE" );
         else
             fprintf( stdout, "CD_PATH_FORMAT=ABSOLUTE;export CD_PATH_FORMAT" );
     }
}

          
