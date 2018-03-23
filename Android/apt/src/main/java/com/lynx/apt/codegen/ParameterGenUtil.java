// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.apt.codegen;

import com.squareup.javapoet.CodeBlock;
import java.util.List;
import javax.lang.model.element.VariableElement;

public class ParameterGenUtil {

    public static final int OBJECT_ARRAY_TYPE=0;
    public static final int LYNX_LIST_TYPE=1;

    public static String genParameterStr(String argsName,
                                         List<? extends VariableElement> params,int type) {
        if (params != null && params.size() > 0) {
            StringBuilder stringBuilder = new StringBuilder();
            String getItemString = getItemByType(type);
            for (int i = 0; i < params.size(); i++) {
                stringBuilder.append(String.format("(%s)"+getItemString,
                        params.get(i).asType().toString(), argsName, i));
                if (i != params.size() - 1) {
                    stringBuilder.append(", ");
                }
            }
            return stringBuilder.toString();
        }
        return "";
    }

    public static CodeBlock.Builder genConditionStr(String argsName,
                                                    List<? extends VariableElement> params,
                                                    int type) {
        CodeBlock.Builder codeBlock = CodeBlock.builder();
        String getItemString = getItemByType(type);
        String lengthStr = getLengthEqualByType(type);
        if (params != null && params.size() > 0) {
            codeBlock.beginControlFlow("if($L!=null & "+lengthStr+")",
                    argsName, argsName, params.size());
            StringBuilder stringBuilder = new StringBuilder();
            for (int i = 0; i < params.size(); i++) {
                stringBuilder.append(String.format(getItemString+" instanceof %s", argsName,
                        i, typeStringConvert(params.get(i).asType().toString())));
                if (i != params.size() - 1) {
                    stringBuilder.append(" & ");
                }
            }
            codeBlock.beginControlFlow("if($L)",stringBuilder.toString());
        }

        return codeBlock;
    }

    private static String typeStringConvert(String type) {
        switch (type) {
            case "int":
                return "java.lang.Integer";
            case "char":
                return "java.lang.Character";
            case "long":
                return "java.lang.Long";
            case "float":
                return "java.lang.Float";
            case "boolean":
                return "java.lang.Boolean";
            case "double":
                return "java.lang.Double";
            default:
                return type;
        }
    }

    private static String getLengthEqualByType(int type){
        String getItemStr = "";
        switch (type){
            case OBJECT_ARRAY_TYPE:
                getItemStr= "$L.length == $L";
                break;
            case LYNX_LIST_TYPE:
                getItemStr= "$L.length() == $L";
                break;
        }
        return getItemStr;
    }

    private static String getItemByType(int type){
        String getLetEqualStr = "";
        switch (type){
            case OBJECT_ARRAY_TYPE:
                getLetEqualStr= "%s[%d]";
                break;
            case LYNX_LIST_TYPE:
                getLetEqualStr= "%s.get(%d)";
                break;
        }
        return getLetEqualStr;
    }
}
