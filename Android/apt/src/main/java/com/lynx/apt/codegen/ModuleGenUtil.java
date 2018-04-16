// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.apt.codegen;

import com.lynx.core.LynxRuntime;
import com.lynx.modules.ext.LynxModule;
import com.lynx.modules.ext.LynxUIComponent;
import com.squareup.javapoet.MethodSpec;
import com.squareup.javapoet.ParameterizedTypeName;
import com.squareup.javapoet.TypeSpec;
import java.util.List;
import java.util.Map;
import javax.lang.model.element.Modifier;

/**
 * public class LynxExtModule extends ExtModule {
 *
 * @Override public Map<Integer, LynxUIComponent> createComponents() {
 * HashMap<Integer, LynxUIComponent> map = new HashMap<>();
 * LynxUIComponent component = new LynxLabelComponent();
 * component.collectMethodSpecList();
 * map.put(2, component);
 * return map;
 * }
 * }
 */
public class ModuleGenUtil {
    public static TypeSpec genModuleClass(ModuleSpec moduleSpec) {
        TypeSpec.Builder classSpec = TypeSpec.classBuilder(moduleSpec.moduleName)
                .addModifiers(Modifier.PUBLIC).superclass(LynxModule.class);
        if (moduleSpec.uiComponentSpecs != null && moduleSpec.uiComponentSpecs.size() > 0) {
            MethodSpec createComponents = genCreateUIComponents(moduleSpec.uiComponentSpecs);
            classSpec.addMethod(createComponents);
        }
        if (moduleSpec.jsComponentSpecs != null && moduleSpec.jsComponentSpecs.size() > 0) {
            MethodSpec genJSComponent = genRegisterFunctionObject(moduleSpec.jsComponentSpecs);
            classSpec.addMethod(genJSComponent);
        }
        return classSpec.build();
    }

    private static MethodSpec genCreateUIComponents(List<ComponentSpec> componentSpecList) {
        MethodSpec.Builder createComponents = MethodSpec.methodBuilder("createUIComponents")
                .addAnnotation(Override.class).addModifiers(Modifier.PUBLIC)
                .returns(ParameterizedTypeName.get(Map.class,
                        Integer.class, LynxUIComponent.class));
        createComponents
                .addStatement("java.util.HashMap<Integer, LynxUIComponent> map = new java.util.HashMap<>()");
        for (int i = 0; i < componentSpecList.size(); i++) {
            ComponentSpec m = componentSpecList.get(i);
            String fullClassPath = m.packageName + "." + m.genClassName;
            createComponents.addStatement("$L component$L = new $L()",
                    fullClassPath, i, fullClassPath);
            createComponents.addStatement("component$L.registerMethodSpecList()", i);
            createComponents.addStatement("map.put($L, component$L)", m.type, i);
            createComponents.addStatement("com.lynx.utils.RegisterUtil.nativeRegisterTag($L,$S)", m.type, m.tagName);
        }
        createComponents.addStatement("return map");
        return createComponents.build();
    }

    private static MethodSpec genRegisterFunctionObject(List<ComponentSpec> componentSpecList) {
        MethodSpec.Builder registerFunctionObject =
                MethodSpec.methodBuilder("registerFunctionObject")
                .addAnnotation(Override.class).addModifiers(Modifier.PUBLIC)
                .addParameter(LynxRuntime.class, "runtime");
        for (int i = 0; i < componentSpecList.size(); i++) {
            ComponentSpec m = componentSpecList.get(i);
            String fullClassPath = m.packageName + "." + m.genClassName;
            registerFunctionObject.addStatement("$L component$L = new $L(new $L(runtime))",
                    fullClassPath, i, fullClassPath, m.fullAnnotationClassName);
            registerFunctionObject.addStatement("runtime.registerModule(component$L, $S)",
                    i, m.jsObjectName);
        }
        return registerFunctionObject.build();
    }
}
