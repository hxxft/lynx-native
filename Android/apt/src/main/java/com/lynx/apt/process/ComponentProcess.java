// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.apt.process;

import com.google.auto.service.AutoService;
import com.lynx.apt.codegen.ComponentSpec;
import com.lynx.apt.codegen.ModuleGenUtil;
import com.lynx.apt.codegen.ModuleSpec;
import com.lynx.apt.codegen.jscomponent.JSComponentGenUtil;
import com.lynx.apt.codegen.uicomponent.UIComponentGenUtil;
import com.lynx.core.annotation.UIComponent;
import com.lynx.core.annotation.JSObject;
import com.lynx.core.annotation.JSMethod;
import com.squareup.javapoet.JavaFile;
import com.squareup.javapoet.TypeSpec;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import javax.annotation.processing.AbstractProcessor;
import javax.annotation.processing.ProcessingEnvironment;
import javax.annotation.processing.Processor;
import javax.annotation.processing.RoundEnvironment;
import javax.annotation.processing.SupportedSourceVersion;
import javax.lang.model.SourceVersion;
import javax.lang.model.element.Element;
import javax.lang.model.element.ExecutableElement;
import javax.lang.model.element.TypeElement;
import javax.lang.model.util.Elements;

@AutoService(Processor.class)
@SupportedSourceVersion(SourceVersion.RELEASE_7)
public class ComponentProcess extends AbstractProcessor {

    private Elements elementUtils;
    private String packageName;
    private String moduleName;
    private boolean isProcess;

    @Override
    public boolean process(Set<? extends TypeElement> set, RoundEnvironment roundEnvironment) {

        if(isProcess){
            return isProcess;
        }
        isProcess = true;
        Set<? extends Element> uiElements = roundEnvironment.getElementsAnnotatedWith(UIComponent.class);
        ModuleSpec moduleSpec = new ModuleSpec();
        moduleSpec.moduleName = moduleName;
        if (uiElements != null && uiElements.size() > 0) {
            moduleSpec.uiComponentSpecs = uiComponentProcess(uiElements);
        }

        Set<? extends Element> jsElements = roundEnvironment.getElementsAnnotatedWith(JSObject.class);

        if (jsElements != null && jsElements.size() > 0) {
            moduleSpec.jsComponentSpecs = jsComponentProcess(jsElements);
        }


        TypeSpec moduleType = ModuleGenUtil.genModuleClass(moduleSpec);


        JavaFile pageFactoryFile = JavaFile.builder(packageName, moduleType).build();
        try {
            pageFactoryFile.writeTo(processingEnv.getFiler());
        } catch (IOException exception) {
            exception.printStackTrace();
        }
        return isProcess;
    }

    private ArrayList<ComponentSpec> jsComponentProcess(Set<? extends Element> elements) {
        ArrayList<ComponentSpec> componentSpecList = new ArrayList<>();
        for (Element e : elements) {
            TypeElement typeElement = (TypeElement) e;
            ComponentSpec componentSpec = genComponentSpec(typeElement);
            String objectName = typeElement.getAnnotation(JSObject.class).name();
            if("".equals(objectName)){
                componentSpec.jsObjectName = typeElement.getSimpleName().toString();
            }else {
                componentSpec.jsObjectName = objectName;
            }
            componentSpecList.add(componentSpec);

            JavaFile pageFactoryFile = JavaFile.builder(componentSpec.packageName,
                    JSComponentGenUtil.genJSComponentClass(componentSpec)).build();
            try {
                pageFactoryFile.writeTo(processingEnv.getFiler());
            } catch (IOException exception) {
                exception.printStackTrace();
            }
        }
        return componentSpecList;
    }

    private ArrayList<ComponentSpec> uiComponentProcess(Set<? extends Element> elements) {
        ArrayList<ComponentSpec> componentSpecList = new ArrayList<>();
        for (Element e : elements) {
            TypeElement typeElement = (TypeElement) e;
            ComponentSpec componentSpec = genComponentSpec(typeElement);
            componentSpec.type = typeElement.getAnnotation(UIComponent.class).type();
            componentSpecList.add(componentSpec);

            JavaFile pageFactoryFile = JavaFile.builder(componentSpec.packageName,
                    UIComponentGenUtil.genComponentClass(componentSpec)).build();
            try {
                pageFactoryFile.writeTo(processingEnv.getFiler());
            } catch (IOException exception) {
                exception.printStackTrace();
            }
        }
        return componentSpecList;

    }

    private ComponentSpec genComponentSpec(TypeElement typeElement) {
        List<? extends Element> memberElements = elementUtils.getAllMembers(typeElement);
        ArrayList<ExecutableElement> methodSpecs = new ArrayList<>();
        for (Element member : memberElements) {
            JSMethod methodAnnotation = member.getAnnotation(JSMethod.class);
            if (methodAnnotation != null) {
                ExecutableElement executableElement = (ExecutableElement) member;
                methodSpecs.add(executableElement);
            }
        }
        ComponentSpec componentSpec = new ComponentSpec();
        componentSpec.methodList = methodSpecs;
        componentSpec.genClassName = typeElement.getSimpleName() + "Component";
        final String fullClass = typeElement.getQualifiedName().toString();
        componentSpec.packageName = "" + fullClass.substring(0, fullClass.lastIndexOf("."));
        componentSpec.fullAnnotationClassName = fullClass;
        return componentSpec;
    }


    @Override
    public synchronized void init(ProcessingEnvironment processingEnvironment) {
        super.init(processingEnvironment);
        packageName = "com.lynx";
        moduleName = "LynxExtModule";
        if (processingEnvironment.getOptions().size() > 0) {
            if (processingEnvironment.getOptions().get("package") != null) {
                packageName = processingEnvironment.getOptions().get("package");
            }
            if (processingEnvironment.getOptions().get("moduleName") != null) {
                moduleName = processingEnvironment.getOptions().get("moduleName");
            }
        }
        elementUtils = processingEnvironment.getElementUtils();
    }

    @Override
    public Set<String> getSupportedAnnotationTypes() {
        HashSet<String> annotationSet = new HashSet<>();
        annotationSet.add(UIComponent.class.getCanonicalName());
        annotationSet.add(JSObject.class.getCanonicalName());
        return annotationSet;
    }
}
