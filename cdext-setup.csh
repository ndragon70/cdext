
setenv CD_CSH 1
setenv PROMPT_STRING  'set prompt="`/usr/bin/pwd` \! => "'
setenv CD_HOME        $HOME
setenv CD_SHELL_TYPE  csh
setenv CD_SIZE        10
setenv CD_STYLE       nondestructive
setenv CD_PATH_FORMAT logical

set SILENT=FALSE

while ( "$1" != "" )
    switch( $1 )
	case -size    :
	    shift
	    set CD_SIZE=$1
	    breaksw
	case -style   :
	    shift
	    set CD_STYLE=$1
	    breaksw
	case -version :
	    shift
	    set CD_VERSION=$1
	    breaksw
	case -home :
	    shift
	    setenv CD_HOME=$1
	    breaksw
	case -bin     :
	    shift
	    set CD_BIN=$1
	    breaksw
	case -prompt  :
	    shift
	    set PROMPT_STRING=$1
	    breaksw
	case -noprompt  :
	    set PROMPT_STRING=""
	    breaksw
	case -silent :
	    set SILENT=TRUE
	    breaksw
	default       :
	    breaksw
    endsw
    
    shift

end

if ($SILENT == FALSE) echo "$CD_SHELL_TYPE-$CD_VERSION :: Configuring cd extensions  ..."

setenv EXECUTE_AFTER_CD "$PROMPT_STRING"

if ($SILENT == FALSE) echo "$CD_SHELL_TYPE-$CD_VERSION :: Creating aliases using $CD_BIN directory ..."

# original cd extension commands
alias setcd  	eval '`$CD_BIN/setcd-$CD_SHELL_TYPE-$CD_VERSION  \!*`'
alias save   	eval '`$CD_BIN/save-$CD_SHELL_TYPE-$CD_VERSION   \!*`'
alias delete 	eval '`$CD_BIN/delete-$CD_SHELL_TYPE-$CD_VERSION \!*`'
alias cd     	eval '`$CD_BIN/ccd-$CD_SHELL_TYPE-$CD_VERSION    \!*`; eval $EXECUTE_AFTER_CD'
alias list            '$CD_BIN/list-$CD_SHELL_TYPE-$CD_VERSION   \!* | cat'


# ood philosophy commands
alias cd.set    eval '`$CD_BIN/setcd-$CD_SHELL_TYPE-$CD_VERSION  \!*`'
alias cd.se	cd.set
alias cd.save   eval '`$CD_BIN/save-$CD_SHELL_TYPE-$CD_VERSION   \!*`'
alias cd.sav	cd.save
alias cd.sa	cd.save
alias cd.s	cd.save
alias cd.delete eval '`$CD_BIN/delete-$CD_SHELL_TYPE-$CD_VERSION \!*`'
alias cd.delet	cd.delete
alias cd.dele	cd.delete
alias cd.del	cd.delete
alias cd.de	cd.delete
alias cd.d	cd.delete
alias cd.list         '$CD_BIN/list-$CD_SHELL_TYPE-$CD_VERSION   \!* | cat'
alias cd.lis	cd.list
alias cd.li	cd.list
alias cd.l	cd.list


if ($SILENT == FALSE) then

echo "$CD_SHELL_TYPE-$CD_VERSION :: Setting Environment Variables ..."

cd.set

echo "$CD_SHELL_TYPE-$CD_VERSION :: Setting stack size = $CD_SIZE"

cd.set stack size $CD_SIZE

echo "$CD_SHELL_TYPE-$CD_VERSION :: Setting stack style $CD_STYLE"

cd.set stack style $CD_STYLE

echo "$CD_SHELL_TYPE-$CD_VERSION :: Setting path format $CD_PATH_FORMAT"

cd.set path format $CD_PATH_FORMAT

echo "$CD_SHELL_TYPE-$CD_VERSION :: Done."

else

cd.set
cd.set stack size $CD_SIZE
cd.set stack style $CD_STYLE
cd.set path format $CD_PATH_FORMAT

endif
