import os
import sys
import time
import uuid
import subprocess
import re
from subprocess import call

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

runId = str(uuid.uuid4())
adbPath = os.environ['ANDROID_HOME'] + "/platform-tools/adb";
packageName = "com.lynx.example"
gtestDriverIntentName = "com.lynx.UNITTEST"

def scanLogcat():
    adbCmd =  adbPath + " logcat | grep APP_STDOUT"
    p = subprocess.Popen([adbPath, "logcat"], stdout=subprocess.PIPE, bufsize=0, shell=False)
    startMark = "GTest_Start:" + runId
    endMark = "GTest_End:" + runId
    gtestLogMark = "APP_STDOUT:"
    sizeOfLogMark = len(gtestLogMark)
    # print startMark + " -- > " + endMark
    while True:
        line = p.stdout.readline()
        if startMark in line:
            break
    while True:
        line = p.stdout.readline()
        if gtestLogMark not in line:
            continue
        if endMark in line:
            break
        if "referenceTable " in line:
            continue
        # Skip log prefix and newline
        line = line[line.index(gtestLogMark) + sizeOfLogMark + 1 :-1]
        # Put term colors
        if "[  FAILED  ]" in line:
            line = line.replace("[  FAILED  ]", bcolors.FAIL + "[  FAILED  ]" + bcolors.ENDC)
        elif ": Failure" in line:
            line = bcolors.WARNING + line + bcolors.ENDC
        else:
            line = re.sub("^\[(.*)\]", bcolors.OKGREEN + r'[\1]' + bcolors.ENDC, line)
        print line
        sys.stdout.flush()

# call(["adb", "logcat","-c"])

FNULL = open(os.devnull, 'w')
call([adbPath,"shell", "am", "force-stop", packageName], stdout=FNULL, stderr=subprocess.STDOUT)
call([adbPath, "shell", "monkey", "-p",packageName, "1"], stdout=FNULL, stderr=subprocess.STDOUT)
time.sleep(1)
call([adbPath,"shell", "am", "broadcast", "-a", gtestDriverIntentName, "--es", "cmd", "\"" + ' '.join(sys.argv) + " " + runId + "\""], stdout=FNULL, stderr=subprocess.STDOUT)
scanLogcat()


