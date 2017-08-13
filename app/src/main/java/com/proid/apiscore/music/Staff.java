package com.proid.apiscore.music;

import java.lang.reflect.Array;
import java.util.ArrayList;

/**
 * Created by pgg on 12/08/17.
 */

public class Staff {
    public enum Clef { C };
    private ArrayList<Valued> valued;
    private Clef clef = Clef.C;
    //private TimeSignature ts;
    //private int tempoBPM;

    public void setValued(ArrayList<Valued> v) {
        valued = v;
    }

    public ArrayList<Valued> getValued() {
        return valued;
    }

    public Clef getClef() {
        return clef;
    }

    public int noteFirstLine() {
        if(clef == null)
            clef = Clef.C;
        switch (clef) {
            case C:
            default:
                return Note.noteFromC0("F3");
        }
    }
}
