CD_HOME=$HOME                                   ; export CD_HOME
CD_SIZE=10                                      ; export CD_SIZE
CD_STYLE=nondestructive                         ; export CD_STYLE
CD_PATH_FORMAT=logical                          ; export CD_PATH_FORMAT
CD_BIN=/usr/loca/bin/cdext

# sh does not support alias or cd.<> function names
cd        () { COMMAND="`$CD_BIN ccd $*`"; eval $COMMAND; }
delete    () { COMMAND="`$CD_BIN delete $*`"; eval $COMMAND; }
list      () { $CD_BIN list $*; }
save      () { COMMAND="`$CD_BIN save $*`"; eval $COMMAND; }
setcd     () { COMMAND="`$CD_BIN setcd $*`"; eval $COMMAND; }

setcd
setcd stack size  $CD_SIZE
setcd stack style $CD_STYLE
setcd path format $CD_PATH_FORMAT
