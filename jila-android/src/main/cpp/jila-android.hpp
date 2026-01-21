#pragma once
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// Retrieves the last error message.
const char* Jila_Android_GetError();

/*
Initializes the JILA android library with the Android application context.
Returns 1 on success, 0 on error. Call function Jila_Android_GetError
for info about this.
*/
bool Jila_Android_InitContext(JNIEnv* env, jobject context);

/*
Create NotificationChannel for Android notification.
Required in newest android versions.

You may call this function in any android version, because
in Java code we check Android version for you.

Returns 1 on success, 0 on error. Call function Jila_Android_GetError
for info about this.
*/
bool Jila_Android_CreateNotificationChannel(
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

Returns 1 on success, 0 on error. Call function Jila_Android_GetError
for info about this.
*/
bool Jila_Android_PushNotification(
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

/*
OnFolderOpen callback.
*/
typedef void(*onFolderOpen)(const char* folder_uri);

/*
Open Android open folder dialog.
*/
void Jila_Android_OpenFolder(onFolderOpen callback);

/*
Returns a array of files in folder (a array of android "content://" uri) 
or NULL if fails.

Please clear this return array by using free when you done using it.
*/
const char** Jila_Android_IterateFiles(const char* folder_uri, bool recursive);


#ifdef __cplusplus
}
#endif