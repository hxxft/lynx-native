cmake_minimum_required(VERSION 3.4.1)

add_compile_options(-std=c++11)
add_compile_options(-fexceptions)

add_definitions(-DUSING_V8=0)
add_definitions(-DOS_ANDROID=1)
add_definitions(-DGNU_SUPPORT=1)
add_definitions(-DJSONCPP_NO_LOCALE_SUPPORT=1)
add_definitions(-DDEBUG_MEMORY=0)
add_definitions(-DCOMPILER_GCC=1)
add_definitions(-DENABLE_TRACING=0)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

MESSAGE(${CMAKE_BUILD_TYPE})


if (CMAKE_BUILD_TYPE MATCHES "Release")

# strip lib
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
set(UNITTEST_SRC_LIST "")
add_definitions(-DGTEST_ENABLE=0)

else()
add_definitions(-DGTEST_ENABLE=1)
include_directories(${CMAKE_SOURCE_DIR}/../../lynx/third_party/googletest/include
                    ${CMAKE_SOURCE_DIR}/../../lynx/third_party/googletest)

set(UNITTEST_SRC_LIST
        ${CMAKE_SOURCE_DIR}/../../lynx/third_party/googletest/src/gtest-all.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/test/gtest_driver.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/base/scoped_ptr_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/base/ref_counted_ptr_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/base/weak_ptr_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/base/vector2d_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/base/position_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/base/string/string_number_convert_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/parser/render_tokenizer_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/layout/css_type_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/layout/css_color_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../lynx/layout/css_style_unittest.cc)

endif()

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
                    ${CMAKE_SOURCE_DIR}/../../lynx/lepus
                   )

aux_source_directory(${CMAKE_SOURCE_DIR}/../../lynx/lepus
                     LEPUS_SOURCE_FILES)

add_library(lynx SHARED
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/java_type.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/lynx_java_type.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/convert.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/android_jni.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/params_transform.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/jni_helper.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/android/scoped_java_ref.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/base64.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/debug/memory_debug.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/debug/memory_tracker.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/debug/timing_tracker.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/log/logging.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/observer/observer_list.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/poller/select_poller.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/poller/utility.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/string/string_number_convert.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/base/string/string_utils.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/thread.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/message_loop.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/message_pump_posix.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/message_pump_android.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/message_pump_io_posix.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/threading/thread_local_posix.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/trace_event/trace_logger.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/trace_event/trace_writer.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/timer/timer.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/timer/timer_heap.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/base/timer/time_utils.cc
				${CMAKE_SOURCE_DIR}/../../lynx/base/io_buffer.cc	
				${CMAKE_SOURCE_DIR}/../../lynx/base/rand_util.cc	
            	${CMAKE_SOURCE_DIR}/../../lynx/build/jni_load.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/config/global_config_data.cc
            	#${CMAKE_SOURCE_DIR}/../../lynx/render/test/demo.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/event_target.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/render_object.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/render_tree_host.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/render_tree_host_impl.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/view_stub.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/view.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/body.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/label.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/input.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/swiper_view.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/extended_view.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/canvas_view.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/image_view.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/list_view.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/cell_view.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/text_area.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/scroll_view.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/list_shadow.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/animation.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/android/render_object_impl_android.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/android/label_measurer_android.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/android/render_tree_host_impl_android.cc
            	#${CMAKE_SOURCE_DIR}/../../lynx/render/optimize/optimized_render_object.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/impl/render_object_impl.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/impl/render_command.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/impl/command_collector.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/css_color.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/css_style.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/css_layout.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/css_type.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/layout/container_node.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/layout/layout_object.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/android/jni_runtime_bridge.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/android/element_register_util.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/android/result_callback.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_array.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_object.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_value.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_function_object.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_object_template.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/base/lynx_function_object_android.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/loader.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/runtime.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/js_context.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/js_vm.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/navigator.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/screen.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/console.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/document.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/canvas.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/canvas_2d_context.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/element.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/timed_task.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/js_event.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/jsc_helper.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/jsc_context.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/js_value_ext.cc
            	#${CMAKE_SOURCE_DIR}/../../lynx/runtime/v8/simple_allocator.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/jsc_function.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/timeout_callback.cc
            	#${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/params_transform.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/objects/window_object.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/objects/object_template.cc
                ${CMAKE_SOURCE_DIR}/../../lynx/runtime/jsc/objects/function_object.cc
				${CMAKE_SOURCE_DIR}/../../lynx/net/base/net_errors.cc
				${CMAKE_SOURCE_DIR}/../../lynx/net/base/net_errors_posix.cc
				${CMAKE_SOURCE_DIR}/../../lynx/net/socket/socket.cc
				${CMAKE_SOURCE_DIR}/../../lynx/net/socket/socket_posix.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/net/url_request_context.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/net/url_request_ghost_delegate.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/net/android/url_request_android.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/html/html_loader.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/html/html_request_delegate.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/script/script_loader.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/script/script_request_delegate.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/cache/source_cache.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/loader/cache/source_cache_manager.cc

            	${CMAKE_SOURCE_DIR}/../../lynx/parser/input_stream.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/render_parser.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/render_token.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/render_tokenizer.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/tree_builder.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/parser/style_parser.cc

            	${CMAKE_SOURCE_DIR}/../../lynx/third_party/jsoncpp/src/lib_json/json_reader.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/third_party/jsoncpp/src/lib_json/json_value.cpp
            	${CMAKE_SOURCE_DIR}/../../lynx/third_party/jsoncpp/src/lib_json/json_writer.cpp

            	${CMAKE_SOURCE_DIR}/../../lynx/render/android/jni_coordinator_bridge.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/coordinator/coordinator_action.cc
            	${CMAKE_SOURCE_DIR}/../../lynx/render/coordinator/coordinator_executor.cc

            	${LEPUS_SOURCE_FILES}
                ${UNITTEST_SRC_LIST}
            	)



target_link_libraries(lynx
                      android
                      log
                      z
                      ${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libjsc.so
                      #${CMAKE_SOURCE_DIR}/../../lynx/v8/libv8.cr.so
                      #${CMAKE_SOURCE_DIR}/../../lynx/v8/libv8_libbase.cr.so
                      #${CMAKE_SOURCE_DIR}/../../lynx/v8/libv8_libplatform.cr.so
                      jnigraphics
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
