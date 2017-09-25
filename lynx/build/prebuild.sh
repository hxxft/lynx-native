#!/bin/bash
CURRENT_PATH=$(cd `dirname $0`; pwd)
ROOT_LYNX_JAVA_PATH=$CURRENT_PATH"/../../Android/sdk/src/main/java/"
LYNX_OUTPUT_DIR=$CURRENT_PATH"/../gen/"
LYNX_GEN_FILE=$CURRENT_PATH"/jni_generator.py"
while read line
do
	file_name=${line##*/}
    jni_file_name=${file_name%.*}"_jni.h"
    input_file=$ROOT_LYNX_JAVA_PATH$line
    output_file=$LYNX_OUTPUT_DIR$jni_file_name
    python $LYNX_GEN_FILE $input_file $output_file
echo "python $LYNX_GEN_FILE $input_file $output_file"
done < $CURRENT_PATH"/jni_files"
