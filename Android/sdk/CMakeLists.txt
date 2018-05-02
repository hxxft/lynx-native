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
include_directories(${CMAKE_SOURCE_DIR}/../../Core/third_party/googletest/include
                    ${CMAKE_SOURCE_DIR}/../../Core/third_party/googletest)

set(UNITTEST_SRC_LIST
        ${CMAKE_SOURCE_DIR}/../../Core/third_party/googletest/src/gtest-all.cc
        ${CMAKE_SOURCE_DIR}/../../Core/test/gtest_driver.cc
        ${CMAKE_SOURCE_DIR}/../../Core/base/scoped_ptr_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../Core/base/ref_counted_ptr_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../Core/base/weak_ptr_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../Core/base/vector2d_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../Core/base/position_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../Core/base/string/string_number_convert_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../Core/parser/render_tokenizer_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../Core/layout/css_type_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../Core/layout/css_color_unittest.cc
        ${CMAKE_SOURCE_DIR}/../../Core/layout/css_style_unittest.cc)

endif()

include_directories(${CMAKE_SOURCE_DIR}/../../Core
                    ${CMAKE_SOURCE_DIR}/../../Core/gen
                    ${CMAKE_SOURCE_DIR}/../../Core/base/android
                    #${CMAKE_SOURCE_DIR}/../../Core/runtime/v8
                    #${CMAKE_SOURCE_DIR}/../../Core/runtime/v8/include
                    #${CMAKE_SOURCE_DIR}/../../Core/v8
                    #${CMAKE_SOURCE_DIR}/../../Core/v8/include
                    ${CMAKE_SOURCE_DIR}/../../Core/third_party/jsoncpp/src/lib_json
                    ${CMAKE_SOURCE_DIR}/../../Core/third_party/jsoncpp/include
                    ${CMAKE_SOURCE_DIR}/../../Core/third_party/JavaScriptCore
                    ${CMAKE_SOURCE_DIR}/../../Core/lepus
                   )

aux_source_directory(${CMAKE_SOURCE_DIR}/../../Core/lepus
                     LEPUS_SOURCE_FILES)

add_library(lynx SHARED
                ${CMAKE_SOURCE_DIR}/../../Core/base/android/java_type.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/android/lynx_java_type.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/android/convert.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/android/android_jni.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/android/params_transform.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/android/jni_helper.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/android/scoped_java_ref.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/base64.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/debug/memory_debug.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/debug/memory_tracker.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/debug/timing_tracker.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/log/logging.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/observer/observer_list.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/poller/select_poller.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/poller/utility.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/string/string_number_convert.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/string/string_utils.cc
                ${CMAKE_SOURCE_DIR}/../../Core/base/threading/thread.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/threading/message_loop.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/threading/message_pump_posix.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/threading/message_pump_android.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/threading/message_pump_io_posix.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/threading/thread_local_posix.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/trace_event/trace_logger.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/trace_event/trace_writer.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/timer/timer.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/timer/timer_heap.cc
            	${CMAKE_SOURCE_DIR}/../../Core/base/timer/time_utils.cc
				${CMAKE_SOURCE_DIR}/../../Core/base/io_buffer.cc	
				${CMAKE_SOURCE_DIR}/../../Core/base/rand_util.cc	
            	${CMAKE_SOURCE_DIR}/../../Core/build/jni_load.cc
            	${CMAKE_SOURCE_DIR}/../../Core/config/global_config_data.cc
				${CMAKE_SOURCE_DIR}/../../Core/debugger/android/debug_host_impl.cc
				${CMAKE_SOURCE_DIR}/../../Core/debugger/debug_client.cc
				${CMAKE_SOURCE_DIR}/../../Core/debugger/debug_session.cc
            	#${CMAKE_SOURCE_DIR}/../../Core/render/test/demo.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/event_target.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/render_object.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/render_tree_host.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/render_tree_host_impl.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/view_stub.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/view.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/body.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/label.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/input.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/swiper_view.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/extended_view.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/canvas_view.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/image_view.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/list_view.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/cell_view.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/text_area.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/scroll_view.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/list_shadow.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/animation.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/android/render_object_impl_android.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/android/label_measurer_android.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/android/render_tree_host_impl_android.cc
            	#${CMAKE_SOURCE_DIR}/../../Core/render/optimize/optimized_render_object.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/impl/render_object_impl.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/impl/render_command.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/impl/command_collector.cc
            	${CMAKE_SOURCE_DIR}/../../Core/layout/css_color.cc
            	${CMAKE_SOURCE_DIR}/../../Core/layout/css_style.cc
            	${CMAKE_SOURCE_DIR}/../../Core/layout/css_layout.cc
            	${CMAKE_SOURCE_DIR}/../../Core/layout/css_type.cc
            	${CMAKE_SOURCE_DIR}/../../Core/layout/container_node.cc
                ${CMAKE_SOURCE_DIR}/../../Core/layout/layout_object.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/android/jni_runtime_bridge.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/android/element_register_util.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/android/result_callback.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/base/lynx_array.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/base/lynx_object.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/base/lynx_value.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/base/lynx_function_object.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/base/lynx_object_template.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/base/lynx_function_object_android.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/loader.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/runtime.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/js_context.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/js_vm.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/navigator.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/screen.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/console.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/document.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/canvas.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/canvas_2d_context.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/element.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/timed_task.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/js_event.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/jsc_helper.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/jsc_context.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/js_value_ext.cc
            	#${CMAKE_SOURCE_DIR}/../../Core/runtime/v8/simple_allocator.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/jsc_function.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/timeout_callback.cc
            	#${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/params_transform.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/objects/window_object.cc
            	${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/objects/object_template.cc
                ${CMAKE_SOURCE_DIR}/../../Core/runtime/jsc/objects/function_object.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/base/net_errors.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/base/net_errors_posix.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/socket/socket.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/socket/socket_posix.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/websocket/websocket_errors.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/websocket/websocket_frame.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/websocket/websocket_frame_parser.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/websocket/websocket_hand_shake.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/websocket/websocket_stream.cc
				${CMAKE_SOURCE_DIR}/../../Core/net/websocket/websocket.cc
            	${CMAKE_SOURCE_DIR}/../../Core/net/url_request_context.cc
            	${CMAKE_SOURCE_DIR}/../../Core/net/url_request_ghost_delegate.cc
            	${CMAKE_SOURCE_DIR}/../../Core/net/android/url_request_android.cc
            	${CMAKE_SOURCE_DIR}/../../Core/loader/html/html_loader.cc
            	${CMAKE_SOURCE_DIR}/../../Core/loader/html/html_request_delegate.cc
            	${CMAKE_SOURCE_DIR}/../../Core/loader/script/script_loader.cc
            	${CMAKE_SOURCE_DIR}/../../Core/loader/script/script_request_delegate.cc
            	${CMAKE_SOURCE_DIR}/../../Core/loader/cache/source_cache.cc
            	${CMAKE_SOURCE_DIR}/../../Core/loader/cache/source_cache_manager.cc

            	${CMAKE_SOURCE_DIR}/../../Core/parser/input_stream.cc
            	${CMAKE_SOURCE_DIR}/../../Core/parser/render_parser.cc
            	${CMAKE_SOURCE_DIR}/../../Core/parser/render_token.cc
            	${CMAKE_SOURCE_DIR}/../../Core/parser/render_tokenizer.cc
            	${CMAKE_SOURCE_DIR}/../../Core/parser/tree_builder.cc
            	${CMAKE_SOURCE_DIR}/../../Core/parser/style_parser.cc

            	${CMAKE_SOURCE_DIR}/../../Core/third_party/jsoncpp/src/lib_json/json_reader.cpp
            	${CMAKE_SOURCE_DIR}/../../Core/third_party/jsoncpp/src/lib_json/json_value.cpp
            	${CMAKE_SOURCE_DIR}/../../Core/third_party/jsoncpp/src/lib_json/json_writer.cpp

            	${CMAKE_SOURCE_DIR}/../../Core/render/android/jni_coordinator_bridge.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/coordinator/coordinator_action.cc
            	${CMAKE_SOURCE_DIR}/../../Core/render/coordinator/coordinator_executor.cc

            	${LEPUS_SOURCE_FILES}
                ${UNITTEST_SRC_LIST}
            	)



target_link_libraries(lynx
                      android
                      log
                      z
                      ${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libjsc.so
                      #${CMAKE_SOURCE_DIR}/../../Core/v8/libv8.cr.so
                      #${CMAKE_SOURCE_DIR}/../../Core/v8/libv8_libbase.cr.so
                      #${CMAKE_SOURCE_DIR}/../../Core/v8/libv8_libplatform.cr.so
                      jnigraphics
                      )

#target_link_libraries(lynx
#                      android
#                      log
#                      z
#                      ${CMAKE_SOURCE_DIR}/../../Core/runtime/v8/libs/libv8_base.a
#                      ${CMAKE_SOURCE_DIR}/../../Core/runtime/v8/libs/libv8_libplatform.a
#                      ${CMAKE_SOURCE_DIR}/../../Core/runtime/v8/libs/libv8_libbase.a
#                      ${CMAKE_SOURCE_DIR}/../../Core/runtime/v8/libs/libv8_nosnapshot.a
#                      )
