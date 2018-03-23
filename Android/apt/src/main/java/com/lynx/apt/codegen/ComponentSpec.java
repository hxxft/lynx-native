// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.apt.codegen;

import java.util.List;
import javax.lang.model.element.ExecutableElement;

public class ComponentSpec {
    public int type;
    public String genClassName;
    public String packageName;
    public String fullAnnotationClassName;
    public String jsObjectName;
    public List<ExecutableElement> methodList;
}
