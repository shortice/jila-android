#include "jila-android.hpp"
#include <jni.h>
#include <string>

static JavaVM* j_JavaVM = nullptr;
static jobject j_Context = nullptr;
static jobject j_JilaClass = nullptr;

// Java methods
static jmethodID j_CreateNotificationChannelMethodID = nullptr;
static jmethodID j_PushNotificationMethodID = nullptr;
static jmethodID j_GetResID = nullptr;

static std::string g_LastError;

char* Jila_Android_GetError() {
    return g_LastError.data();
}

void Jila_Android_SetError(const char* error) {
    g_LastError = error;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    j_JavaVM = vm;
    return JNI_VERSION_1_6;
}

/*
Initialize app Context. This call is required for ALL methods on this lib.

context is the android.content.Context. You can find it at your app Activity.
*/
int Jila_Android_InitContext(JNIEnv* env, jobject context) {
    // Initialize context
    if (j_JavaVM == nullptr) {
        Jila_Android_SetError("JNI_OnLoad not called");
        return -1;
    }

    j_Context = env->NewGlobalRef(context);
    if (j_Context == nullptr) {
        Jila_Android_SetError("Failed to create global reference for context");
        return -1;
    }

    // Find Jila class and create object
    jclass jilaClass = env->FindClass("org/jila/android/Jila");
    if (jilaClass == nullptr) {
        Jila_Android_SetError("Failed to find class org/jila/android/Jila");
        return -1;
    }

    jmethodID constructorJila = env->GetMethodID(
            jilaClass,
            "<init>", // <init> Java constructor
            "(Landroid/content/Context;)V");
    if (constructorJila == nullptr) {
        Jila_Android_SetError("Failed to find jila constructor");
        return -1;
    }
    j_JilaClass = env->NewObject(jilaClass, constructorJila, j_Context);

    // Cache method IDs
    j_CreateNotificationChannelMethodID = env->GetMethodID(
            jilaClass,
            "createNotificationChannel",
            "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (j_CreateNotificationChannelMethodID == nullptr) {
        Jila_Android_SetError("Failed to find method createNotificationChannel");
        return -1;
    }

    j_PushNotificationMethodID = env->GetMethodID(
            jilaClass,
            "PushNotification",
            "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;I)V");
    if (j_PushNotificationMethodID == nullptr) {
        Jila_Android_SetError("Failed to find method PushNotification");
        return -1;
    }

    j_GetResID = env->GetMethodID(
            jilaClass,
            "GetRes",
            "(Ljava/lang/String;Ljava/lang/String;)I");
    if (j_GetResID == nullptr) {
        Jila_Android_SetError("Failed to find method GetRes");
        return -1;
    }
    // TODO: improve errors?

    env->DeleteLocalRef(jilaClass);
    return 0;
}

int Jila_Android_CreateNotificationChannel(
        const char* channelId,
        const char* channelName,
        const char* channelDescription) {
    if (j_JilaClass == nullptr || j_CreateNotificationChannelMethodID == nullptr) {
        Jila_Android_SetError("Android JILA API not initialized");
        return -1;
    }

    JNIEnv* env;
    j_JavaVM->AttachCurrentThread(&env, nullptr);

    jstring jChannelId = env->NewStringUTF(channelId);
    jstring jChannelName = env->NewStringUTF(channelName);
    jstring jChannelDescription = env->NewStringUTF(channelDescription);

    env->CallVoidMethod(
            j_JilaClass,
            j_CreateNotificationChannelMethodID,
            jChannelId,
            jChannelName,
            jChannelDescription);

    env->DeleteLocalRef(jChannelId);
    env->DeleteLocalRef(jChannelName);
    env->DeleteLocalRef(jChannelDescription);

    return 0;
}

int Jila_Android_PushNotification(
        const char* channelId,
        int notificationId,
        const char* title,
        const char* text,
        int smallIconResId) {
    if (j_JilaClass == nullptr || j_PushNotificationMethodID == nullptr) {
        Jila_Android_SetError("Android JILA API not initialized");
        return -1;
    }

    JNIEnv* env;
    j_JavaVM->AttachCurrentThread(&env, nullptr);

    jstring jChannelId = env->NewStringUTF(channelId);
    jstring jTitle = env->NewStringUTF(title);
    jstring jText = env->NewStringUTF(text);

    env->CallVoidMethod(
            j_JilaClass,
            j_PushNotificationMethodID,
            jChannelId,
            notificationId,
            jTitle,
            jText,
            smallIconResId);

    env->DeleteLocalRef(jChannelId);
    env->DeleteLocalRef(jTitle);
    env->DeleteLocalRef(jText);

    return 0;
}


int Jila_Android_GetResID(const char* name, const char* defType) {
    if (j_JilaClass == nullptr || j_GetResID == nullptr) {
        Jila_Android_SetError("Android JILA API not initialized");
        return -1;
    }

    JNIEnv* env;
    j_JavaVM->AttachCurrentThread(&env, nullptr);

    jstring _name = env->NewStringUTF(name);
    jstring _defType = env->NewStringUTF(defType);

    jint result = env->CallIntMethod(
            j_JilaClass,
            j_GetResID,
            _name,
            _defType);

    env->DeleteLocalRef(_name);
    env->DeleteLocalRef(_defType);

    return result;
}
