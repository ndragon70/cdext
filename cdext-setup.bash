export CD_BIN=/usr/local/bin
export CD_PRE=cdext
export CD_HOME=$HOME
export CD_SHELL_TYPE=bash
export CD_SIZE=10
export CD_STYLE=nondestructive
export CD_PATH_FORMAT=logical

# helper functions
_cd()     { eval "$($CD_BIN/cdext-ccd $*)";}
_delete() { eval "$($CD_BIN/cdext-delete $*)";}
_list()   { $CD_BIN/cdext-list $*; }
_save()   { eval "$($CD_BIN/cdext-save $*)";}
_setcd()  { eval "$($CD_BIN/cdext-setcd $*)";}

# aliased commands
alias cd=_cd
alias cd.delete=_delete
alias cd.delet=_delete
alias cd.dele=_delete
alias cd.del=_delete
alias cd.de=_delete
alias cd.d=_delete
alias cd.list=_list
alias cd.lis=_list
alias cd.li=_list
alias cd.l=_list
alias cd.set=_setcd
alias cd.se=_setcd
alias cd.save=_save
alias cd.sav=_save
alias cd.sa=_save
alias cd.s=_save
alias cd.ls="_list stack"

setcd
setcd stack size  $CD_SIZE
setcd stack style $CD_STYLE
setcd path format $CD_PATH_FORMAT
