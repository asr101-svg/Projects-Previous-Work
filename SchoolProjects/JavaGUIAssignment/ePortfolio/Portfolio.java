/*
 * Program Title: Portfolio.java
 * Description: Main portfolio for assignment three of object oriented programming, using GUIs for implementation.
 */

package ePortfolio;
import java.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

/**
 * This class handles the management of the arraylist and also runs the main method for the program
 */
public class Portfolio extends JFrame implements ActionListener {
    public static final int WIDTH = 600; 
    public static final int HEIGHT = 500;

    private ArrayList<Investments> investments;
    private HashMap<String,ArrayList<Integer>> hashIndex;
    private int investType;
    private int updateCount = 0;

    //main panels
    private JPanel homePanel;
    private JPanel buyPanel;
    private JPanel sellPanel;
    private JPanel updatePanel;
    private JPanel searchPanel;
    private JPanel gainPanel;

    private JMenuItem sellChoice;
    private JMenuItem buyChoice;
    private JMenuItem searchChoice;
    private JMenuItem gainChoice;
    private JMenuItem updateChoice;
    private JMenuItem quitChoice;
    //text fields for buy panel
    private JTextField symbolBuy;
    private JTextField nameBuy;
    private JTextField quantityBuy;
    private JTextField priceBuy;
    //text fiels for sell panel
    private JTextField symbolSell;
    private JTextField quantitySell;
    private JTextField priceSell;
    //text field for update panel
    private JTextField symbolUpdate;
    private JTextField nameUpdate;
    private JTextField priceUpdate;
    //text fields for search panel
    private JTextField symbolSearch;
    private JTextField nameSearch;
    private JTextField lowPriceSearch;
    private JTextField highPriceSearch;
    //text fields for gain panel
    private JTextField totalGain;
    
    //buttons for buy panel
    private JButton buyButton;
    private JButton resetBuy;
    //buttons for sell panel
    private JButton sellButton;
    private JButton resetSell;
    //buttons for update panel
    private JButton prevButton;
    private JButton nextButton;
    private JButton saveButton;
    //buttons for search panel
    private JButton searchButton;
    private JButton resetSearch;

    //message areas for each panel
    private JTextArea theTextBuy;
    private JTextArea theTextSell;
    private JTextArea theTextSearch;
    private JTextArea theTextGain;
    private JTextArea theTextUpdate;

    private JComboBox<String> buyTypeList;
    

    public Portfolio() {
        
        super( );
        investments = new ArrayList<>();
        hashIndex = new HashMap <String,ArrayList<Integer>>();
        setTitle("ePortfolio");
        setSize(WIDTH, HEIGHT);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout( ));

        JMenu optionMenu = new JMenu("Commands");//main menu bar
        buyChoice = new JMenuItem("Buy");
        buyChoice.addActionListener(this);
        optionMenu.add(buyChoice);

        sellChoice = new JMenuItem("Sell");
        sellChoice.addActionListener(this);
        optionMenu.add(sellChoice);

        updateChoice = new JMenuItem("Update");
        updateChoice.addActionListener(this);
        optionMenu.add(updateChoice);

        searchChoice = new JMenuItem("Search");
        searchChoice.addActionListener(this);
        optionMenu.add(searchChoice);

        gainChoice = new JMenuItem("Get Gain");
        gainChoice.addActionListener(this);
        optionMenu.add(gainChoice);

        quitChoice = new JMenuItem("Quit");
        quitChoice.addActionListener(this);
        optionMenu.add(quitChoice);

        JMenuBar optionBar = new JMenuBar();
        optionBar.add(optionMenu);
        setJMenuBar(optionBar);

        theTextBuy = new JTextArea("", 1000,100);//text area for messages (buy)
        theTextBuy.setEditable(false);
        theTextBuy.setLineWrap(true);

        theTextSell = new JTextArea("", 1000,100);//text area for messages (sell)
        theTextSell.setEditable(false);
        theTextSell.setLineWrap(true);

        theTextSearch = new JTextArea("", 1000,100);//text area for messages (search)
        theTextSearch.setEditable(false);
        theTextSearch.setLineWrap(true);

        theTextGain = new JTextArea("", 1000,100);//text area for messages (gain)
        theTextGain.setEditable(false);
        theTextGain.setLineWrap(true);

        theTextUpdate = new JTextArea("", 1000,100);//text area for messages (update)
        theTextUpdate.setEditable(false);
        theTextUpdate.setLineWrap(true);

        JScrollPane scrollText = new JScrollPane(theTextBuy);//message box (buy)
        scrollText.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scrollText.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        JScrollPane scrollText2 = new JScrollPane(theTextSell);//message box (sell)
        scrollText.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scrollText.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        JScrollPane scrollText3 = new JScrollPane(theTextSearch);//message box (search)
        scrollText.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scrollText.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        JScrollPane scrollText4 = new JScrollPane(theTextGain);//message box (gain)
        scrollText.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scrollText.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        JScrollPane scrollText5 = new JScrollPane(theTextUpdate);//message box (update)
        scrollText.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scrollText.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);

        homePanel = new JPanel();//initial panel when loading in
        homePanel.setLayout(new GridLayout(2,1,0,5));
        JLabel welcome = new JLabel("Welcome to ePortfolio");
        welcome.setFont(welcome.getFont().deriveFont(Font.PLAIN, 16));
        homePanel.add(welcome);
        //welcome text 
        JTextArea welcomeText = new JTextArea("Choose a command from the “Commands” menu to buy or sell an investment, update prices for all investments, get gain for the portfolio, search for relevant investments, or quit the program.");
        welcomeText.setFont(welcomeText.getFont().deriveFont(Font.PLAIN, 14));
        welcomeText.setLineWrap(true);//wrapping so it doesnt cut off
        welcomeText.setOpaque(false);//to make text background blend with ui
        homePanel.add(welcomeText);
        add(homePanel,BorderLayout.CENTER);

        //buy option
        buyPanel = new JPanel();
        buyPanel.setLayout(new GridLayout(2,1)); //2 rows with 1 column as main panel

        JPanel upperBuyPanel = new JPanel();//sub panel for upper area
        upperBuyPanel.setLayout(new GridLayout(1,2));//1 row 2 colums for upper part

        JPanel leftBuyPanel = new JPanel();//left panel of upper area
        leftBuyPanel.setLayout(new GridLayout(5,2));
        leftBuyPanel.add(new JLabel("Type"));
        String[] typeString = {"Stock","Mutual Fund"};//combo box for investment type
        buyTypeList = new JComboBox<>(typeString);
        buyTypeList.setSelectedIndex(0);
        buyTypeList.addActionListener(this);
        leftBuyPanel.add(buyTypeList);
        leftBuyPanel.add(new JLabel("Symbol"));
        symbolBuy = new JTextField(30);
        leftBuyPanel.add(symbolBuy);
        leftBuyPanel.add(new JLabel("Name"));
        nameBuy = new JTextField(30);
        leftBuyPanel.add(nameBuy);
        leftBuyPanel.add(new JLabel("Quantity"));
        quantityBuy = new JTextField(30);
        leftBuyPanel.add(quantityBuy);
        leftBuyPanel.add(new JLabel("Price"));
        priceBuy = new JTextField(30);
        leftBuyPanel.add(priceBuy);

        JPanel rightBuyPanel = new JPanel();//right side of upper buy panel
        rightBuyPanel.setLayout(new GridLayout(2,1));
        JPanel buyButtonPanel = new JPanel();//used to make the buy and reset buttons not take up entire panel space
        buyButtonPanel.setLayout(new FlowLayout());
        buyButton = new JButton("Buy");
        buyButton.addActionListener(new ActionListener () {
            public void actionPerformed(ActionEvent e) {
                buyInvestment();
            }
        });
        buyButtonPanel.add(buyButton);
        rightBuyPanel.add(buyButtonPanel);

        JPanel resetBuyButtonPanel = new JPanel();//used to make the buy and reset buttons not take up entire panel space
        resetBuyButtonPanel.setLayout(new FlowLayout());
        resetBuy = new JButton("Reset");
        resetBuy.addActionListener(new ActionListener () {
            public void actionPerformed(ActionEvent e) {//resets all the text within the message panels
                symbolBuy.setText("");
                nameBuy.setText("");
                quantityBuy.setText("");
                priceBuy.setText("");
            
            }
        });
        resetBuyButtonPanel.add(resetBuy);
        rightBuyPanel.add(resetBuyButtonPanel);

        upperBuyPanel.add(leftBuyPanel);//adding right and left upper panels
        upperBuyPanel.add(rightBuyPanel);
        
        JPanel lowerBuyPanel = new JPanel();//lower panel
        lowerBuyPanel.setLayout(new GridLayout(2,1));
        lowerBuyPanel.add(new JLabel("Messages"),BorderLayout.WEST);
        lowerBuyPanel.add(scrollText);

        buyPanel.add(upperBuyPanel);//adding both upper and lower panels of buy
        buyPanel.add(lowerBuyPanel);

        sellPanel = new JPanel();
        sellPanel.setLayout(new GridLayout(2,1)); //2 rows with 1 column as main panel

        JPanel upperSellPanel = new JPanel();//sub panel for upper area
        upperSellPanel.setLayout(new GridLayout(1,2));//1 row 2 colums for upper part

        JPanel leftSellPanel = new JPanel();//left panel of upper area
        leftSellPanel.setLayout(new GridLayout(3,2));//three options
        leftSellPanel.add(new JLabel("Symbol"));
        symbolSell = new JTextField(30);
        leftSellPanel.add(symbolSell);
        leftSellPanel.add(new JLabel("Quantity"));
        quantitySell = new JTextField(30);
        leftSellPanel.add(quantitySell);
        leftSellPanel.add(new JLabel("Price"));
        priceSell = new JTextField(30);
        leftSellPanel.add(priceSell);

        JPanel rightSellPanel = new JPanel();//right side of upper sell panel
        rightSellPanel.setLayout(new GridLayout(2,1));
        JPanel sellButtonPanel = new JPanel();
        sellButtonPanel.setLayout(new FlowLayout());
        sellButton = new JButton("Sell");
        sellButton.addActionListener(new ActionListener () {
            public void actionPerformed(ActionEvent e) {
                sellInvestment();
            }
        });
        sellButtonPanel.add(sellButton);
        rightSellPanel.add(sellButtonPanel);
        JPanel resetSellButtonPanel = new JPanel();//used to make the buy and reset buttons not take up entire panel space
        resetSellButtonPanel.setLayout(new FlowLayout());
        resetSell = new JButton("Reset");
        resetSell.addActionListener(new ActionListener () {
            public void actionPerformed(ActionEvent e) {//resets all the text within the message panels
                symbolSell.setText("");
                quantitySell.setText("");
                priceSell.setText("");
            
            }
        });
        resetSellButtonPanel.add(resetSell);
        rightSellPanel.add(resetSellButtonPanel);

        upperSellPanel.add(leftSellPanel);//adding right and left upper panels
        upperSellPanel.add(rightSellPanel);
        
        JPanel lowerSellPanel = new JPanel();//lower panel
        lowerSellPanel.setLayout(new GridLayout(2,1));
        lowerSellPanel.add(new JLabel("Messages"),BorderLayout.WEST);
        lowerSellPanel.add(scrollText2);

        sellPanel.add(upperSellPanel);//adding both upper and lower panels of sell
        sellPanel.add(lowerSellPanel);

        updatePanel = new JPanel();
        updatePanel.setLayout(new GridLayout(2,1)); //2 rows with 1 column as main panel

        JPanel upperUpdatePanel = new JPanel();//sub panel for upper area
        upperUpdatePanel.setLayout(new GridLayout(1,2));//1 row 2 colums for upper part

        JPanel leftUpdatePanel = new JPanel();//left panel of upper area
        leftUpdatePanel.setLayout(new GridLayout(3,2));//three options
        leftUpdatePanel.add(new JLabel("Symbol"));
        symbolUpdate = new JTextField(30);
        symbolUpdate.setEditable(false);
        leftUpdatePanel.add(symbolUpdate);
        leftUpdatePanel.add(new JLabel("Name"));
        nameUpdate = new JTextField(30);
        nameUpdate.setEditable(false);
        leftUpdatePanel.add(nameUpdate);
        leftUpdatePanel.add(new JLabel("Price"));
        priceUpdate = new JTextField(30);
        leftUpdatePanel.add(priceUpdate);

        JPanel rightUpdatePanel = new JPanel();//right side of upper sell panel
        rightUpdatePanel.setLayout(new GridLayout(3,1));
        JPanel prevButtonPanel = new JPanel();
        prevButtonPanel.setLayout(new FlowLayout());
        prevButton = new JButton("Prev");
        prevButton.addActionListener(new ActionListener () {
            public void actionPerformed(ActionEvent e) {
                if (updateCount == 0 || investments.isEmpty()) {
                    appendMessageBox("Cannot go back any further!", theTextUpdate);
                }
                else {
                    updateCount--;
                    nameUpdate.setText(investments.get(updateCount).getName());
                    symbolUpdate.setText(investments.get(updateCount).getSymbol());
                }
            }
        });
        prevButtonPanel.add(prevButton);
        rightUpdatePanel.add(prevButtonPanel);
        JPanel nextButtonPanel = new JPanel();//used to make the buy and reset buttons not take up entire panel space
        nextButtonPanel.setLayout(new FlowLayout());
        nextButton = new JButton("Next");
        nextButton.addActionListener(new ActionListener () {
            public void actionPerformed(ActionEvent e) {
                if (updateCount == investments.size() - 1 || investments.isEmpty()) {
                    appendMessageBox("Cannot go any further!", theTextUpdate);
                }
                else {
                    updateCount++;
                    nameUpdate.setText(investments.get(updateCount).getName());
                    symbolUpdate.setText(investments.get(updateCount).getSymbol());
                }
            }
        });
        nextButtonPanel.add(nextButton);
        rightUpdatePanel.add(nextButtonPanel);
        JPanel saveButtonPanel = new JPanel();
        saveButtonPanel.setLayout(new FlowLayout());
        saveButton = new JButton("Save");
        saveButton.addActionListener(new ActionListener () {
            public void actionPerformed(ActionEvent e) {
                updateInvestmentPrices();
            }
        });
        saveButtonPanel.add(saveButton);
        rightUpdatePanel.add(saveButtonPanel);

        upperUpdatePanel.add(leftUpdatePanel);//adding right and left upper panels
        upperUpdatePanel.add(rightUpdatePanel);

        JPanel lowerUpdatePanel = new JPanel();//lower panel
        lowerUpdatePanel.setLayout(new GridLayout(2,1));
        lowerUpdatePanel.add(new JLabel("Messages"),BorderLayout.WEST);
        lowerUpdatePanel.add(scrollText5);

        updatePanel.add(upperUpdatePanel);//adding both upper and lower panels of sell
        updatePanel.add(lowerUpdatePanel);

        searchPanel = new JPanel();
        searchPanel.setLayout(new GridLayout(2,1)); //2 rows with 1 column as main panel

        JPanel upperSearchPanel = new JPanel();//sub panel for upper area
        upperSearchPanel.setLayout(new GridLayout(1,2));//1 row 2 colums for upper part

        JPanel leftSearchPanel = new JPanel();//left panel of upper area
        leftSearchPanel.setLayout(new GridLayout(4,2));//four options
        leftSearchPanel.add(new JLabel("Symbol"));
        symbolSearch = new JTextField(30);
        leftSearchPanel.add(symbolSearch);
        leftSearchPanel.add(new JLabel("Name Keywords"));
        nameSearch = new JTextField(30);
        leftSearchPanel.add(nameSearch);
        leftSearchPanel.add(new JLabel("Low Price"));
        lowPriceSearch = new JTextField(30);
        leftSearchPanel.add(lowPriceSearch);
        leftSearchPanel.add(new JLabel("High Price"));
        highPriceSearch = new JTextField(30);
        leftSearchPanel.add(highPriceSearch);

        JPanel rightSearchPanel = new JPanel();//right side of upper sell panel
        rightSearchPanel.setLayout(new GridLayout(2,1));
        JPanel resetSearchButtonPanel = new JPanel();
        resetSearchButtonPanel.setLayout(new FlowLayout());

        resetSearch = new JButton("Reset");
        resetSearch.addActionListener(new ActionListener () {
            public void actionPerformed(ActionEvent e) {//resets all the text within the message panels
                symbolSearch.setText("");
                nameSearch.setText("");
                highPriceSearch.setText("");
                lowPriceSearch.setText("");
            }
        });
        resetSearchButtonPanel.add(resetSearch);
        rightSearchPanel.add(resetSearchButtonPanel);

        JPanel searchButtonPanel = new JPanel();//used to make the buy and reset buttons not take up entire panel space
        searchButtonPanel.setLayout(new FlowLayout());
        searchButton = new JButton("Search");
        searchButton.addActionListener(new ActionListener () {
            public void actionPerformed(ActionEvent e) {
                searchInvestments();
            }
        });
        searchButtonPanel.add(searchButton);
        rightSearchPanel.add(searchButtonPanel);

        upperSearchPanel.add(leftSearchPanel);//adding right and left upper panels
        upperSearchPanel.add(rightSearchPanel);

        JPanel lowerSearchPanel = new JPanel();//lower panel
        lowerSearchPanel.setLayout(new GridLayout(2,1));
        lowerSearchPanel.add(new JLabel("Search Results"),BorderLayout.WEST);
        lowerSearchPanel.add(scrollText3);

        searchPanel.add(upperSearchPanel);//adding both upper and lower panels of sell
        searchPanel.add(lowerSearchPanel);

        gainPanel = new JPanel();
        gainPanel.setLayout(new BorderLayout());//border because only one upper and lower
        JPanel upperGainPanel = new JPanel();//sub panel for upper area
        upperGainPanel.setLayout(new FlowLayout(FlowLayout.CENTER));//center of panel

        upperGainPanel.add(new JLabel("Total Gain"));
        totalGain = new JTextField(30);
        totalGain.setEditable(false);
        upperGainPanel.add(totalGain);

        JPanel lowerGainPanel = new JPanel();//lower panel
        lowerGainPanel.setLayout(new GridLayout(2,1));
        lowerGainPanel.add(new JLabel("Individual Gains"));
        lowerGainPanel.add(scrollText4);

        gainPanel.add(upperGainPanel, BorderLayout.NORTH);
        gainPanel.add(lowerGainPanel, BorderLayout.CENTER);
        
    }
    /**
     * actionPerformed gets an action event and handles it
     * @param e is the specific action command that is found
     */
    public void actionPerformed(ActionEvent e) {
        String selectedOption = e.getActionCommand( );
        //for when the option is a menu change option
        if (e.getSource() == buyChoice || e.getSource() == sellChoice || e.getSource() == searchChoice || e.getSource() == gainChoice  || e.getSource() == updateChoice || e.getSource() == quitChoice) {
            getContentPane().remove(buyPanel);
            getContentPane().remove(sellPanel);
            getContentPane().remove(updatePanel);
            getContentPane().remove(searchPanel);
            getContentPane().remove(gainPanel);
            getContentPane().remove(homePanel);
            if (selectedOption.equalsIgnoreCase("Buy")) {//buy menu
                updateMessageBox("",theTextBuy);
                add(buyPanel, BorderLayout.CENTER);
                investType = 1;//defaulted to one
            }
            else if (selectedOption.equalsIgnoreCase("Sell")) {//sell menu
                updateMessageBox("",theTextSell);
                add(sellPanel, BorderLayout.CENTER);
            }
            else if (selectedOption.equalsIgnoreCase("Update")) {//update menu
                updateCount = 0;
                updateMessageBox("",theTextUpdate);
                
                add(updatePanel, BorderLayout.CENTER);
                if (!investments.isEmpty()) {//only works when investments has data, otherwise will show that there is 0 data
                    nameUpdate.setText(investments.get(updateCount).getName());
                    symbolUpdate.setText(investments.get(updateCount).getSymbol());
                }
                else {
                    updateMessageBox("Your have no existing investments! You can only use this option when you have investments.",theTextUpdate);
                }
            }
            else if (selectedOption.equalsIgnoreCase("Search")) {//search menu
                updateMessageBox("",theTextSearch);
                add(searchPanel, BorderLayout.CENTER);
            }
            else if (selectedOption.equalsIgnoreCase("Get Gain")) {//get gain menu
                updateMessageBox("",theTextGain);
                add(gainPanel, BorderLayout.CENTER);
                findGainInvestments();
            }
            else if (selectedOption.equalsIgnoreCase("Quit")) {//quit option
                System.exit(0);
            }
        }
        if (e.getSource() == buyTypeList) {//for when user selects combo box for buying menu
            selectedOption = (String)buyTypeList.getSelectedItem();
            if (selectedOption.equalsIgnoreCase("Stock")) {
                investType = 1;
                System.out.println("changed to stock");
            }
            else if (selectedOption.equalsIgnoreCase("Mutual Fund")){
                investType = 2;
                System.out.println("changed to mutual fund");
            }
            else {
                investType = -1;
            }
        }
        validate();//re sets up panels each time menu is changed
        repaint();
    }
    

    /**
     * updateMessageBox updates the given message box within a panel replacing it
     * @param message is the message being used
     * @param textArea is the name of the text area being changed
     */
    public void updateMessageBox(String message,JTextArea textArea) {
        textArea.setText(message);
    }
    /**
     * appendMessageBox appends the given message box within a panel storing new text on the next line
     * @param message is the message being used
     * @param textArea is the name of the text area being appended to
     */
    public void appendMessageBox(String message,JTextArea textArea) {
        textArea.append("\n"+message);
    }
    /**
     * buyInvestment handles the buying of an investment and stores the new data into the investments arraylist
     */
    public void buyInvestment() {
        //initializing all variables acquired from panel
        String investmentSymbol = symbolBuy.getText();
        String investmentName = nameBuy.getText();
        String investmentPrice = priceBuy.getText();
        String investmentQuantity = quantityBuy.getText();
        //initializing basic variables
        boolean sameFlag = false;
        int quantityVal;
        double priceVal;
        int updateIndex = 0;
        double newBookValue;
        String[] hashKeys;
        
        if (investmentSymbol.isEmpty() || investmentName.isEmpty() || investmentPrice.isEmpty() || investmentQuantity.isEmpty()) {//when all input is empty
            updateMessageBox("One of your inputs is empty, please try again!",theTextBuy);
            return;
        }
        try {//attempts to parse quantity and double
            quantityVal = Integer.parseInt(investmentQuantity);
            priceVal = Double.parseDouble(investmentPrice);
        }
        catch (NumberFormatException e) {
            updateMessageBox("Your price or quantity input is invalid! please try again",theTextBuy);
            return;
        }
        if (quantityVal <= 0) {//when the quantity value is negative
            updateMessageBox("Invalid input! The quantity is too small, please try again.",theTextBuy);
            return;
        }
        if (priceVal <= 0) {//when price is negative
            updateMessageBox("Invalid input! The quantity is too small, please try again.",theTextBuy);
            return;
        }
        if (!investments.isEmpty()) {//checks if the investment already exists/mismatch with symbol name
            for (int i = 0; i < investments.size(); i++) {
                if (investments.get(i).getSymbol().equals(investmentSymbol) && investments.get(i) instanceof MutualFund && investType == 1) {
                    updateMessageBox("There is already a mutual fund value that exists with this symbol name! Please re-input a new symbol name.",theTextBuy);
                    return;
                }
                else if (investments.get(i).getSymbol().equals(investmentSymbol) && investments.get(i) instanceof Stock && investType == 2) {
                    updateMessageBox("There is already a stock value that exists with this symbol name! Please re-input a new symbol name.",theTextBuy);
                    return;
                }
                else if (investments.get(i).getSymbol().equals(investmentSymbol) && !investments.get(i).getName().equals(investmentName)) {
                    updateMessageBox("There already is an investment with the same symbol name! Please try again",theTextBuy);
                    return;
                }
                else if (investments.get(i).getSymbol().equals(investmentSymbol) && investments.get(i).getName().equals(investmentName)){
                    sameFlag = true;
                    updateIndex = i;
                }           
            }
        }
        hashKeys = investmentName.split("\\s+");
        for (int i = 0; i < hashKeys.length; i++) {//adds hashmap investment data
            if (hashIndex.containsKey(hashKeys[i].toLowerCase())) {//add new index to hashmap
                hashIndex.get(hashKeys[i].toLowerCase()).add(investments.size());
            }
            else {//add new key + index to hashmap
                hashIndex.put(hashKeys[i].toLowerCase(), new ArrayList<>());
                hashIndex.get(hashKeys[i].toLowerCase()).add(investments.size());
            }
        }
        if (sameFlag) {//updates new stock amount you are buying
            updateMessageBox("Investment found within portfolio",theTextBuy);
            
            switch(investType) {
                case 1:
                    newBookValue = priceVal*quantityVal + 9.99;//calculates new book value and adds it
                    newBookValue = newBookValue + investments.get(updateIndex).getBookValue();
                    quantityVal = quantityVal + investments.get(updateIndex).getQuantity();
                    investments.get(updateIndex).setQuantity(quantityVal);
                    investments.get(updateIndex).setPrice(priceVal);
                    investments.get(updateIndex).setBookValue(newBookValue);
                    break;
                case 2:
                    newBookValue = priceVal*quantityVal;//calculates the new bookvalue and adds it
                    newBookValue = newBookValue + investments.get(updateIndex).getBookValue();
                    quantityVal = quantityVal + investments.get(updateIndex).getQuantity();
                    investments.get(updateIndex).setQuantity(quantityVal);
                    investments.get(updateIndex).setPrice(priceVal);
                    investments.get(updateIndex).setBookValue(newBookValue);
                    break;
            }
            appendMessageBox("Added more shares to investment in portfolio.",theTextBuy);
        }
        else {//adds new investment you are buying
            switch (investType) {
                case 1:
                    try {
                        investments.add(new Stock(investmentSymbol, investmentName, quantityVal,priceVal));
                    }
                    catch (IllegalArgumentException e) {
                        appendMessageBox(e.getMessage(),theTextBuy);
                        return;
                    }
                    
                    break;
                case 2:
                    try {
                        investments.add(new MutualFund(investmentSymbol, investmentName, quantityVal,priceVal));
                    }
                    catch (IllegalArgumentException e) {
                        appendMessageBox(e.getMessage(),theTextBuy);
                        return;
                    }
                    break;

            }
            appendMessageBox("Added investment to portfolio.",theTextBuy);
        }
    }
    /**
     * sellInvestment handles selling investments based off a given quantity and price
     */
    public void sellInvestment() {
        //initializing all variables acquired from panel
        String investmentSymbol = symbolSell.getText();
        String investmentPrice = priceSell.getText();
        String investmentQuantity = quantitySell.getText();
        //initializing basic variables
        int updateIndex= -1;
        int quantityVal;
        double priceVal;
        int tempQuantity;
        double totalSold;
        double newBookVal;
        String[] hashKeys;

        if (investmentSymbol.isEmpty() || investmentPrice.isEmpty() || investmentQuantity.isEmpty()) {//chekcing for empty input
            appendMessageBox("One of your inputs is empty, please try again!",theTextSell);
            return;
        }
        try {
            quantityVal = Integer.parseInt(investmentQuantity);
            priceVal = Double.parseDouble(investmentPrice);
        }
        catch (NumberFormatException e) {
            appendMessageBox("Your price or quantity input is invalid! please try again",theTextSell);
            return;
        }

        for (int i = 0; i < investments.size(); i++ ) {//searches through both stock and mutual fund to find your symbol
            if (investments.get(i).getSymbol().equals(investmentSymbol)) {
                updateIndex = i;
            }
        }
        if (updateIndex == -1) { //if no symbol is found, return back to menu
            appendMessageBox("The following symbol could not be found.",theTextSell);
            return;
        }
        if (priceVal <= 0) {
            appendMessageBox("The price value you entered is either too low or negative! Please try again.",theTextSell);
            return;
        }
        if (quantityVal <= 0) {//checks if user enters negative quantity or 0 quantity
            appendMessageBox("The quantity value you entered is either too low or negative! Please try again.",theTextSell);
        }
        tempQuantity = quantityVal;//stores the number of quantity youre selling by
        quantityVal = investments.get(updateIndex).getQuantity() - quantityVal;//calculates the remaining quantity
        if (investments.get(updateIndex) instanceof Stock) {
            totalSold = tempQuantity*priceVal - 9.99;
        }
        else {
            totalSold = tempQuantity*priceVal - 45;
        }
        if (quantityVal == 0) {//if all of the investment is sold
            appendMessageBox("Sold " + tempQuantity + " Shares of " + investments.get(updateIndex).getSymbol() + " for " + totalSold,theTextSell);
            hashKeys = investments.get(updateIndex).getName().split("\\s+");
            
            for (int i = 0; i < hashKeys.length; i++) {
                if (hashIndex.containsKey(hashKeys[i].toLowerCase())) {
                    if (hashIndex.get(hashKeys[i].toLowerCase()).size() <= 1) {//when the only index found is the one were removing
                        hashIndex.remove(hashKeys[i].toLowerCase());//remove key entirely
                    }
                    else {
                        hashIndex.get(hashKeys[i].toLowerCase()).remove(Integer.valueOf(updateIndex));//remove one index
                    }
                }
                
            }
            investments.remove(investments.get(updateIndex));

        }
        else if (quantityVal < 0) {//checks if the quantity is too much 
            appendMessageBox("The quantity you want to sell is greater than the amount you currently have!",theTextSell);
            return;
        }
        else {//otherwise sells
            try {
                appendMessageBox("Sold " + tempQuantity + " Shares of " + investments.get(updateIndex).getSymbol() + " for " + totalSold,theTextSell);
                newBookVal = investments.get(updateIndex).getBookValueRemaining(tempQuantity);//calculates the partial book value
                investments.get(updateIndex).setQuantity(quantityVal);//sets quantity,price and bookvalue
                investments.get(updateIndex).setPrice(priceVal);
                investments.get(updateIndex).setBookValue(newBookVal);
            }
            catch (IllegalArgumentException e ) {
                appendMessageBox(e.getMessage(),theTextSell);
                return;
            }
            
        }
    }
    /**
     * searchInvestment searches through an investment 
     */
    public void searchInvestments() {
        ArrayList<Integer> indiciesKeep = new ArrayList<Integer>();
        ArrayList<Integer> keywordFilter = new ArrayList<Integer>();
        String[] arrayWords;
        String[] nameSplit;
        boolean foundFlag = false;
        String investmentSymbol = symbolSearch.getText();//getting symbol 
        String investmentPriceRange = "";
        if (!lowPriceSearch.getText().isEmpty() || !highPriceSearch.getText().isEmpty()) {//storing price range if not empty
            investmentPriceRange = lowPriceSearch.getText() + "-" + highPriceSearch.getText();
        }
        String investmentKeywords = nameSearch.getText();//getting keywrods
        if (investments.isEmpty()) {//when investments is empty
            updateMessageBox("You have entered no values in both stock investments or mutual fund investments! Please enter an investment to use this function.",theTextSearch);
            return;
        }
        if (checkRange(investmentPriceRange) == -1 && !investmentPriceRange.isEmpty()) {//if invalid pricerange is entered
            appendMessageBox("Your price range is invalid! Please enter a valid price range and try again.",theTextSearch);
            return;
        }
        updateMessageBox("",theTextSearch);
        if (!investmentKeywords.isBlank()) {//sorts through keywords and stores the found keyword indicies in an arraylist indiciesKeep
            arrayWords = investmentKeywords.split("\\s+"); 
            for (int i =0; i < investments.size();i++) {//add ALL indices from investments
                indiciesKeep.add(i);
            }
            for (int i = 0; i < arrayWords.length; i++) {
                if (hashIndex.containsKey(arrayWords[i].toLowerCase())) {
                    keywordFilter.addAll(hashIndex.get(arrayWords[i].toLowerCase()));
                    indiciesKeep.retainAll(keywordFilter);
                    keywordFilter.clear();
                }
            }
            for (int i = 0; i < indiciesKeep.size(); i++) {//iterates through valid keyword indicies
                if (investmentPriceRange.isBlank() && investmentSymbol.isBlank()) {//if only keyword is listed
                    appendMessageBox(investments.get(indiciesKeep.get(i)).toString(),theTextSearch);
                    foundFlag = true;
                }
                else if (investmentSymbol.isBlank()) {//if only symbol is empty (keyword + range)
                    if (priceRangeChecker(investments.get(indiciesKeep.get(i)),investmentPriceRange) == true) {
                        appendMessageBox(investments.get(indiciesKeep.get(i)).toString(),theTextSearch);
                        foundFlag = true;
                    }
                }
                else if (investmentPriceRange.isBlank()) { // if only price range is empty (symbol + keyword)
                    if (investments.get(indiciesKeep.get(i)).getSymbol().equalsIgnoreCase(investmentSymbol)) {
                        appendMessageBox(investments.get(indiciesKeep.get(i)).toString(),theTextSearch);
                        foundFlag = true;
                    }
                }
                else {//if none are empty (keyword + range + symbol)
                    if (investments.get(indiciesKeep.get(i)).getSymbol().equalsIgnoreCase(investmentSymbol) && priceRangeChecker(investments.get(indiciesKeep.get(i)),investmentPriceRange) == true) {
                        appendMessageBox(investments.get(indiciesKeep.get(i)).toString(),theTextSearch);
                        foundFlag = true;
                    }
                }
            }
        }
        else {
            for (int i = 0; i < investments.size(); i++) {//looks through investment
                if (investmentSymbol.isBlank() && investmentKeywords.isBlank() && investmentPriceRange.isBlank()) {//if all are empty
                    appendMessageBox(investments.get(i).toString(),theTextSearch);
                    foundFlag = true;
                }
                else if (investmentKeywords.isBlank() && investmentPriceRange.isBlank()) {//if keywords + price range are empty (symbol)
                    if (investments.get(i).getSymbol().equalsIgnoreCase(investmentSymbol)) {
                        appendMessageBox(investments.get(i).toString(),theTextSearch);
                        foundFlag = true;
                    }
                }
                else if (investmentSymbol.isBlank() && investmentKeywords.isBlank()) { //if symbol + keywords are empty (range)
                    if (priceRangeChecker(investments.get(i),investmentPriceRange) == true) {
                        appendMessageBox(investments.get(i).toString(),theTextSearch);
                        foundFlag = true;
                    }
                }
                else if (investmentKeywords.isBlank()) { //if only keyword is empty (range + symbol)
                    if (investments.get(i).getSymbol().equalsIgnoreCase(investmentSymbol) && priceRangeChecker(investments.get(i),investmentPriceRange) == true) {
                        appendMessageBox(investments.get(i).toString(),theTextSearch);
                        foundFlag = true;
                    }
                }
            }
        }
        if (foundFlag == false) {
            appendMessageBox("The following investment was not found.",theTextSearch);
        }
        
    }
    /**
     * Method checkRange to check the price range a user has entered and sees whether or not it is valid
     * @return returns whether the range is valid or not
     * @param priceRange the price range listed
     */
    public static int checkRange(String priceRange) {
        StringTokenizer numTokenizer = new StringTokenizer(priceRange,"-");
        int minNum,maxNum,specNum;
        if (numTokenizer.countTokens() == 1) {
            try {//try catch statement for parsing,if the parse fails it will return -1
                specNum = Integer.parseInt(numTokenizer.nextToken());
            }
            catch (Exception e) {
                return -1;
            }
            return 1;
        }
        try {
            minNum = Integer.parseInt(numTokenizer.nextToken());
            maxNum = Integer.parseInt(numTokenizer.nextToken());
        }
        catch (Exception e) {
            return -1;
        }
        if (minNum > maxNum) {
            return -1;
        }
        return 1;
    }
    /**
     * Method priceRangeChecker to compare the given price range with the mutal funds price and sees if it is in the range
     * @return returns either true or false whether or not the given price is within the price range
     * @param priceRange the price range listed
     * @param investments the given investment that is being compared
     */
    public static boolean priceRangeChecker(Investments investments, String priceRange) {
        StringTokenizer numTokenizer = new StringTokenizer(priceRange,"-");
        double minNum,maxNum,specNum;
        if (priceRange.indexOf('-') == 0) {//-100
            maxNum = Double.parseDouble(numTokenizer.nextToken());
            if (investments.getPrice() <= maxNum) {
                return true;
            }
            else {
                return false;
            }
        }
        else if (priceRange.indexOf('-') == (priceRange.length() - 1)) {
            minNum = Double.parseDouble(numTokenizer.nextToken());
            if (investments.getPrice() >= minNum) {//100-
                return true;
            }
            else {
                return false;
            }
        }
        if (numTokenizer.countTokens() == 1) {
            specNum = Double.parseDouble(numTokenizer.nextToken());
            if (specNum == investments.getPrice()) {
                return true;
            }
            else {
                return false;
            }
        }
        minNum = Double.parseDouble(numTokenizer.nextToken());
        maxNum = Double.parseDouble(numTokenizer.nextToken());
        if (minNum <= investments.getPrice() && investments.getPrice() <= maxNum) {
            return true;
        }
        return false;
    }

    /**
     * Method updateInvestmentPrices to update all the prices in a stock and arraylist
     */
    public void updateInvestmentPrices() {
        double newPrice;
        String investmentPrice = priceUpdate.getText();
        try {
            newPrice = Double.parseDouble(investmentPrice);
        }
        catch (NumberFormatException e) {
            appendMessageBox("Your price input is invalid! please try again",theTextUpdate);
            return;
        }
        if (investments.isEmpty()) {
            appendMessageBox("You have no existing investments, there is nothing to save.",theTextUpdate); 
            return;
        }
        if (newPrice <= 0) {
            appendMessageBox("Your price input is not a valid price number.",theTextUpdate); 
            return;
        }
        try {
            investments.get(updateCount).setPrice(newPrice);
            appendMessageBox("Set price of investment " + investments.get(updateCount).getSymbol(),theTextUpdate);
        }
        catch (IllegalArgumentException e) {
            appendMessageBox(e.getMessage(),theTextUpdate);
        }
        
    }

    /**
     * Method findGainInvestments to calculate the total gain from the arraylists
     */
    public void findGainInvestments() {
        double newVal = 0;
        String outputVal;
        
        if (!investments.isEmpty()) {//goes through stock
            for (int i = 0; i < investments.size(); i++) {
                appendMessageBox("Investment symbol " + investments.get(i).getSymbol() + " gain:" + String.format("%.2f",investments.get(i).getGainValue()),theTextGain);
                newVal += investments.get(i).getGainValue();
            }
        }
        outputVal = String.format("%.2f", newVal);
        totalGain.setText(outputVal);
    }
    /**
     * Method main runs the GUI for main menu 
     * @param args used for command line arguments (Not used in this context)
     */
    public static void main(String[] args) {
        Portfolio w1 = new Portfolio( );
        
        w1.setVisible(true);
    }
}



