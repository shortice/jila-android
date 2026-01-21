package org.jila.android;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

public class FolderPickerActivity extends Activity {
    private static final int REQ_CODE = 1000;

    public native void OnFolderPick(String path);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);

        try {
            startActivityForResult(intent, REQ_CODE);
        } catch (Exception e) {
            finishWithResult("");
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQ_CODE) {
            if (resultCode == RESULT_OK && data != null) {
                Uri uri = data.getData();
                if (uri != null) {
                    getContentResolver().takePersistableUriPermission(
                        uri,
                        Intent.FLAG_GRANT_READ_URI_PERMISSION |
                        Intent.FLAG_GRANT_WRITE_URI_PERMISSION
                    );
                    finishWithResult(uri.toString());
                    return;
                }
            }

            finishWithResult("");
        }
    }

    private void finishWithResult(String path) {
        OnFolderPick(path);
        finish();
    }
}