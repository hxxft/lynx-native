cmake_minimum_required(VERSION 3.4.1)

add_compile_options(-std=c++11)
add_compile_options(-fexceptions)

add_definitions(-DUSING_V8=0)
add_definitions(-DOS_ANDROID=1)
add_definitions(-DGNU_SUPPORT=1)
add_definitions(-DJSONCPP_NO_LOCALE_SUPPORT=1)
add_definitions(-DDEBUG_MEMORY=0)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# strip lib
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")


include_directories(${CMAKE_SOURCE_DIR}/../../lynx
                    ${CMAKE_SOURCE_DIR}/../../lynx/gen
                    ${CMAKE_SOURCE_DIR}/../../lynx/base/android
                    #${CMAKE_SOURCE_DIR}/../../lynx/runtime/v8
                    #${CMAKE_SOURCE_DIR}/../../lynx/runtime/v8/include
                    #${CMAKE_SOURCE_DIR}/../../lynx/v8
                    #${CMAKE_SOURCE_DIR}/../../lynx/v8/include
                    ${CMAKE_SOURCE_DIR}/../../lynx/third_party/jsoncpp/src/lib_json
                    ${CMAKE_SOURCE_DIR}/../../lynx/third_party/jsoncpp/include
                    ${CMAKE_SOURCE_DIR}/../../lynx/third_party/JavaScriptCore
                   )

add_library(lynx SHARED
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/java_type.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/convert.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/android_jni.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/params_transform.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/jni_helper.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/scoped_java_ref.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/base/debug/memory_debug.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/base/debug/memory_tracker.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/base/observer/observer_list.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/thread.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/message_loop.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/message_pump_posix.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/message_pump_android.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/base/timer/timer.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/base/timer/timer_heap.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/base/timer/time_utils.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/build/jni_load.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/config/global_config_data.cpp
            	#${CMAKE_SOURCE_DIR}/../../lynx/render/test/demo.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/event_target.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/render_object.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/render_tree_host.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/render_tree_host_impl.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/view_stub.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/view.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/body.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/label.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/input.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/swiper_view.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/extended_view.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/image_view.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/list_view.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/cell_view.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/text_area.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/scroll_view.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/list_shadow.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/android/render_object_impl_android.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/android/label_measurer_android.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/android/render_tree_host_impl_android.cpp
            	#${CMAKE_SOURCE_DIR}/../../lynx/render/optimize/optimized_render_object.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/impl/render_object_impl.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/impl/render_command.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/impl/command_collector.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/touch/touch_dispatcher.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/render/touch/touch_event.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/css_color.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/css_style.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/css_layout.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/css_type.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/container_node.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/layout/layout_object.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/android/jni_runtime_bridge.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_array.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_object.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_value.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_function_object.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_object_template.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_function_object_android.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/loader.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/runtime.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/js_context.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/js_vm.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/history.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/location.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/navigator.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/screen.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/console.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/document.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/canvas.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/canvas_2d_context.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/body.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/element.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/timed_task.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/js_event.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/jsc_helper.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/jsc_context.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/jsc_runtime.cpp
            	#${CMAKE_SOURCE_DIR}/../../lynx/runtime/v8/simple_allocator.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/jsc_function.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/timeout_callback.cpp
            	#${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/params_transform.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/objects/window_object.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/objects/object_template.cpp
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/objects/function_object.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/net/url_request_context.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/net/url_request_ghost_delegate.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/net/android/url_request_android.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/html/html_loader.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/html/html_request_delegate.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/script/script_loader.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/script/script_request_delegate.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/cache/source_cache.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/cache/source_cache_manager.cpp

            	${CMAKE_SOURCE_DIR}/../../lynx/parser/input_stream.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/render_parser.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/render_token.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/render_tokenizer.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/tree_builder.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/style_parser.cpp

            	${CMAKE_SOURCE_DIR}/../../lynx/third_party/jsoncpp/src/lib_json/json_reader.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/third_party/jsoncpp/src/lib_json/json_value.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/third_party/jsoncpp/src/lib_json/json_writer.cpp)



target_link_libraries(lynx
                      android
                      log
                      z
                      ${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libjsc.so
                      #${CMAKE_SOURCE_DIR}/../../lynx/v8/libv8.cr.so
                      #${CMAKE_SOURCE_DIR}/../../lynx/v8/libv8_libbase.cr.so
                      #${CMAKE_SOURCE_DIR}/../../lynx/v8/libv8_libplatform.cr.so
                      )

#target_link_libraries(lynx
#                      android
#                      log
#                      z
#                      ${CMAKE_SOURCE_DIR}/../../lynx/runtime/v8/libs/libv8_base.a
#                      ${CMAKE_SOURCE_DIR}/../../lynx/runtime/v8/libs/libv8_libplatform.a
#                      ${CMAKE_SOURCE_DIR}/../../lynx/runtime/v8/libs/libv8_libbase.a
#                      ${CMAKE_SOURCE_DIR}/../../lynx/runtime/v8/libs/libv8_nosnapshot.a
#                      )
