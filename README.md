# JILA Android Gradle Plugin

JILA is a lightweight Android library that provides a C++ interface to 
high-level Android API. Written for the Jila framework, but it's 
designed to be easily integrated into any C++-based library with
included basic Android support.

## Architecture

The library consists of two main parts:

1.  **Java Component**: A high-level Java layer (`org.jila.android.Jila`)
that directly interacts with the Android SDK.

2.  **C++ Component**: A C++ layer with a simple C-style interface 
that acts as a bridge to the Java component. It uses JNI to call the 
Java methods. This allows your C++ code to access Android APIs without 
dealing with the complexities of JNI directly (and Android API).

## How use?

One full example how use this lib in my [SDL-JNI](https://github.com/shortice/SDL-Jni) repository.
