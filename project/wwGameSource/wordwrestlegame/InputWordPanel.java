package wordwrestlegame;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import wordwrestlegame.utils.Constants;

public class InputWordPanel extends JPanel implements ActionListener {
  /**
   * default serializable id
   */
  private static final long serialVersionUID = 1L;
  private final WordWrestleGame wwGame;
  private final String playerName;
  private final int playerId;
  private final Font newTextFieldFont;
  protected JTextField playerNameField;
  protected JTextField textField;
  protected JTextField scoreTag;
  protected JTextField scoreField;
  protected JTextArea textArea;
  public int playerScore;

  public InputWordPanel(WordWrestleGame game, String name, int id) {
    super(new GridBagLayout());
    wwGame = game;
    playerName = name;
    playerId = id;
    playerScore = 0;
    
    playerNameField = new JTextField(playerName + ":", 10);
    playerNameField.setEditable(false);
    if (playerId == 2) {  // Second player will be the first to enter a word
      playerNameField.setForeground(Constants.BLUE);
    } else {
      playerNameField.setForeground(Constants.RED);
    }
    newTextFieldFont = new Font(playerNameField.getFont().getName(),
        playerNameField.getFont().getStyle(), Constants.INPUT_FONT_SIZE);


    textField = new JTextField(Constants.INPUT_WORD_LENGTH);
    textField.addActionListener(this);
    textField.setFont(newTextFieldFont);
    if (id != 1) {
      textField.setEditable(false);
    }
    
    scoreTag = new JTextField("Score:", 10);
    scoreTag.setEditable(false);
    scoreField = new JTextField(Integer.toString(playerScore),
        Constants.INPUT_WORD_LENGTH);
    scoreField.setEditable(false);

    textArea = new JTextArea(5, Constants.INPUT_WORD_LENGTH);
    textArea.setEditable(false);
    textArea.setFont(newTextFieldFont);
    JScrollPane scrollPane = new JScrollPane(textArea);

    // Add Components to this panel.
    GridBagConstraints c = new GridBagConstraints();

    c.gridx = 0;
    c.fill = GridBagConstraints.HORIZONTAL;
    c.ipady = 20;
    add(playerNameField, c);
    c.gridx = 1;
    c.fill = GridBagConstraints.HORIZONTAL;
    add(textField, c);
    
    c.gridx = 0;
    c.gridy = 1;
    c.fill = GridBagConstraints.HORIZONTAL;
    add(scoreTag, c);
    c.gridx = 1;
    add(scoreField, c);

    c.gridy = 2;
    c.fill = GridBagConstraints.BOTH;
    c.weightx = 1.0;
    c.weighty = 1.0;
    add(scrollPane, c);
  }
  
  public void setInputAreaEditable(boolean isEditable) {
    if (isEditable) {
      textField.setEditable(true);
    } else {
      textField.setEditable(false);
    }
    textField.setText("");
  }

  public void actionPerformed(ActionEvent evt) {
    String text = textField.getText();
    int isValidInput = isValidWordInput(text);
    if (isValidInput == -1) {
      JOptionPane.showMessageDialog(this, "Not a word (has space)!",
          "Illegal Input Error", JOptionPane.ERROR_MESSAGE);
      return;
    } else if (isValidInput == -2) {
      JOptionPane.showMessageDialog(this, "No Common Suffix-Prefix",
          "Illegal Input Error", JOptionPane.ERROR_MESSAGE);
      return;
    }
    
    // Update Score
    playerScore += isValidInput * isValidInput;
    scoreField.setText(Integer.toString(playerScore));

    textArea.append(text + Constants.NEWLINE);
    textField.selectAll();
    // Make sure the new text is visible, even if there
    // was a selection in the text area.
    textArea.setCaretPosition(textArea.getDocument().getLength());
    
    if (wwGame != null) {
      setInputAreaEditable(false);
      wwGame.playerInputedWord(text, playerId);
    }
  }
  
  /*
   * 0 -- valid input; 1 - not word(has space)
   */
  private int isValidWordInput(String inputWord) {
    int commonLen;
    
    if (inputWord.contains(" ")) {
      return -1;
    } else if (wwGame != null) {
      commonLen = wwGame.updateCompressedWord(inputWord);
      if (commonLen > 0) {
        return commonLen;
      } else {
        return -2;
      }
    } else {
      return 0;
    }
  }

  /**
   * Create the GUI and show it. For thread safety, this method should be
   * invoked from the event dispatch thread.
   */
  private static void createAndShowGUI() {
    // Create and set up the window.
    JFrame frame = new JFrame("TextDemo");
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    // Add contents to the window.
    frame.add(new InputWordPanel(null, "playerName", 1));

    // Display the window.
    frame.pack();
    frame.setVisible(true);
  }

  public static void main(String[] args) {
    // Schedule a job for the event dispatch thread:
    // creating and showing this application's GUI.
    javax.swing.SwingUtilities.invokeLater(new Runnable() {
      public void run() {
        createAndShowGUI();
      }
    });
  }
}
