export CD_BIN=/usr/local/bin
export CD_PRE=cdext
export CD_HOME=$HOME
export CD_SHELL_TYPE=bash
export CD_SIZE=10
export CD_STYLE=nondestructive
export CD_PATH_FORMAT=logical

SILENT=TRUE

while [ "$1" != "" ]
do
    case "$1" in
	-size)
	    shift
	    export CD_SIZE=$1
	    ;;
	-style)
	    shift
	    export CD_STYLE=$1
	    ;;
	-version)
	    shift
	    export CD_VERSION=$1
	    ;;
	-home)
	    shift
	    export CD_HOME=$1
	    ;;
	-bin)
	    shift
	    export CD_BIN=$1
	    ;;
	-prompt)
	    shift
	    export PROMPT_STRING=$1
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

	echo "$CD_SHELL_TYPE :: Configuring cd extensions  ..."

fi

if [ "$PROMPT_STRING" != "" ]

then

	PS1=$PROMPT_STRING	

fi

if [ $SILENT = "FALSE" ]
then
	echo "$CD_SHELL_TYPE :: Creating aliases using $CD_BIN directory ..."
fi

# original cd extension commands
chdir     () { \cd $*; }
mycd      () { COMMAND="`$CD_BIN/cdext-ccd        $*`"; eval $COMMAND; }
delete    () { COMMAND="`$CD_BIN/cdext-delete  $*`"; eval $COMMAND; }
list      () {           $CD_BIN/cdext-list    $*| cat; }
save      () { COMMAND="`$CD_BIN/cdext-save    $*`"; eval $COMMAND; }
setcd     () { COMMAND="`$CD_BIN/cdext-setcd   $*`"; eval $COMMAND; }

# ood philosophy commands

alias cd=mycd
alias cd.delete=delete
alias cd.delet=delete
alias cd.dele=delete
alias cd.del=delete
alias cd.de=delete
alias cd.d=delete
alias cd.list=list
alias cd.lis=list
alias cd.li=list
alias cd.l=list
alias cd.set=setcd
alias cd.se=setcd
alias cd.save=save
alias cd.sav=save
alias cd.sa=save
alias cd.s=save
alias cd.ls="list stack"

if [ -e $CD_BIN/cdext-setcd ]; then
if [ $SILENT = "FALSE" ]
then
	echo "$CD_SHELL_TYPE :: Setting Environment Variables ..."

	setcd

	echo "$CD_SHELL_TYPE :: Setting stack size = $CD_SIZE"

	setcd stack size $CD_SIZE

	echo "$CD_SHELL_TYPE :: Setting stack style $CD_STYLE"

	setcd stack style $CD_STYLE

	echo "$CD_SHELL_TYPE :: Setting path format $CD_PATH_FORMAT"

	setcd path format $CD_PATH_FORMAT

	echo "$CD_SHELL_TYPE :: Done."
else
	setcd

	setcd stack size  $CD_SIZE

	setcd stack style $CD_STYLE

	setcd path format $CD_PATH_FORMAT
fi
fi
