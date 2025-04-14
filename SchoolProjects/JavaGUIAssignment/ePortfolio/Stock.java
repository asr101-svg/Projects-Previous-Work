package ePortfolio;
import java.util.*;
/*
 * Program Title: Stock.java
 * Description: Stock user class used by portfolio file
 */

    /**
    * Class Stock, main class for handling stock investments and helping with manipulating their values
    */
    public class Stock extends Investments{

        /**
         * Constructor class stock
         * @param symbol takes the value and stores it into investment symbol variable
         * @param name takes value and stores it into investment name variable
         * @param quantity takes value and stores it into investment quantity variable
         * @param price takes variable and stores it into price variable
         */
        public Stock(String symbol,String name,int quantity,double price) {//constructor for stock
            super(symbol,name,quantity,price);
            updateBookValue();
        }
        /**
         * No arg constructor for stock, sets up default values for each variable
         */
        public Stock() {//no arg constructor
            super();
        }

        /**
         * updateBookValue method recalculates book value
         */
        public void updateBookValue() {//re calculating the bookvalue with the basic bookvalue calculation
            bookValue = price*quantity + 9.99;
        }
        /**
         * getPaymentValue calculates payment 
         * @return returns the payment value calculated
         */
        public double getPaymentValue() {
            return price*quantity - 9.99;
        }

         /**
         * getGainValue calculates gain
         * @return returns the gain value
         */
        public double getGainValue() {
            double payment = getPaymentValue();
            return payment - bookValue;
        }

        /**
         * equals returns true or false when two investments equal eachother
         * @param otherObject is the other instance being compared
         *  
         */
        @Override
        public boolean equals (Object otherObject) {
            if (otherObject == null) {
                return false;
            } 
            else if (getClass() != otherObject.getClass()) {
                return false;
            } 
            else {
                Stock otherInvest = (Stock)otherObject;
                return  symbol.equals(otherInvest.symbol) && name.equals(otherInvest.name) && quantity == otherInvest.quantity && price == otherInvest.price;
            }
        }

        /**
         * toString displays investment information through a string
         * @return string that has the information in a string
         */
        @Override
        public String toString() {//to string
            return String.format("Type: Stock \nSymbol: %s\nName: %s\nQuantity: %d\nPrice: $%.2f\nBook Value: $%.2f\n",symbol,name,quantity,price,bookValue);
        }

        

    }
