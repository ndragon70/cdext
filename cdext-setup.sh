
PROMPT_STRING="`/usr/bin/pwd` => "              ; export PROMPT_STRING
CD_HOME=$HOME                                   ; export CD_HOME
CD_SHELL_TYPE=bash-ksh-sh                       ; export CD_SHELL_TYPE
CD_SIZE=10                                      ; export CD_SIZE
CD_STYLE=nondestructive                         ; export CD_STYLE
CD_PATH_FORMAT=logical                          ; export CD_PATH_FORMAT
SILENT=FALSE

# a sh hack ... the PS1 command does not execute after every change of directory
EXECUTE_AFTER_CD='PS1="`/usr/bin/pwd` => "'; export EXECUTE_AFTER_CD

while [ "$1" != "" ]
do
    case "$1" in
	-size)
	    shift
	    CD_SIZE=$1; export CD_SIZE
	    ;;
	-style)
	    shift
	    CD_STYLE=$1; export CD_STYLE
	    ;;
	-version)
	    shift
	    CD_VERSION=$1; export CD_VERSION
	    ;;
	-home)
	    shift
	    CD_HOME=$1; export CD_HOME
	    ;;
	-bin)
	    shift
	    CD_BIN=$1; export CD_BIN
	    ;;
	-prompt)
	    shift
	    PROMPT_STRING=$1; export PROMPT_STRING
	    EXECUTE_AFTER_CD=$PROMPT_STRING; export EXECUTE_AFTER_CD
	    ;;
	-noprompt)
	    PROMPT_STRING=""; export PROMPT_STRING
	    EXECUTE_AFTER_CD=$PROMPT_STRING; export EXECUTE_AFTER_CD
	    ;;
	-silent)
	    SILENT=TRUE
	    ;;
	*)
	    ;;
    esac
    
    shift
done


if [ $SILENT = "FALSE" ]
then
	echo "$CD_SHELL_TYPE-$CD_VERSION :: Configuring cd extensions  ..."
fi

if [ $SILENT = "FALSE" ]
then
	echo "$CD_SHELL_TYPE-$CD_VERSION :: Creating aliases using $CD_BIN directory ..."
fi


# original cd extension commands :: sh does not support alias or cd.<> function names
cd        () { COMMAND="`$CD_BIN/ccd-$CD_SHELL_TYPE-$CD_VERSION    $*`"; eval $COMMAND; eval $EXECUTE_AFTER_CD; }
delete    () { COMMAND="`$CD_BIN/delete-$CD_SHELL_TYPE-$CD_VERSION $*`"; eval $COMMAND; }
list      () { $CD_BIN/list-$CD_SHELL_TYPE-$CD_VERSION   $*| cat; }
save      () { COMMAND="`$CD_BIN/save-$CD_SHELL_TYPE-$CD_VERSION   $*`"; eval $COMMAND; }
setcd     () { COMMAND="`$CD_BIN/setcd-$CD_SHELL_TYPE-$CD_VERSION  $*`"; eval $COMMAND; }

if [ $SILENT = "FALSE" ]
then

echo "$CD_SHELL_TYPE-$CD_VERSION :: Setting Environment Variables ..."

setcd

echo "$CD_SHELL_TYPE-$CD_VERSION :: Setting stack size = $CD_SIZE"

setcd stack size $CD_SIZE

echo "$CD_SHELL_TYPE-$CD_VERSION :: Setting stack style $CD_STYLE"

setcd stack style $CD_STYLE

echo "$CD_SHELL_TYPE-$CD_VERSION :: Setting path format $CD_PATH_FORMAT"

setcd path format $CD_PATH_FORMAT

echo "$CD_SHELL_TYPE-$CD_VERSION :: Done."

else

setcd
setcd stack size  $CD_SIZE
setcd stack style $CD_STYLE
setcd path format $CD_PATH_FORMAT


fi
