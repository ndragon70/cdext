
setenv CD_CSH 1
setenv CD_HOME        $HOME
setenv CD_SIZE        10
setenv CD_STYLE       nondestructive
setenv CD_PATH_FORMAT logical
setenv CD_BIN /usr/local/bin/cdext

alias setcd  	eval '`$CD_BIN setcd \!*`'
alias save   	eval '`$CD_BIN save \!*`'
alias delete 	eval '`$CD_BIN delete \!*`'
alias cd     	eval '`$CD_BIN ccd \!*`'
alias list            '$CD_BIN list \!*'

alias cd.set    eval '`$CD_BIN setcd \!*`'
alias cd.se	cd.set
alias cd.save   eval '`$CD_BIN save \!*`'
alias cd.sav	cd.save
alias cd.sa	cd.save
alias cd.s	cd.save
alias cd.delete eval '`$CD_BIN delete \!*`'
alias cd.delet	cd.delete
alias cd.dele	cd.delete
alias cd.del	cd.delete
alias cd.de	cd.delete
alias cd.d	cd.delete
alias cd.list   '$CD_BIN list \!*'
alias cd.lis	cd.list
alias cd.li	cd.list
alias cd.l	cd.list

cd.set
cd.set stack size $CD_SIZE
cd.set stack style $CD_STYLE
cd.set path format $CD_PATH_FORMAT
