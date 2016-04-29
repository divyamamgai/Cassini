setlocal enabledelayedexpansion
set Directory=%cd%/openssl.cnf
set ReplaceSlash=/
set Directory=%Directory:\=!ReplaceSlash!%
setx OPENSSL_CONF "%Directory%"