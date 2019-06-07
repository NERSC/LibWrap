#!/bin/sh
function checkModule(){
  MODULE="swrap"
  if lsmod | grep "$MODULE" &> /dev/null ; then
    echo "$MODULE is loaded!"
    return 0
  else
    echo "$MODULE is not loaded!"
    return 1
  fi
}

checkModule
#if $?; then
#  #do somthing
#fi
