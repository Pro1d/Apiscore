package com.proid.apiscore;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

import static java.lang.Integer.min;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    SurfaceView sv;
    boolean running = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        MicrophoneRecorder.startRecording();
        sv = (SurfaceView) findViewById(R.id.sv_main_spectrum);
        sv.getHolder().addCallback(this);
        running = true;

        new Thread(new Runnable() {
            @Override
            public void run() {
                while(running) {
                    tryDrawing(sv.getHolder());
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        running = false;
        MicrophoneRecorder.stopRecording();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        tryDrawing(surfaceHolder);
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        tryDrawing(surfaceHolder);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }

    private void tryDrawing(SurfaceHolder holder) {
        Canvas canvas = holder.lockCanvas();
        if (canvas == null) {
            Log.e("SurfaceHolder", "Cannot draw onto the canvas as it's null");
        } else {
            draw(canvas);
            holder.unlockCanvasAndPost(canvas);
        }
    }

    Bitmap buffer;
    float intensityMax = 0;
    private void draw(Canvas canvas) {
        if(buffer == null || buffer.getWidth() != canvas.getWidth() || buffer.getHeight() != canvas.getHeight()) {
            buffer = Bitmap.createBitmap(canvas.getWidth(), canvas.getHeight(), Bitmap.Config.ARGB_8888);
        }
        Canvas c = new Canvas(buffer);
        c.drawARGB(255,0,0,0);
        Paint paint = new Paint();
        float[] intensity = MicrophoneRecorder.getSpectrumIntensity();
        String[] name = MicrophoneRecorder.getSpectrumName();
        int size = intensity.length;
        int maxNote = 0;
        float mean = 0;
        for(int i = 0; i < size; i++) {
            if (intensity[maxNote] < intensity[i])
                maxNote = i;
            mean += intensity[i];
        }
        intensityMax = Math.max(intensity[maxNote], intensityMax * 0.95f);
        float scale = intensityMax > 0 ? 1/intensityMax : 1;
        mean /= size;
        float var = 0;
        for(int i = 0; i < size; i++)
            var += (intensity[i] - mean)*(intensity[i] - mean);
        float stddev = (float) Math.sqrt(var / size);
        float textSize = c.getHeight() / size * 0.85f;
        int staticWidth = (int) (textSize  * 3);
        int histogramWidth = c.getWidth()-staticWidth;
        paint.setTextSize(textSize);

        //Log.i("###", "Fetched "+intensity.length+" values");
        for(int i = 0; i < size; i++) {
            paint.setColor(name[i].contains("#") ? 0xff88a088 : 0xffa0a0ff);
            float v = intensity[i] * scale;
            int w = (int) (v * histogramWidth);
            int y = i * c.getHeight() / size;
            int y2 = (i+1) * c.getHeight() / size;
            // Static area
            paint.setColor(name[i].contains("#") ? 0xff779077 : 0xff9090e0);
            c.drawRect(0, y, staticWidth, y2, paint);
            // Histogram
            paint.setColor(name[i].contains("#") ? 0xff88a088 : 0xffa0a0ff);
            c.drawRect(staticWidth, y, w+staticWidth, y2, paint);
            // Textn
            paint.setColor(v > 0 && i == maxNote ? 0xffffcccc : 0xff424242);
            c.drawText(name[i], 10, (int) (y2 - ((y2-y) - textSize)/2), paint);
        }
        // Mean & StdDev lines
        int mx = (int) (staticWidth + histogramWidth * mean * scale);
        int s1x = (int) (staticWidth + histogramWidth * (mean + stddev) * scale);
        int s2x = (int) (staticWidth + histogramWidth * (mean - stddev) * scale);
        paint.setColor(0xffAA5555);
        c.drawLine(mx, 0, mx, c.getHeight(), paint);
        paint.setColor(0xffBB8888);
        c.drawLine(s1x, 0, s1x, c.getHeight(), paint);
        c.drawLine(s2x, 0, s2x, c.getHeight(), paint);

        if(intensity[maxNote] > 0.03)
            drawNoteAndStaff(c, name[maxNote], 0xffffffff, c.getWidth()-c.getWidth()/2, 0, c.getWidth()/2, c.getWidth()/2);
        else
            drawNoteAndStaff(c, "", 0xffffffff, c.getWidth()-c.getWidth()/2, 0, c.getWidth()/2, c.getWidth()/2);
        canvas.drawBitmap(buffer, 0, 0, null);
    }

    void drawNoteAndStaff(Canvas c, String noteName, int color, int x, int y, int w, int h) {
        final int y_firstLine = y + h / 3;
        final int height_space = h / 12;
        final boolean drawNote = noteName.matches("[A-G]#?[0-9]");
        Log.i("###", "Note "+noteName +" -> "+drawNote);

        Paint paint = new Paint();
        paint.setStyle(Paint.Style.FILL);
        paint.setColor(color);
        paint.setTextSize(height_space);
        paint.setTextAlign(Paint.Align.RIGHT);
        paint.setTypeface(Typeface.MONOSPACE);

        // hline
        for(int i = 0; i < 5; i++) {
            int py = y_firstLine + i * height_space;
            c.drawLine(x, py, x+w, py, paint);
        }

        if(drawNote) {
            int note = noteName.charAt(0) - 'A';
            final boolean sharp = noteName.contains("#");
            note += (noteName.charAt(noteName.length() - 1) - '0') * 7;
            final int note_pos = note - 4 * 7 - 5;
            Log.i("###", "Note pos: "+note_pos);
            final float radius = height_space / 2;
            final float py_note = y_firstLine - note_pos * radius;
            final float px_note = x + w / 2;

            // over high
            for (int i = 2; i <= note_pos; i += 2) {
                int py = y_firstLine - i / 2 * height_space;
                c.drawLine(px_note - radius * 1.5f, py, px_note + radius * 1.5f, py, paint);
            }
            // over low
            for (int i = -6; i >= note_pos; i -= 2) {
                int py = y_firstLine - i / 2 * height_space;
                c.drawLine(px_note - radius * 1.5f, py, px_note + radius * 1.5f, py, paint);
            }

            // note
            c.drawOval(px_note - radius, py_note - 0.9f * radius,
                    px_note + radius, py_note + 0.9f * radius,
                    paint);

            // alt
            if (sharp)
                c.drawText("#", px_note - radius, py_note + radius, paint);
        }
    }
}
