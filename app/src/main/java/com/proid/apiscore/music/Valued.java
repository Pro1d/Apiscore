package com.proid.apiscore.music;

/**
 * Created by pgg on 12/08/17.
 */

public class Valued {
    private int value;

    public Valued(int v) {
        value = v;
    }

    public int getValue() {
        return value;
    }

    public static int getWholeNoteValue() {
        return 1 << 12;
    }
}
