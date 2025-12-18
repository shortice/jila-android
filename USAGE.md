# Using JILA in your project

Here's how to integrate the JILA library into your existing Android Gradle project that uses CMake.

## 1. Add the library to your project

1.  Add JILA as a Git submodule to your project.

```bash
git submodule add https://github.com/shortice/jila-android jila-android
```

2.  In your `settings.gradle`, include the JILA module:

```groovy
include ':jila-android'
project(':jila-android').projectDir = new File('jila-android/jila-android')
```

3.  In your app's `build.gradle`, add JILA as a dependency:

```groovy
android {
    <...>
    // Enable prefab feature for .aar
    buildFeatures {
        prefab true
    }
    <...>
}

<...>
dependencies {
    implementation fileTree(include: ["*.jar", "*.aar"], dir: "libs")
    // Including arr
    implementation project(':jila-android')
}
```

## 2. Update your CMakeLists.txt

In your project's `CMakeLists.txt`, you need to find the `jila-android` library and link it to your native library.

```cmake
# Find the jila-android package. This is made available by the Gradle build.
find_package(jila-android REQUIRED CONFIG)

# Link jila-android to your main native library
target_link_libraries(
    your-native-lib # The name of your main library
    jila-android::jila-android
)
```

## 3. Initialize the library in your C++ code

Before you can use any of the JILA functions, you must initialize it with the Android application context. This should be done once, early in your application's lifecycle.

```cpp
#include <jila-android.hpp>
#include <jni.h>

// Somewhere in your initialization code where you have access to the JNIEnv and Activity/Context
void Init(JNIEnv* env, jobject context) {
    // Code for Get android Activity Context
    if (Jila_Android_InitContext(env, context) != 0) {
        // Handle initialization error
        char* error = Jila_GetError();
        // Log the error...
    }
}
```

## 4. Use it.

Explore available API in the jila-android.hpp file. Documentation maybe soon.

-----

Full example project with this library on my [SDL-JNI](https://github.com/shortice/SDL-Jni) repository.
