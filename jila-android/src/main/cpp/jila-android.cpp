/*
MIT License

Copyright (c) 2025 Shortice

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <jni.h>
#include <string>
#include "jila-android.hpp"

// === Global variables

static JavaVM* j_JavaVM = nullptr;

static jobject j_JilaObject = nullptr;

static std::string g_LastError;
static onFolderOpen onFolderOpenCallback = NULL;

// Java methods
static jmethodID j_CreateNotificationChannelMethodID = nullptr;
static jmethodID j_PushNotificationMethodID = nullptr;
static jmethodID j_GetResID = nullptr;
static jmethodID j_OpenFolder = nullptr;
static jmethodID j_IterateFiles = nullptr;

// === Helper functions

void Jila_Android_SetError(const char* error) {
    g_LastError = error;
}

const char* Jila_Android_GetError() {
    return g_LastError.c_str();
}

JNIEnv* GetJNIEnv() {
    JNIEnv* env = nullptr;
    if (j_JavaVM == nullptr) {
        Jila_Android_SetError("JNI_OnLoad not called");
        return nullptr;
    }

    int getEnvStat = j_JavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    /*if (getEnvStat == JNI_EDETACHED) {
        if (j_JavaVM->AttachCurrentThread(&env, nullptr) != 0) {
            Jila_Android_SetError("Failed to attach current thread");
            return nullptr;
        }
    } else */ if (getEnvStat == JNI_EVERSION) {
        Jila_Android_SetError("JNI version not supported");
        return nullptr;
    }

    // TODO: multi-thread support

    return env;
}

// === JNI exported functions


extern "C" {

JNIEXPORT void JNICALL Java_org_jila_android_FolderPickerActivity_OnFolderPick(
    JNIEnv* env, jobject instance, jstring path
) {
    if (!onFolderOpenCallback) return;

    const char* path_str = env->GetStringUTFChars(path, NULL);

    if(!path_str) return;

    onFolderOpenCallback(path_str);

    env->ReleaseStringUTFChars(path, path_str);

    onFolderOpenCallback = NULL;
}

}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    j_JavaVM = vm;
    return JNI_VERSION_1_6;
}

jmethodID GetMethodID_(
    JNIEnv* env,
    jclass jilaClass,
    const char* name,
    const char* signature
) {
    jmethodID id = env->GetMethodID(
        jilaClass,
        name,
        signature
    );

    if (id == nullptr) {
        Jila_Android_SetError(
            "Failed to find some method. You upgraded Java android-jila files?"
        );
    }

    return id;
}

bool Jila_Android_InitContext(JNIEnv* env_, jobject context) {
    // Initialize context
    if (j_JavaVM == nullptr) {
        Jila_Android_SetError("JNI_OnLoad not called");
        return false;
    }

    if (context == nullptr) {
        Jila_Android_SetError("context is null");
        return false;
    }

    JNIEnv* env = env_; // Use the passed env for initialization

    // Find Jila class and create object
    jclass jilaClass = env->FindClass("org/jila/android/Jila");

    jmethodID constructorJila = env->GetMethodID(
        jilaClass,
        "<init>", // <init> Java constructor
        "(Landroid/content/Context;)V"
    );


    jobject temp_object = env->NewObject(
        jilaClass, 
        constructorJila, 
        context
    );

    j_JilaObject = env->NewGlobalRef(
        temp_object
    );

    env->DeleteLocalRef(temp_object);
    env->DeleteLocalRef(context);

    // Cache method IDs

    j_CreateNotificationChannelMethodID = GetMethodID_(
        env, jilaClass, "createNotificationChannel",
        "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"
    );

    j_PushNotificationMethodID = GetMethodID_(
        env, jilaClass, "PushNotification",
        "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;I)V"
    );

    j_GetResID = GetMethodID_(
        env, jilaClass, "GetRes",
        "(Ljava/lang/String;Ljava/lang/String;)I"
    );

    j_OpenFolder = GetMethodID_(
        env, jilaClass, "OpenFolder",
        "()V"
    );

    j_IterateFiles = GetMethodID_(
        env, jilaClass, "IterateFiles",
        "(Ljava/lang/String;Z)[Ljava/lang/String;"
        // (String, bool) -> String[]
    );

    env->DeleteLocalRef(jilaClass);

    return true;
}

bool Jila_Android_CreateNotificationChannel(
    const char* channelId,
    const char* channelName,
    const char* channelDescription
) {
    JNIEnv* env = GetJNIEnv();
    if (!env) return false;

    jstring jChannelId = env->NewStringUTF(channelId);
    jstring jChannelName = env->NewStringUTF(channelName);
    jstring jChannelDescription = env->NewStringUTF(channelDescription);

    env->CallVoidMethod(
        j_JilaObject,
        j_CreateNotificationChannelMethodID,
        jChannelId,
        jChannelName,
        jChannelDescription
    );

    env->DeleteLocalRef(jChannelId);
    env->DeleteLocalRef(jChannelName);
    env->DeleteLocalRef(jChannelDescription);

    return true;
}

bool Jila_Android_PushNotification(
    const char* channelId,
    int notificationId,
    const char* title,
    const char* text,
    int smallIconResId
) {
    JNIEnv* env = GetJNIEnv();
    if (!env or !j_PushNotificationMethodID) return false;

    jstring jChannelId = env->NewStringUTF(channelId);
    jstring jTitle = env->NewStringUTF(title);
    jstring jText = env->NewStringUTF(text);

    env->CallVoidMethod(
        j_JilaObject,
        j_PushNotificationMethodID,
        jChannelId,
        notificationId,
        jTitle,
        jText,
        smallIconResId
    );

    env->DeleteLocalRef(jChannelId);
    env->DeleteLocalRef(jTitle);
    env->DeleteLocalRef(jText);

    return true;
}


int Jila_Android_GetResID(const char* name, const char* defType) {
    JNIEnv* env = GetJNIEnv();
    if (!env or !j_GetResID) return 0;

    jstring _name = env->NewStringUTF(name);
    jstring _defType = env->NewStringUTF(defType);

    jint result = env->CallIntMethod(
        j_JilaObject,
        j_GetResID,
        _name,
        _defType
    );

    env->DeleteLocalRef(_name);
    env->DeleteLocalRef(_defType);

    return result;
}

// Open Folder functions

void Jila_Android_OpenFolder(onFolderOpen callback) {
    JNIEnv* env = GetJNIEnv();
    if (!env or !j_OpenFolder) return;

    onFolderOpenCallback = callback;

    env->CallVoidMethod(
        j_JilaObject,
        j_OpenFolder
    );
}

const char** Jila_Android_IterateFiles(const char* folder_uri, bool recursive) {
    JNIEnv* env = GetJNIEnv();
    if (!env or !j_IterateFiles) return NULL;

    jstring uri = env->NewStringUTF(folder_uri);

    jobjectArray files_arrays = (jobjectArray)env->CallObjectMethod(
        j_JilaObject,
        j_IterateFiles,
        uri,
        (jboolean)recursive
    );

    if (files_arrays == NULL) {
        env->DeleteLocalRef(uri);
        return NULL;
    }

    jsize length = env->GetArrayLength(files_arrays);

    if (length == 0) {
        env->DeleteLocalRef(uri);
        env->DeleteLocalRef(files_arrays);

        return NULL;
    }

    const char** result_array = (const char**)malloc(
        (length + 1) * sizeof(char*)
    );

    if (result_array == NULL) {
        env->DeleteLocalRef(uri);
        env->DeleteLocalRef(files_arrays);

        return NULL;
    }

    for (jsize i = 0; i < length; i++) {
        jstring file_uri_js = (jstring)env->GetObjectArrayElement(
            files_arrays, i
        );

        const char* temp_chars = env->GetStringUTFChars(
            file_uri_js, NULL
        );

        result_array[i] = strdup(temp_chars);
        env->ReleaseStringUTFChars(file_uri_js, temp_chars);

        env->DeleteLocalRef(file_uri_js);
    }

    result_array[length] = NULL;

    env->DeleteLocalRef(uri);
    env->DeleteLocalRef(files_arrays);

    return result_array;
}


