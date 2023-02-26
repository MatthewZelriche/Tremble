#!/usr/bin/env python

import argparse
import subprocess
import os
import platform
import shutil

parser = argparse.ArgumentParser(description='Build Tremble.')
parser.add_argument('--config', dest='buildConfig',
                   default="release",
                   help='The build type to use. \'checked\' or \'release\'')
parser.add_argument('--buildPhysx', dest='buildPhysx',
                   default=False,
                   help='Whether to build Physx. Defaults to false so we arent constantly rebuilding')

args = parser.parse_args()

# Build config files for Physx
cwdSave = os.getcwd()
os.chdir(cwdSave + "/vendor/PhysX/physx/")
if platform.system() == "Linux":
    # TODO
    if args.buildPhysx:
        subprocess.run(["generate_projects.sh"])
    physxOS = "linux"
elif platform.system() == "Windows":
    if args.buildPhysx:
        subprocess.run(["generate_projects.bat", "vc17win64"])
    physxOS = "win"
else:
    print("Error: Platform not supported")

# Only build physx when we want to, otherwise it constantly rebuilds itself for some reason (?)
if args.buildPhysx:
    # Build physx itself
    subprocess.run(["cmake", "--build", "compiler/vc17win64/", "--config", args.buildConfig,
                    "--target", "ALL_BUILD"])

physxBuildDir = os.getcwd() + "/bin/" + physxOS + ".x86_64.vc143.mt/" + args.buildConfig
print(physxBuildDir)
# Back to cmake root
os.chdir(cwdSave)

# Build tremble
cmakeBuildType = 'Release'
if args.buildConfig == "checked":
    cmakeBuildType = "Debug"

subprocess.run(["cmake", "-S.", "-Bbuild/" +  cmakeBuildType, "-DCMAKE_BUILD_TYPE=" + 
                cmakeBuildType, "-G", "Visual Studio 17 2022"])
subprocess.run(["cmake", "--build", "build/" + cmakeBuildType, "--config", cmakeBuildType,
                    "--target", "ALL_BUILD"])

# Redist
cmakeBuildDir = os.getcwd() + "/build/" + "/bin/" + cmakeBuildType + "/"
#if args.buildPhysx:
shutil.copy(physxBuildDir + "/PhysXCommon_64.dll", cmakeBuildDir)
shutil.copy(physxBuildDir + "/PhysX_64.dll", cmakeBuildDir)
shutil.copy(physxBuildDir + "/PhysXFoundation_64.dll", cmakeBuildDir)
shutil.copytree(os.getcwd() + "/res/", cmakeBuildDir + "/res/", dirs_exist_ok=True)