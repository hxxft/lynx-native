// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.apt.codegen.jscomponent;

/*
public class PageNavigatorJSObject extends ExtLynxFunctionObject {

    public PageNavigatorJSObject(JSComponent component) {
        super(component);
    }


    @Override
    protected void initWithReceiver(Object receiver) {
        mMethodNameArray.add("startNewPage");
    }


    @Override
    public Object exec(String methodName, Object[] args) {
        if ("startNewPage".equals(methodName)) {
            ((PageNavigator) mComponent).startNewPage((String) args[0]);
        }
        return super.exec(methodName, args);
    }
}

*/

import com.lynx.apt.codegen.ComponentSpec;
import com.lynx.apt.codegen.ParameterGenUtil;
import com.lynx.modules.ext.ExtLynxFunctionObject;
import com.lynx.modules.ext.JSComponent;
import com.squareup.javapoet.CodeBlock;
import com.squareup.javapoet.MethodSpec;
import com.squareup.javapoet.TypeSpec;
import java.util.List;
import javax.lang.model.element.ExecutableElement;
import javax.lang.model.element.Modifier;
import static com.lynx.apt.codegen.ParameterGenUtil.OBJECT_ARRAY_TYPE;

public class JSComponentGenUtil {

    public static TypeSpec genJSComponentClass(ComponentSpec componentSpec) {
        TypeSpec.Builder builder = TypeSpec.classBuilder(componentSpec.genClassName);
        builder.superclass(ExtLynxFunctionObject.class);
        builder.addModifiers(Modifier.PUBLIC);

        builder.addMethod(genConstruct());
        builder.addMethod(genInitWithReceiver(componentSpec.methodList));
        builder.addMethod(genExec(componentSpec.methodList, componentSpec.fullAnnotationClassName));
        return builder.build();
    }

    public static MethodSpec genConstruct() {
        MethodSpec.Builder method = MethodSpec.constructorBuilder();
        method.addModifiers(Modifier.PUBLIC);
        method.addParameter(JSComponent.class, "component");
        method.addStatement("super(component)");
        return method.build();
    }

    public static MethodSpec genInitWithReceiver(List<ExecutableElement> methodSpecList) {
        MethodSpec.Builder methodSpec = MethodSpec.methodBuilder("initWithReceiver");
        methodSpec.addModifiers(Modifier.PUBLIC);
        methodSpec.addParameter(Object.class, "receiver");
        methodSpec.addAnnotation(Override.class);
        methodSpec.addStatement("mReceiver = receiver");
        methodSpec.addStatement("mMethodNameArray = new com.lynx.core.base.LynxArray()");
        for (ExecutableElement spec : methodSpecList) {
            methodSpec.addStatement("mMethodNameArray.add($S)", spec.getSimpleName().toString());
        }
        return methodSpec.build();
    }

    public static MethodSpec genExec(List<ExecutableElement> methodSpecList, String type) {
        MethodSpec.Builder methodSpec = MethodSpec.methodBuilder("exec");
        methodSpec.returns(Object.class);
        methodSpec.addModifiers(Modifier.PUBLIC);
        methodSpec.addParameter(String.class, "methodName");
        methodSpec.addParameter(Object[].class, "args");

        for (ExecutableElement spec : methodSpecList) {
            CodeBlock.Builder builder = CodeBlock.builder();
            builder.beginControlFlow("if($S.equals(methodName))", spec.getSimpleName().toString());
            String paramString = ParameterGenUtil.genParameterStr("args",
                    spec.getParameters(), OBJECT_ARRAY_TYPE);
            CodeBlock.Builder typeCheckBlock = ParameterGenUtil.genConditionStr("args",
                    spec.getParameters(),OBJECT_ARRAY_TYPE);
            if (!"void".equals(spec.getReturnType().toString())) {
                typeCheckBlock.addStatement("return (($L)mComponent).$L($L)"
                        , type, spec.getSimpleName().toString(), paramString);
            } else {
                typeCheckBlock.addStatement("(($L)mComponent).$L($L)"
                        , type, spec.getSimpleName().toString(), paramString);
                typeCheckBlock.addStatement("return null");
            }

            if ( spec.getParameters().size() > 0) {
                typeCheckBlock.endControlFlow();
                typeCheckBlock.endControlFlow();
            }
            builder.add(typeCheckBlock.build());
            builder.endControlFlow();
            methodSpec.addCode(builder.build());
        }
        methodSpec.addStatement("return super.exec(methodName, args)");
        return methodSpec.build();
    }

}
