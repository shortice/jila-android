#pragma once
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// Retrieves the last error message.
char* Jila_Android_GetError();

/*
Initializes the JILA android library with the Android application context.
Returns 0 on success, -1 on error. Call function Jila_Android_GetError
for info about this.
*/
int Jila_Android_InitContext(JNIEnv* env, jobject context);

/*
Create NotificationChannel for Android notification.
Required in newest android versions.

You may call this function in any android version, because
in Java code we check Android version for you.

Returns 0 on success, -1 on error. Call function Jila_Android_GetError
for info about this.
*/
int Jila_Android_CreateNotificationChannel(
    const char* channelId,
    const char* channelName,
    const char* channelDescription
);

/*
Push simple android notification.

You need create notification channel first, smallIconResId can obtained
in Jila_GetResID function.

Notification channel is required for newest Android version, but
in Java code we check this for you.

Returns 0 on success, -1 on error. Call function Jila_Android_GetError
for info about this.
*/
int Jila_Android_PushNotification(
	const char* channelId,
	int notificationId,
	const char* title,
	const char* text,
	int smallIconResId
);

/*
Get Android resource ID.

Return 0 if resource not exists.
*/
int Jila_Android_GetResID(const char* name, const char* defType);

#ifdef __cplusplus
}
#endif