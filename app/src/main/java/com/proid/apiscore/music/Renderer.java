package com.proid.apiscore.music;

import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Point;
import android.graphics.Typeface;

import java.util.ArrayList;

/**
 * Created by pgg on 12/08/17.
 */

public class Renderer {
    private Paint paint = new Paint() {
        {
            setStyle(Paint.Style.FILL);
            setColor(0xffffffff);
            setTextAlign(Paint.Align.RIGHT);
            setTypeface(Typeface.MONOSPACE);
        }
    };
    private Path pathGClef = new Path() {
        {
            moveTo(2, 3.5f);
            lineTo(1, 3);
            lineTo(2, 2);
            lineTo(3, 3);
            lineTo(2, 4);
            lineTo(0, 3);
            lineTo(3, 0);
            lineTo(0.5f, -1);
            lineTo(2, 0);
            lineTo(2, 5);
            lineTo(1.5f, 5.5f);
            lineTo(1, 5);
        }
    };

    void render(Canvas c, Staff staff) {
        final float H = c.getHeight();
        final float W = c.getHeight();
        final float lineSpace = H / 12;
        final float yFirstLine = H / 3;
        final float staffHeight = lineSpace * 4;

        final float xClefOffset = staffHeight / 2;
        final float xValuedOffset = xClefOffset + staffHeight;

        renderStaff(c, 0, yFirstLine, W, lineSpace);
        renderClef(c, staff.getClef(), xClefOffset, yFirstLine, lineSpace);
        renderValued(c, staff, xValuedOffset, yFirstLine, W-xValuedOffset, lineSpace);
    }

    private void renderStaff(Canvas c, float x, float y, float w, float lineSpace) {
        for(int i = 0; i < 5; i++)
            c.drawLine(x, y + i*lineSpace, x+w, y + i*lineSpace, paint);
    }

    private void renderClef(Canvas c, Staff.Clef clef, float x, float y, float lineSpace) {
        if(clef == Staff.Clef.C) {
            Matrix mat = new Matrix();
            mat.postSkew(0.25f / 2, 0, 2, 2);
            mat.postScale(lineSpace, lineSpace);
            mat.postTranslate(x, y);
            Path p = new Path();
            pathGClef.transform(mat, p);

            c.drawPath(p, paint);
        }
    }

    private int getValuedRenderingWidth(Valued v) {
        int wholeNote = Valued.getWholeNoteValue();
        int n = v.getValue();

        if(n >= wholeNote)
            return 6;
        if(n >= wholeNote/2)
            return 4;
        if(n >= wholeNote/4)
            return 3;
        return 2;
    }

    private void renderValued(Canvas c, Staff staff, float x, float y, float width, float lineSpace) {
        int widthMax = (int) (width / lineSpace);
        int usedWidth = 0;
        ArrayList<Valued> valued = staff.getValued();
        int indexStart = valued.size();

        while(indexStart > 0) {
            int w = getValuedRenderingWidth(valued.get(indexStart - 1));
            if (w + usedWidth <= widthMax) {
                usedWidth += w;
                indexStart--;
            }
            else
                break;
        }

        int px = 0;
        for(int i = indexStart; i < valued.size(); i++) {
            Valued v = valued.get(i);
            if(v instanceof Note) {
                Note n = (Note) v;
                int zero = staff.noteFirstLine();
                int offset = n.getNotePositionFromC0() - zero;
                float line = -offset * .5f;
                boolean fill = v.getValue() <= Valued.getWholeNoteValue()/4;
                boolean queueDown = offset >= -4;
                int queue = getQueueStyle(v);
                renderNote(c, (px + .5f) * lineSpace, y + line * lineSpace, lineSpace, fill, queueDown, queue);
            }
            px += getValuedRenderingWidth(v);
        }
    }

    public static int getQueueStyle(Valued v) {
        int whole = Valued.getWholeNoteValue();
        int value = v.getValue();
        if(value >= whole)
            return 0;
        int queue = 1;
        while((value << queue) < whole/2)
            queue++;
        return queue;
    }

    private void renderNote(Canvas c, float cx, float cy, float lineSpace, boolean fill, boolean queueDown, int queueStyle) {
        c.save();
        c.translate(cx, cy);
        if(queueStyle > 0) {
            if (queueDown)
                c.rotate(180);
            Path p = new Path();
            p.moveTo(lineSpace * .5f, lineSpace * .1f);
            p.lineTo(lineSpace * .5f, lineSpace * 3.5f);
            for(int i = 1; i < queueStyle; i++) {
                p.moveTo(lineSpace*.5f, lineSpace * (3.5f - .85f*i));
                p.lineTo(lineSpace*(.5f+1.3f), lineSpace * (3.5f - 2 - .85f*i));
            }
            paint.setStyle(Paint.Style.STROKE);
            c.drawPath(p, paint);
        }
        c.rotate(30);
        paint.setStyle(fill ? Paint.Style.FILL : Paint.Style.STROKE);
        c.drawOval(-lineSpace*.5f, lineSpace*.45f, lineSpace*.5f, lineSpace*.45f, paint);
        c.restore();
    }
}
