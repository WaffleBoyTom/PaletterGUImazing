export PALETTER_DIR="${HOME}/Documents/PaletterGUImazing/PaletterGUImazing"
export PALETTER_SRC="${PALETTER_DIR}/src"
# refer to https://tldp.org/HOWTO/Bash-Prompt-HOWTO/x329.html for some tips on how to customize PS1
export PS1='\[\033[1;33m\]${PWD/*\//}: '
alias START_PALETTER="${PALETTER_DIR}/bin/PaletterGUImazing"
alias BUILD_PALETTER="cd ${PALETTER_DIR}/build/Desktop_Qt_6_8_1-Debug && cmake --build ."
alias SIMPLE_BUILD_PALETTER="cmake --build ."
alias BUILD_AND_RUN_PALETTER="BUILD_PALETTER && START_PALETTER"
