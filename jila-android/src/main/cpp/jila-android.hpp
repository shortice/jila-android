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

The local jni reference of `context` is autoclean by returning this function.
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
Returns a array of files/directories in folder (a array of android "content://" uri) 
or NULL if fails.

Please clear this return array by using free when you done using it.
*/
const char** Jila_Android_IterateFs(const char* folder_uri, bool recursive, bool iterFolders);


#ifdef __cplusplus
}
#endif