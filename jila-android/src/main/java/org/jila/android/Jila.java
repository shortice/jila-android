package org.jila.android;

import android.annotation.SuppressLint;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.os.Build;

public class Jila {
    private Context appContext;

    public Jila(Context context) {
        appContext = context;
    }

    // Method to create a notification channel
    public void createNotificationChannel(
            String channelId,
            String channelName,
            String channelDescription
    ) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(
                    channelId,
                    channelName,
                    importance);

            channel.setDescription(channelDescription);
            NotificationManager notificationManager = (
                    NotificationManager) appContext.getSystemService(Context.NOTIFICATION_SERVICE);
            if (notificationManager != null) {
                notificationManager.createNotificationChannel(channel);
            }
        }
    }

    // Method to push a notification
    public void PushNotification(
            String channelId,
            int notificationId,
            String title,
            String text,
            int smallIconResId
    ) {
        NotificationManager notificationManager = (
                NotificationManager) appContext.getSystemService(Context.NOTIFICATION_SERVICE);
        if (notificationManager == null) {
            return;
        }

        Notification.Builder builder;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            builder = new Notification.Builder(appContext, channelId);
        } else {
            builder = new Notification.Builder(appContext);
        }

        builder.setContentTitle(title)
                .setContentText(text)
                .setSmallIcon(smallIconResId);

        notificationManager.notify(notificationId, builder.build());
    }

    @SuppressLint("DiscouragedApi")
    public int GetRes(String name, String defType) {
        return appContext.getResources().getIdentifier(
                name,
                defType,
                appContext.getPackageName()
        );
    }
}
