package org.jila.android;

import android.annotation.SuppressLint;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.content.ContentResolver;
import android.database.Cursor;
import android.net.Uri;
import android.provider.DocumentsContract;
import java.util.ArrayList;
import java.util.List;

public class Jila {
    private Context appContext;
    // private native void Android_SetError(String error); TODO: continue

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
                importance
            );

            channel.setDescription(channelDescription);
            NotificationManager notificationManager = (
                NotificationManager
            ) appContext.getSystemService(
                Context.NOTIFICATION_SERVICE
            );

            if (notificationManager != null) {
                notificationManager.createNotificationChannel(channel);
            }
        }
    }

    // Method to push a notification
    @SuppressWarnings("deprecation")
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

    public void OpenFolder() {
        Intent intent = new Intent(appContext, FolderPickerActivity.class);
	    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);

        appContext.startActivity(intent);
    }

    public String[] IterateFs(String contentUri, boolean recursive, boolean iterDirectory) {
        List<String> fileList = new ArrayList<>();
        Uri rootUri = Uri.parse(contentUri);
        
        String documentId = DocumentsContract.getTreeDocumentId(rootUri);
        
        listChildDocuments(rootUri, documentId, recursive, fileList, iterDirectory);
        
        return fileList.toArray(new String[0]);
    }

    private void listChildDocuments(
        Uri treeUri, 
        String parentDocId, 
        boolean recursive, 
        List<String> results,
        boolean iterDirectory
    ) {
        Uri childrenUri = DocumentsContract.buildChildDocumentsUriUsingTree(treeUri, parentDocId);
        ContentResolver resolver = appContext.getContentResolver();
        Cursor cursor = null;

        try {
            cursor = resolver.query(childrenUri, new String[]{
                DocumentsContract.Document.COLUMN_DOCUMENT_ID,
                DocumentsContract.Document.COLUMN_MIME_TYPE,
                DocumentsContract.Document.COLUMN_DISPLAY_NAME
            }, null, null, null);

            if (cursor != null) {
                while (cursor.moveToNext()) {
                    String docId = cursor.getString(0);
                    String mimeType = cursor.getString(1);

                    if (DocumentsContract.Document.MIME_TYPE_DIR.equals(mimeType)) {
                        if (iterDirectory) {
                            Uri fileUri = DocumentsContract.buildDocumentUriUsingTree(treeUri, docId);
                            results.add(fileUri.toString());
                        }
                        
                        if (recursive) {
                            listChildDocuments(treeUri, docId, true, results, iterDirectory);
                        }
                    } else {
                        if (!iterDirectory) {
                            Uri fileUri = DocumentsContract.buildDocumentUriUsingTree(treeUri, docId);
                            results.add(fileUri.toString());
                        }
                    }
                }
            }
        } catch (Exception e) {
            
        } finally {
            if (cursor != null) cursor.close();
        }
    }

    public String getNLDir() {
        return appContext.getApplicationInfo().nativeLibraryDir;
    }
}
