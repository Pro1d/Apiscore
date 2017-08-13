package com.proid.apiscore.music;

/**
 * Created by pgg on 12/08/17.
 */

public class Note extends Valued {
    public enum Accidental { Sharp, Natural };

    private String name;
    private int noteFromC0 = 0;
    private Accidental accidental;

    public Note(String name, int value) {
        super(value);

        if(!isNameValid(name))
            throw new RuntimeException("Invalid note name");
        this.name = name;

        accidental = name.contains("#") ? Accidental.Sharp : Accidental.Natural;
        noteFromC0 = noteFromC0(name);
    }

    public int getNotePositionFromC0() {
        return noteFromC0;
    }

    public Accidental getAccidental() {
        return accidental;
    }

    public static boolean isNameValid(String name) {
        return name.matches("[A-G]#?-?[1-9][0-9]*");
    }

    public static int noteFromC0(String name) {
        if(!isNameValid(name))
            throw new RuntimeException("Invalid note name");

        int noteFromC0 = "CDEFGAB".indexOf(name.charAt(0));
        int index = name.substring(1,2).matches("[\\-0-9]") ? 1 : 2;
        noteFromC0 += 7 * Integer.getInteger(name.substring(index));

        return noteFromC0;
    }
}
