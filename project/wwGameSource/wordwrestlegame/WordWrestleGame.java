package wordwrestlegame;

import java.awt.*;
import java.awt.event.*;
import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.GridLayout;

import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyledDocument;

import wordwrestlegame.utils.Constants;

//no deprecation here, 2 different methods of action listening

public class WordWrestleGame extends Applet implements ActionListener {
  private static String initWord = Constants.INIT_WORD;
  private static String styleInitWord;
  private static JTextPane compressedWord;
  private static JTextField numMovesTextField;
  private static InputWordPanel lPlyPanel;
  private static InputWordPanel rPlyPanel;
  private static JButton reset;
  private int numOfMoves = 5;
  private int turns = 0;

  public void start() {
    System.out.println("Starting...");
    gameStart();
  }
  
  public void resetWWGame() {
    lPlyPanel.resetInputWordPanel();
    rPlyPanel.resetInputWordPanel();
    turns = 0;
    initWord = Constants.INIT_WORD;
    numMovesTextField.setText("Number of Moves: "
        + Integer.toString(numOfMoves));
    compressedWord.setText(Constants.PRE_HTML + initWord + Constants.POST_HTML);

    String s = (String) JOptionPane.showInputDialog(this,
        "Enter number of moves: ");
    numOfMoves = Integer.parseInt(s);
    numMovesTextField.setText("Number of Moves: "
        + Integer.toString(numOfMoves));
  }

  public void gameStart() {
    setSize(new Dimension(800, 500));
    setLayout(new BorderLayout());

    add(getNumOfMovesPanel(), BorderLayout.NORTH);
    add(getPlayersPanel(), BorderLayout.CENTER);
    add(getCompressedWordPanel(), BorderLayout.SOUTH);    
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

    reset = new JButton("Reset");
    reset.addActionListener(this);

    numOfMovesPanel.add(numMovesTextField);
    numOfMovesPanel.add(reset, BorderLayout.EAST);
    return numOfMovesPanel;
  }

  private JPanel getCompressedWordPanel() {
    compressedWord = new JTextPane();
    compressedWord.setContentType("text/html");
    compressedWord.setPreferredSize(new Dimension(50, 200));
    
    compressedWord.setText(Constants.PRE_HTML + initWord + Constants.POST_HTML);
    compressedWord.setEditable(false);
    JPanel compressWordPanel = new JPanel(new BorderLayout());

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
    // Set @styleInitWord
    compressedWord.setText(styleInitWord);
    System.out.println("Compressed Word = " + initWord + " playerID: "
        + playerID + " turns= " + turns + " numOfMoves: " + numOfMoves);
    if (turns % 2 == 0) {
      --numOfMoves;
      numMovesTextField.setText("Number of Moves: "
          + Integer.toString(numOfMoves));
      if (numOfMoves == 0) {
        String winPlayerName;
        if (lPlyPanel.getPlayerScore() > rPlyPanel.getPlayerScore()) {
          winPlayerName = lPlyPanel.playerName + " wins!";
        } else if (lPlyPanel.getPlayerScore() == rPlyPanel.getPlayerScore()) {
          winPlayerName = "Tie";
        } else {
          winPlayerName = rPlyPanel.playerName + " wins!";
        }
        JOptionPane.showMessageDialog(this, winPlayerName,
            "Game Over", JOptionPane.INFORMATION_MESSAGE);
        resetWWGame();
        return;
      }
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
    String commonWord = new String();
    String preCommonWord = new String();
    String postCommonWord = new String();
    
    for (int i = input.length(); i > 0; i--) {
      String inputPrefix = input.substring(0, i);
      if (initWord.endsWith(inputPrefix)) {
        int suffixPrefixInd = initWord.lastIndexOf(inputPrefix);
        preCommonWord = initWord.substring(0, suffixPrefixInd);
        commonWord = initWord.substring(suffixPrefixInd);
        postCommonWord = input.substring(i);
        commonLen = inputPrefix.length();
        prefixEndInd = i;
        break;
      }
    }
    
    if (commonLen > 0) {
      initWord += input.substring(prefixEndInd);
      System.out.println(preCommonWord + "  " + commonWord + "  "
          + postCommonWord);
      styleInitWord = Constants.PRE_HTML + preCommonWord
          + "<font color=\"red\">" + commonWord + "</font>" + postCommonWord
          + Constants.POST_HTML;
    }
    
    return commonLen;
  }

  public void actionPerformed(ActionEvent event) {
    String mycaption = event.getActionCommand();
    System.out.println("The event is: " + mycaption);
    resetWWGame();
  }
  
  public static void main(String[] args) {
    WordWrestleGame game = new WordWrestleGame();
    game.init();
  }

}
