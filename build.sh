#!/bin/bash

if [ -d target ]; then
   rm -rf target
fi

mkdir target

cp src/main/java/* target/
cp src/main/native/* target/

cd target

export JAVA_HOME="/C/Program Files/Java/jdk1.8.0_131/"
export PATH=$JAVA_HOME/bin:$PATH
export PATH=/C/Users/John/Documents/tcc:$PATH

javac MainWindow.java

javah –jni -cp "." MainWindow
