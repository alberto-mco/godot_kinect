#!python
import os

# platform= makes it in line with Godots scons file, keeping p for backwards compatibility
platform = ARGUMENTS.get("p", "linux")
platform = ARGUMENTS.get("platform", platform)

# This makes sure to keep the session environment variables on windows,
# that way you can run scons in a vs 2017 prompt and it will find all the required tools
env = Environment()

if platform == "windows":
    env = Environment(ENV=os.environ)

godot_headers_path = ARGUMENTS.get("headers", os.getenv("GODOT_HEADERS", "godot-cpp/godot_headers"))
godot_bindings_path = ARGUMENTS.get("cpp_bindings", os.getenv("CPP_BINDINGS", "godot-cpp"))
kinect_headers_path = ARGUMENTS.get("kinect_headers", os.getenv("KINECT_HEADERS", "kinect-inc"))
kinect_lib_path = ARGUMENTS.get("kinect_lib", os.getenv("KINECT_LIB", "kinect-lib"))

# default to debug build, must be same setting as used for cpp_bindings
target = ARGUMENTS.get("target", "debug")

#bits
bits = ARGUMENTS.get("bits", "64")

if bits == "64":
    kinect_lib_path = kinect_lib_path + "/amd64"
else:
    kinect_lib_path = kinect_lib_path + "/x86"

if ARGUMENTS.get("use_llvm", "no") == "yes":
    env["CXX"] = "clang++"

# put stuff that is the same for all first, saves duplication
if platform == "windows":
    # need to add detection of msvc vs mingw, this is for msvc...
    env.Append(LINKFLAGS=["/WX"])
    if target == "debug":
        env.Append(CCFLAGS=["-EHsc", "-D_DEBUG", "/MDd"])
    else:
        env.Append(CCFLAGS=["-O2", "-EHsc", "-DNDEBUG", "/MD"])


def add_sources(sources, dir):
    for f in os.listdir(dir):
        if f.endswith(".cpp"):
            sources.append(dir + "/" + f)


env.Append(
    CPPPATH=[
        godot_headers_path,
        godot_bindings_path + "/include",
        godot_bindings_path + "/include/gen/",
        godot_bindings_path + "/include/core/",
        kinect_headers_path,
    ]
)

if target == "debug":
    env.Append(LIBS=["libgodot-cpp.windows.debug.64", 'kinect10.lib'])
else:
    env.Append(LIBS=["libgodot-cpp.windows.release.64", 'kinect10.lib'])

env.Append(LIBPATH=[godot_bindings_path + "/bin/", kinect_lib_path])

sources = []
add_sources(sources, "src")

library = env.SharedLibrary(target="bin/libkinect", source=sources)
Default(library)
