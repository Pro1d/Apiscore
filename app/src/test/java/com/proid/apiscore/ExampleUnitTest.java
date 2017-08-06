package com.proid.apiscore;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
public class ExampleUnitTest {
    @Test
    public void noteName_matches() throws Exception {
        String note = "A4";
        assertTrue(note.matches("[A-G]#?[0-9]"));
    }
    @Test
    public void noteName_matchesSharp() throws Exception {
        String note = "A#4";
        assertTrue(note.matches("[A-G]#?[0-9]"));
    }
    @Test
    public void noteName_doesNotMatchesEmpty() throws Exception {
        String note = "";
        assertFalse(note.matches("[A-G]#?[0-9]"));
    }
}