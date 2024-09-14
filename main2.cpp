#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class Product {
private:
    string productID;
    string name;
    float price;

public:
    // Constructor to initialize the product
    Product(string productID, string name, float price) 
        : productID(productID), name(name), price(price) {}

    // Getters
    string getProductID() const { return productID; }
    string getName() const { return name; }
    float getPrice() const { return price; }

    // Print product details
    void printProduct() const {
        cout << left << setw(15) << productID 
             << left << setw(50) << name 
             << right << setw(10) << fixed << setprecision(2) << price << endl;
    }
};

// Convert strings to uppercase
string toUpperCase(const string& str) {
    string result = str;
    for (char& c : result) {
        c = toupper(c);
    }
    return result;
}

void printAllProducts(const vector<Product>& products) {
    cout << left << setw(15) << "Product ID" 
         << left << setw(50) << "Name" 
         << right << setw(10) << "Price" << endl;
    cout << string(75, '-') << endl;

    for (const auto& product : products) {
        product.printProduct();
    }
    cout << endl;
}

class Payment {
private:
    string paymentMethod;

public:
    
    Payment(const string& method) : paymentMethod(method) {}

    // Function to display payment method
    void printPaymentMethod() const {
        cout << "Payment Method: " << paymentMethod << endl;
    }

    // Getter for the payment method
    string getPaymentMethod() const {
        return paymentMethod;
     }

    // Static method to validate and return a valid payment method
    static Payment selectPaymentMethod() {
        int choice;
        string method;

        cout << "\nChoose a payment method:\n";
        cout << "1. Cash on Delivery (COD)\n";
        cout << "2. GCash\n";
        cout << "3. Maya\n";
        cout << "4. Visa\n";
        cout << "5. MasterCard\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                method = "Cash on Delivery (COD)";
                break;
            case 2:
                method = "GCash";
                break;
            case 3:
                method = "Maya";
                break;
            case 4:
                method = "Visa";
                break;
            case 5:
                method = "MasterCard";
                break;
            default:
                cout << "Invalid choice. Defaulting to Cash on Delivery (COD).\n";
                method = "Cash on Delivery (COD)";
                break;
        }

        return Payment(method);
    }
};

class Order {
private:
    vector<pair<Product, int>> orderItems; // Store product and quantity
    double totalAmount;
    Payment payment;  // Store the payment method for this order

public:
    // Constructor
    Order(const vector<pair<Product, int>>& items, const Payment& paymentMethod)
        : orderItems(items), totalAmount(0), payment(paymentMethod) {
        for (const auto& item : orderItems) {
            totalAmount += item.first.getPrice() * item.second;
        }
    }

    // Print order details
    void printOrder(int orderNumber) const {
        cout << "\nOrder #" << orderNumber << endl;
        cout << left << setw(15) << "Product ID" 
             << left << setw(50) << "Name" 
             << right << setw(10) << "Price" << setw(10) << "Quantity" << endl;
        cout << string(85, '-') << endl; 

        for (const auto& item : orderItems) {
            cout << left << setw(15) << item.first.getProductID()
                 << left << setw(50) << item.first.getName() 
                 << right << setw(10) << fixed << setprecision(2) << item.first.getPrice()
                 << setw(10) << item.second << endl;
        }

        cout << "\nTotal Amount: " << totalAmount << "\n";
        payment.printPaymentMethod();  
    }
};

class Cart {
private:
    vector<pair<Product, int>> cartItems;  // Store product and quantity
    vector<Order> pastOrders; // Store past orders
    Payment payment; // Payment method for the current checkout

public:
    // Constructor
    Cart() : payment(Payment("None")) {} 

    // Add product to the cart
    void addProductToCart(const Product& product) {
        for (auto& item : cartItems) {
            if (item.first.getProductID() == product.getProductID()) {
                item.second++;  // Increase quantity if product already exists in the cart
                cout << endl << product.getName() << " \nhas been added to the cart.\n";
                return;
            }
        }
        cartItems.push_back(make_pair(product, 1));
        cout << endl << product.getName() << " \nhas been added to the cart.\n";
    }

    // View the cart and prompt for checkout
    void viewCart() {
        if (cartItems.empty()) {
            cout << "Your cart is empty.\n";
        } else {
            cout << left << setw(15) << "Product ID" 
                 << left << setw(50) << "Name" 
                 << right << setw(10) << "Price" << setw(10) << "Quantity" << endl;
            cout << string(85, '-') << endl;

            for (const auto& item : cartItems) {
                cout << left << setw(15) << item.first.getProductID()
                     << left << setw(50) << item.first.getName() 
                     << right << setw(10) << fixed << setprecision(2) << item.first.getPrice()
                     << setw(10) << item.second << endl;
            }

            // Ask for checkout
            char checkoutChoice;
            do {
                cout << "Do you want to check out all the products? (Y/N): ";
                cin >> checkoutChoice;
                checkoutChoice = toupper(checkoutChoice);

                if (checkoutChoice != 'Y' && checkoutChoice != 'N') {
                    cout << "Error, please enter 'Y' or 'N' only.\n";
                }
            } while (checkoutChoice != 'Y' && checkoutChoice != 'N');

            if (checkoutChoice == 'Y') {
                // Select payment method
                payment = Payment::selectPaymentMethod();

                // Move cart items to past orders and pass the selected payment method
                pastOrders.push_back(Order(cartItems, payment));
                cartItems.clear();  // Empty the cart after checkout

                cout << "\nYou have successfully checked out. Thank you for your purchase!\n";
                payment.printPaymentMethod();  // Display the selected payment method
            } else {
                cout << "Returning to the menu.\n";
            }
        }
    }

    // View past orders
    void viewOrders() const {
        if (pastOrders.empty()) {
            cout << "No past orders to display.\n";
        } else {
            int orderNumber = 1;
            for (const auto& order : pastOrders) {
                order.printOrder(orderNumber);  // Print the order along with payment method
                orderNumber++;
            }
        }
    }
};

void addToCart(const string& selectedID, const vector<Product>& products, Cart& cart) {
    bool found = false;
    string upperID = toUpperCase(selectedID);

    for (const auto& product : products) {
        if (toUpperCase(product.getProductID()) == upperID) {
            cart.addProductToCart(product);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Product ID not found. Please try again.\n";
    }
}

int main() {
    vector<Product> products = {
        Product("P001", "Black and Gray Athletic Cotton Socks - 6 Pairs", 180),
        Product("P002", "Intermediate Size Basketball", 1999),
        Product("P003", "Adults Plain Cotton T-Shirt - 2 Pack", 799),
        Product("P004", "2 Slot Toaster - Black", 3000),
        Product("P005", "6 Piece White Dinner Plate Set", 1499),
        Product("P006", "6-Piece Nonstick, Carbon Steel Oven Bakeware Set", 499),
        Product("P007", "Plain Hooded Fleece Sweatshirt", 1199),
        Product("P008", "Luxury Towel Set - Graphite Gray", 999),
        Product("P009", "Liquid Laundry Detergent, 110 Loads", 799),
        Product("P010", "Waterproof Knit Athletic Sneakers - Gray", 2999),
        Product("P011", "Women's Chiffon Beachwear Cover Up - Black", 699)
    };

    Cart cart;
    int choice;
    string addToCartQuestion;
    string selectedID;
    bool condition = true;

    while (condition) {
        cout << "\nChoose an option: \n";
        cout << "1. View Products\n";
        cout << "2. View Cart\n";
        cout << "3. View Orders\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        system("cls");

        switch (choice) {
            case 1:
                printAllProducts(products);

                 do {
                cout << "Add to cart? (y/n): ";
                cin >> addToCartQuestion;
                addToCartQuestion = toUpperCase(addToCartQuestion);

                if (addToCartQuestion != "Y" && addToCartQuestion != "N") {
                    cout << "Error, enter 'y' or 'n' only.\n";
                }
            } while (addToCartQuestion != "Y" && addToCartQuestion != "N");

            while (addToCartQuestion == "Y") {
                cout << "Enter the ID of the product you want to add to the shopping cart: ";
                cin >> selectedID;
                addToCart(selectedID, products, cart);

                do {
                    cout << "\nAdd another product to the cart? (y/n): ";
                    cin >> addToCartQuestion;
                    addToCartQuestion = toUpperCase(addToCartQuestion);

                    if (addToCartQuestion == "Y") {
                        printAllProducts(products);
                    } else if (addToCartQuestion != "Y" && addToCartQuestion != "N") {
                        cout << "Error, enter 'y' or 'n' only.\n";
                    }
                } while (addToCartQuestion != "Y" && addToCartQuestion != "N");
            }
            break;
        case 2:
            cart.viewCart();
            break;
        case 3:
            cart.viewOrders();
            break;
        case 4:
        cout << "Thank you for shopping!";
        condition = (!condition);
            break;

            
        default:
            cin.clear();
            cout << "Invalid choice. Please try again.\n";
            cin.ignore();
    }
}
return 0;
}


