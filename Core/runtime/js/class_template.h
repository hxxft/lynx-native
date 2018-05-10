// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_CLASS_TEMPLATE_H_
#define LYNX_RUNTIME_JSC_CLASS_TEMPLATE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "base/debug/memory_debug.h"
#include "runtime/base/lynx_object.h"
#include "runtime/js/defines.h"

// For Defines DefaultClassTemplate
#define DEFINE_CLASS_TEMPLATE_START(class_name)                                           \
static jscore::ClassTemplate* GenerateDefaultClassTemplate(jscore::JSContext* context) {  \
    jscore::ClassTemplate* class_template = jscore::ClassTemplate::Generate(context, #class_name); \
    if (class_template->is_creation_finished()) return class_template;                    \
    std::string temp;

#define REGISTER_PARENT(parent_class_name)                                                \
    temp = #parent_class_name;                                                            \
    if (!temp.empty()) {                                                                  \
        jscore::ClassTemplateStorage* storage = context->class_template_storage();        \
        jscore::ClassTemplate* parent_class_template = storage->GetClassTemplate(temp);   \
        assert(parent_class_template);                                                    \
        class_template->set_parent_class_template(parent_class_template);                 \
    }

#define EXPOSE_CONSTRUCTOR(enable)                                                        \
    class_template->set_constructor_expose_enable(enable);

#define REGISTER_METHOD_CALLBACK(clazz, c_method_name)                                    \
    temp = #c_method_name;                                                                \
    temp[0] = tolower(temp[0]);                                                           \
    class_template->RegisterMethodCallback(temp, &METHOD_CALLBACK(c_method_name));        \

#define REGISTER_GET_CALLBACK(clazz, field_name)                                          \
    temp = #field_name;                                                                   \
    temp[0] = tolower(temp[0]);                                                           \
    class_template->RegisterAccessorCallback(temp, &GET_CALLBACK(field_name), 0);         \

#define REGISTER_SET_CALLBACK(clazz, field_name)                                          \
    temp = #field_name;                                                                   \
    temp[0] = tolower(temp[0]);                                                           \
    class_template->RegisterAccessorCallback(temp, 0, &SET_CALLBACK(field_name));         \

#define DEFINE_CLASS_TEMPLATE_END                                                         \
    class_template->FinishCreation();                                                     \
    return class_template;                                                                \
}

#define DEFAULT_CLASS_TEMPLATE(JSContext) GenerateDefaultClassTemplate(context)

namespace jscore {

    class ClassTemplateStorage;

    class ClassTemplate {
    public:
        class PrototypeBuilder {
        public:
            virtual void FinishCreation(ClassTemplate* class_template) = 0;
        };

        static PrototypeBuilder* CreatePrototypeBuilder();

        static ClassTemplate* Generate(JSContext* context, const std::string& name);

        void RegisterMethodCallback(const std::string& method_name, JSMethodCallback callback) {
            methods_[method_name] = callback;
        }

        void RegisterAccessorCallback(const std::string& field_name,
                                      JSGetPropertyCallback get_callback,
                                      JSSetPropertyCallback set_callback) {
            Field field;
            if (fields_.find(field_name) != fields_.end()) {
                field = fields_[field_name];
            }
            if (get_callback != 0) {
                field.get_callback = get_callback;
            }
            if (set_callback != 0) {
                field.set_callback = set_callback;
            }
            fields_[field_name] = field;
        }

        void set_class_name(const std::string& class_name) {
            class_name_ = class_name;
        }

        void set_parent_class_template(ClassTemplate *parent_class_template) {
            parent_class_template_ = parent_class_template;
        }

        ~ClassTemplate() {
        }

        PrototypeBuilder* prototype_builder() {
            return prototype_builder_.Get();
        }

        void FinishCreation() {
            if (!is_creation_finish_) {
                prototype_builder_->FinishCreation(this);
                is_creation_finish_ = true;
            }
        }

        inline bool is_creation_finished() {
            return is_creation_finish_;
        }

        inline ClassTemplate* parent_class_template() {
            return parent_class_template_;
        }

        void set_constructor_expose_enable(bool enable) {
            constructor_expose_enable_ = true;
        }

        inline bool constructor_expose_enable() {
            return constructor_expose_enable_;
        }

        inline const std::string &class_name() const {
            return class_name_;
        }

        inline const std::unordered_map<std::string, JSMethodCallback>& methods() const {
            return methods_;
        }

        inline const std::unordered_map<std::string, Field>& fields() const {
            return fields_;
        }

        inline JSContext* context() {
            return context_;
        }

    private:

        ClassTemplate(JSContext *context, const std::string &name)
                : context_(context),
                  methods_(),
                  fields_(),
                  class_name_(),
                  parent_class_template_(NULL),
                  prototype_builder_(CreatePrototypeBuilder()),
                  is_creation_finish_(false),
                  constructor_expose_enable_(false) {

            set_class_name(name);
        }

        JSContext* context_;
        std::unordered_map<std::string, JSMethodCallback> methods_;
        std::unordered_map<std::string, Field> fields_;
        std::string class_name_;
        ClassTemplate* parent_class_template_;
        base::ScopedPtr<PrototypeBuilder> prototype_builder_;
        bool is_creation_finish_;
        bool constructor_expose_enable_;
    };
}

#endif // LYNX_RUNTIME_JSC_CLASS_TEMPLATE_H_
