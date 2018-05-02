// Copyright 2017 The Lynx Authors. All rights reserved.

#include <jni.h>
#include <string>
#include <cstdlib>
#include <unistd.h>

#include <iostream>
#include <iomanip>
#include <thread>

#include <sstream>
#include <android/log.h>

#include "gtest/gtest.h"

#include "test/gtest_driver.h"
#include "GTestDriver_jni.h"

void PumpStdoutToLog() {
    __android_log_print(ANDROID_LOG_INFO, "GTEST_SETUP", "Setting up STDOUT pipe to adb log");
    int stdoutPipe[2];
    pipe(stdoutPipe);
    dup2(stdoutPipe[1], STDOUT_FILENO);
    FILE *exitEndFd = fdopen(stdoutPipe[0], "r");
    std::stringstream outStm;
    int c;

    // It is okay to keep it running like this.
    while (true) {
        c = fgetc(exitEndFd);
        if (c == '\n' || c == EOF) {
            __android_log_print(ANDROID_LOG_ERROR, "APP_STDOUT", "%s", outStm.str().c_str());
            outStm.str("");
        } else {
            outStm << (char) c;
        }
        if (c == EOF) {
            break;
        }
    }
    // TODO - close file handles (as the loop never exits, so the handles get release when the process exits)
}

volatile bool gIsGTestSetup = false;
std::thread* gSTDOutPumpThread = nullptr;

void GTestSetup() {
    if (!gIsGTestSetup) {
        gIsGTestSetup = true;
        gSTDOutPumpThread = new std::thread(PumpStdoutToLog);
        usleep(400);
        fflush(stdout);
    }
}

jint RunGTestsNative(JNIEnv* env,
                    jclass jcaller,
                    jobjectArray gtestCmdLineArgs) {
    // Extract the gtest run params and prepare argc/argv pair
    int argc = env->GetArrayLength(gtestCmdLineArgs);
    char **argv = new char *[argc + 1];
    for (int i = 0; i < argc; i++) {
        jstring string = (jstring) (env->GetObjectArrayElement(gtestCmdLineArgs, i));
        const char *rawString = env->GetStringUTFChars(string, 0);
        argv[i] = new char[strlen(rawString) + 1];
        strcpy(argv[i], rawString);
        env->ReleaseStringUTFChars(string, rawString);
    }

    // Ensure STDOUT -> PIPE -> Log channel is set up.
    GTestSetup();

    // The last parameter is the runId, which is added by the gtest.py. It is used to
    // mark, start and end of the test runs logs in the adb logcat.
    std::string runId(argv[argc - 1]);

    // Put this GTest run instance Start marker
    printf("GTest_Start:%s\n", runId.c_str());

    // Initialize Google Test for the supplied parameters
    ::testing::InitGoogleTest(&argc, argv);
    fflush(stdout);

    // Run the tests.
    int result = RUN_ALL_TESTS();

    // Put this GTest run instance End marker
    printf("GTest_End:%s\n", runId.c_str());
    fflush(stdout);

    // Cleanup
    for (int i = 0; i < argc; i++) delete[] argv[i];
    delete[] argv;

    return result;
}

namespace test {

    bool GTestBridge::RegisterJNIUtils(JNIEnv* env) {
        return RegisterNativesImpl(env);
    }

}
