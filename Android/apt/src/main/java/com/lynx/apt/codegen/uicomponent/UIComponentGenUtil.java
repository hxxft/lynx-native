// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.apt.codegen.uicomponent;

import android.content.Context;
import com.lynx.apt.codegen.ComponentSpec;
import com.lynx.apt.codegen.ParameterGenUtil;
import com.lynx.core.annotation.JSMethod;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.modules.ext.LynxUIComponent;
import com.lynx.ui.LynxUI;
import com.squareup.javapoet.CodeBlock;
import com.squareup.javapoet.MethodSpec;
import com.squareup.javapoet.TypeSpec;
import java.util.List;
import javax.lang.model.element.ExecutableElement;
import javax.lang.model.element.Modifier;

/**
 * public class LynxLabelComponent extends LynxUIComponent {
 *
 * @Override public LynxUI createExtLynxUI(Context context, RenderObjectImpl impl) {
 * return null;
 * }
 * @Override public void registerMethodSpecList() {
 * MethodRegisterUtil.nativeRegisterJSMethod(2, "t3", 10);
 * }
 * @Override public void dispatchJSMethodWithId(LynxUI ui, int id, Object param) {
 * if (id == 10) {
 * ((LynxUILabel) ui).testCall(param);
 * }
 * }
 */

public class UIComponentGenUtil {
    public static TypeSpec genComponentClass (ComponentSpec componentSpec) {
        TypeSpec.Builder builder = TypeSpec.classBuilder(componentSpec.genClassName)
                .addModifiers(Modifier.PUBLIC).superclass(LynxUIComponent.class);
        builder.addMethod(genCreateExtLynxUI(componentSpec.fullAnnotationClassName));
        builder.addMethod(genRegisterMethodSpecList(componentSpec.type, componentSpec.methodList));
        builder.addMethod(genDispatchJSMethodWithId(componentSpec.fullAnnotationClassName,
                componentSpec.methodList));
        return builder.build();
    }

    static MethodSpec genCreateExtLynxUI(String classTypeString) {

        MethodSpec.Builder createLynxUIMethod = MethodSpec.methodBuilder("createExtLynxUI")
                .addModifiers(Modifier.PUBLIC).addAnnotation(Override.class).returns(LynxUI.class)
                .addParameter(Context.class, "context")
                .addParameter(RenderObjectImpl.class, "impl");

        CodeBlock.Builder codeBlock = CodeBlock.builder();
        codeBlock.addStatement("return new $L($L,$L)", classTypeString, "context", "impl");
        createLynxUIMethod.addCode(codeBlock.build());
        return createLynxUIMethod.build();
    }

    static MethodSpec genRegisterMethodSpecList(int type, List<ExecutableElement> methodSpecs) {
        MethodSpec.Builder collectMethodSpecList = MethodSpec.methodBuilder("registerMethodSpecList")
                .addModifiers(Modifier.PUBLIC).addAnnotation(Override.class).returns(void.class);
        CodeBlock.Builder codeBlock = CodeBlock.builder();
        for (ExecutableElement m : methodSpecs) {
            int annotationId = m.getAnnotation(JSMethod.class).id();
            String methodName = m.getSimpleName().toString();
            int id = annotationId == 0 ? methodName.hashCode()
            : annotationId;
            codeBlock.addStatement("com.lynx.utils.RegisterUtil.nativeRegisterJSMethod($L,$S,$L)",
                    type, m.getSimpleName().toString(),  id);
        }
        collectMethodSpecList.addCode(codeBlock.build());
        return collectMethodSpecList.build();
    }

    static MethodSpec genDispatchJSMethodWithId(String classTypeString, List<ExecutableElement> methodSpecs) {
        MethodSpec.Builder collectMethodSpecList = MethodSpec.methodBuilder("dispatchJSMethodWithId")
                .addModifiers(Modifier.PUBLIC).addAnnotation(Override.class).returns(void.class)
                .addParameter(LynxUI.class, "ui").addParameter(int.class, "id").addParameter(Object.class, "param");
        CodeBlock.Builder codeBlock = CodeBlock.builder();
        for (ExecutableElement m : methodSpecs) {
            CodeBlock.Builder controlInvokeBlock = CodeBlock.builder();
            if (m.getParameters()!=null && m.getParameters().size() > 0) {
                controlInvokeBlock
                        .addStatement("com.lynx.core.base.LynxArray args =(com.lynx.core.base.LynxArray) param");
                String params = ParameterGenUtil.genParameterStr("args"
                        ,m.getParameters(),ParameterGenUtil.LYNX_LIST_TYPE);
                CodeBlock.Builder typeCheckCode = ParameterGenUtil.genConditionStr("args"
                        ,m.getParameters(),ParameterGenUtil.LYNX_LIST_TYPE);
                typeCheckCode.addStatement("(($L) ui).$L($L)", classTypeString,
                        m.getSimpleName().toString(),params);
                typeCheckCode.endControlFlow();
                typeCheckCode.endControlFlow();
                controlInvokeBlock.add(typeCheckCode.build());
            } else {
                controlInvokeBlock.addStatement("(($L) ui).$L()",
                        classTypeString, m.getSimpleName().toString());
            }
            int annotationId = m.getAnnotation(JSMethod.class).id();
            String methodName = m.getSimpleName().toString();
            int id = annotationId == 0 ? methodName.hashCode()
                    : annotationId;

            codeBlock.beginControlFlow("if(id == $L)", id)
                    .add(controlInvokeBlock.build())
                    .endControlFlow();
        }
        collectMethodSpecList.addCode(codeBlock.build());
        return collectMethodSpecList.build();
    }
}
