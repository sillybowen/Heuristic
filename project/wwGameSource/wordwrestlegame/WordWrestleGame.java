package wordwrestlegame;

import java.awt.*;
import java.awt.event.*;
import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.GridLayout;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import wordwrestlegame.utils.Constants;

//no deprecation here, 2 different methods of action listening

public class WordWrestleGame extends Applet implements ActionListener {
  private static String initWord = Constants.INIT_WORD;
  private static JTextField compressedWord = new JTextField(initWord);
  private static JTextField numMovesTextField;
  private static InputWordPanel lPlyPanel;
  private static InputWordPanel rPlyPanel;
  private int numOfMoves = 5;
  private int turns = 0;
  
  public void init() {
    JFrame.setDefaultLookAndFeelDecorated(true);
    setSize(new Dimension(800, 500));
    this.setLayout(new BorderLayout());

    add(getNumOfMovesPanel(), BorderLayout.NORTH);
    add(getPlayersPanel(), BorderLayout.CENTER);
    add(getCompressedWordPanel(), BorderLayout.SOUTH);
    // repaint();
  }

  public void start() {
  }

  public void stop() {
  }

  
  private JPanel getNumOfMovesPanel() {
    JPanel numOfMovesPanel = new JPanel(new BorderLayout());
    String s = (String) JOptionPane.showInputDialog(numOfMovesPanel,
        "Enter number of moves: ");
    numOfMoves = Integer.parseInt(s);

    numMovesTextField = new JTextField("Number of Moves: "
        + Integer.toString(numOfMoves));
    Font newTextFieldFont = new Font(numMovesTextField.getFont().getName(),
        numMovesTextField.getFont().getStyle(), Constants.FONT_SIZE);
    numMovesTextField.setFont(newTextFieldFont);
    numMovesTextField.setEditable(false);

    numOfMovesPanel.add(numMovesTextField);
    return numOfMovesPanel;
  }

  private JPanel getCompressedWordPanel() {
    JPanel compressWordPanel = new JPanel(new BorderLayout());
    Font newTextFieldFont=new Font(compressedWord.getFont().getName(),
        compressedWord.getFont().getStyle(), Constants.FONT_SIZE);
    compressedWord.setFont(newTextFieldFont);
    compressWordPanel.add(compressedWord, BorderLayout.CENTER);
    return compressWordPanel;
  }
  
  private JPanel getPlayersPanel() {
    JPanel playersPanel = new JPanel(new GridLayout(1, 2));

    lPlyPanel = new InputWordPanel(this, Constants.FIRST_PLAYER_NAME, 1);
    rPlyPanel = new InputWordPanel(this, Constants.SECOND_PLAYER_NAME, 2);
    playersPanel.add(lPlyPanel, BorderLayout.WEST);
    playersPanel.add(rPlyPanel, BorderLayout.CENTER);
    playersPanel.setVisible(true);
    
    return playersPanel;
  }
  
  public void playerInputedWord(String input, int playerID) {
    ++turns;
    compressedWord.setText(initWord);
    System.out.println("Compressed Word = " + initWord);
    if (turns % 2 == 0) {
      --numOfMoves;
      numMovesTextField.setText("Number of Moves: " + Integer.toString(numOfMoves));
    }
    if (playerID == 1) {  // Left player just played
      rPlyPanel.setInputAreaEditable(true);
    } else {
      lPlyPanel.setInputAreaEditable(true);
    }
    repaint();
  }
  
  /*
   * Find largest prefix of input which is the same as the corresponding
   * suffix of @initWord, return number of characters in common
   * If the return value > 0, @initWord will be updated (concatenation of two)
   */
  public int updateCompressedWord(String input) {
    int commonLen = 0;
    int prefixEndInd = 0;
    for (int i = input.length(); i > 0; i--) {
      String inputPrefix = input.substring(0, i);
      if (initWord.endsWith(inputPrefix)) {
        commonLen = inputPrefix.length();
        prefixEndInd = i;
        break;
      }
    }
    
    if (commonLen > 0) {
      initWord += input.substring(prefixEndInd);
    }
    
    return commonLen;
  }

  public void actionPerformed(ActionEvent event) {
    String mycaption = event.getActionCommand();
    // System.out.println(event);
    // Object source=event.getSource();
    
    if (mycaption == "Draw") {
      // System.out.println(A + " " + B + " " +C);
      // product=x*y;
      // backtoString = Double.toString(product);
      // System.out.println(x*y);
      // labelAns.setText(backtoString);
      repaint();
    }
  }
  
  public static void main(String[] args) {
    // Schedule a job for the event-dispatching thread:
    // creating and showing this application's GUI.
    WordWrestleGame game = new WordWrestleGame();
    game.init();
  }

}
