package wordwrestlegame.utils;

import java.awt.Color;

public final class Constants {
  public static final String GAME_NAME = "Word Wrestle Game";
  public static final String INIT_WORD = "tomcat";
  public static final String FIRST_PLAYER_NAME = "firstPlayer";
  public static final String SECOND_PLAYER_NAME = "secondPlayer";
  public static final String NEWLINE = "\n";
  public static final String PRE_HTML = "<html><h1>";
  public static final String POST_HTML = "</h1></html>";
  
  /*
   * Set color base on RGB You can get RGB value for your color at
   * "Color picker" at above R=255, G=0, B=0
   */
  public static final Color RED = new Color(255, 0, 0);
  public static final Color BLUE = new Color(0, 0, 255);
  
  public static final int INPUT_WORD_LENGTH = 15;
  public static final int FONT_SIZE = 24;
  public static final int INPUT_FONT_SIZE = 16;
}
