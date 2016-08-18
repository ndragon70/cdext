export CD_HOME=$HOME
export CD_SIZE=10
export CD_STYLE=nondestructive
export CD_PATH_FORMAT=logical
export CB_BIN=/usr/local/bin/cdext

chdir     () { \cd ${*:-}; }
mycd      () { COMMAND="`$CD_BIN ccd ${*:-}`"; eval $COMMAND; }
delete    () { COMMAND="`$CD_BIN delete ${*:-}`"; eval $COMMAND; }
list      () { $CD_BIN list ${*:-}; }
save      () { COMMAND="`$CD_BIN save ${*:-}`"; eval $COMMAND; }
setcd     () { COMMAND="`$CD_BIN setcd ${*:-}`"; eval $COMMAND; }

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

setcd
setcd stack size  $CD_SIZE
setcd stack style $CD_STYLE
setcd path format $CD_PATH_FORMAT
