

export CD_HOME=$HOME
export CD_SHELL_TYPE=bash-ksh-sh
export CD_SIZE=10
export CD_STYLE=nondestructive
export CD_PATH_FORMAT=logical
export PROMPT_STRING='$PWD => '

SILENT=FALSE

while [[ "${1:-}" != "" ]]
do
    case "${1:-}" in
	-size)
	    shift
	    export CD_SIZE=${1:-}
	    ;;
	-style)
	    shift
	    export CD_STYLE=${1:-}
	    ;;
	-version)
	    shift
	    export CD_VERSION=${1:-}
	    ;;
	-home)
	    shift
	    export CD_HOME=${1:-}
	    ;;
	-bin)
	    shift
	    export CD_BIN=${1:-}
	    ;;
	-prompt)
	    shift
	    export PROMPT_STRING=${1:-}
	    ;;
	-noprompt)
	    export PROMPT_STRING=""
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

if [ "$PROMPT_STRING" != "" ]
then
	PS1=$PROMPT_STRING	
fi

if [ $SILENT = "FALSE" ]
then
	echo "$CD_SHELL_TYPE-$CD_VERSION :: Creating aliases using $CD_BIN directory ..."
fi


# original cd extension commands
chdir     () { \cd ${*:-}; }
mycd      () { COMMAND="`$CD_BIN/ccd-$CD_SHELL_TYPE-$CD_VERSION    ${*:-}`"; eval $COMMAND; eval ${EXECUTE_AFTER_CD:-}; }
delete    () { COMMAND="`$CD_BIN/delete-$CD_SHELL_TYPE-$CD_VERSION ${*:-}`"; eval $COMMAND; }
list      () { $CD_BIN/list-$CD_SHELL_TYPE-$CD_VERSION   ${*:-}| cat; }
save      () { COMMAND="`$CD_BIN/save-$CD_SHELL_TYPE-$CD_VERSION   ${*:-}`"; eval $COMMAND; }
setcd     () { COMMAND="`$CD_BIN/setcd-$CD_SHELL_TYPE-$CD_VERSION  ${*:-}`"; eval $COMMAND; }

# speed up the execution of the cd extension commands
typeset -fx chdir
typeset -fx mycd
typeset -fx delete
typeset -fx list
typeset -fx save
typeset -fx setcd

# ood philosophy commands
alias -x cd=mycd
alias -x cd.delete=delete
alias -x cd.delet=delete
alias -x cd.dele=delete
alias -x cd.del=delete
alias -x cd.de=delete
alias -x cd.d=delete
alias -x cd.list=list
alias -x cd.lis=list
alias -x cd.li=list
alias -x cd.l=list
alias -x cd.set=setcd
alias -x cd.se=setcd
alias -x cd.save=save
alias -x cd.sav=save
alias -x cd.sa=save
alias -x cd.s=save

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
