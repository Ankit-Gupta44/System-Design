#include <iostream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <chrono>

using namespace std;

class IdGenerator {
public:
    static long generateId() {
        static long id = 0;
        return ++id;
    }
};

class Product {
private:
    long id;
    string name;
    string description;
    double price;
    int quantity;

public:
    Product(const string& name, const string& description, double price, int quantity)
        : id(IdGenerator::generateId()), name(name), description(description), price(price), quantity(quantity) {}

    long getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getDescription() const {
        return description;
    }

    double getPrice() const {
        return price;
    }

    int getQuantity() const {
        return quantity;
    }

    void setQuantity(int quantity) {
        this->quantity = quantity;
    }
};

enum class OrderStatus {
    PENDING,
    PROCESSING,
    SHIPPED,
    DELIVERED,
    CANCELLED
};

class Customer {
private:
    long id;
    string name;
    string email;
    string phone;
    string address;

public:
    Customer(const string& name, const string& email, const string& phone, const string& address)
        : id(IdGenerator::generateId()), name(name), email(email), phone(phone), address(address) {}

    long getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getEmail() const {
        return email;
    }

    string getPhone() const {
        return phone;
    }

    string getAddress() const {
        return address;
    }
};

class Order {
private:
    long id;
    Customer* customer;
    unordered_map<Product*, int> products;
    time_t createdAt;
    OrderStatus status;

public:
    Order(Customer* customer, const unordered_map<Product*, int>& products)
        : id(IdGenerator::generateId()), customer(customer), products(products), createdAt(time(nullptr)), status(OrderStatus::PENDING) {}

    long getId() const {
        return id;
    }

    Customer* getCustomer() const {
        return customer;
    }

    unordered_map<Product*, int> getProducts() const {
        return products;
    }

    time_t getCreatedAt() const {
        return createdAt;
    }

    OrderStatus getStatus() const {
        return status;
    }

    void setStatus(OrderStatus status) {
        this->status = status;
    }
};

class InventoryManagementSystem {
private:
    unordered_map<long, Product*> products;
    unordered_map<long, Order*> orders;
    unordered_map<long, Customer*> customers;

public:
    ~InventoryManagementSystem() {
        for (auto& pair : products) delete pair.second;
        for (auto& pair : orders) delete pair.second;
        for (auto& pair : customers) delete pair.second;
    }

    void addProduct(Product* product) {
        products[product->getId()] = product;
    }

    Product* getProduct(long id) {
        return products[id];
    }

    void updateProductQuantity(long productId, int quantity) {
        Product* product = products[productId];
        if (product) {
            product->setQuantity(quantity);
        }
    }

    void placeOrder(Order* order) {
        orders[order->getId()] = order;
    }

    Order* getOrder(long id) {
        return orders[id];
    }

    void updateOrderStatus(long orderId, OrderStatus status) {
        Order* order = orders[orderId];
        if (order) {
            order->setStatus(status);
        }
    }

    void addCustomer(Customer* customer) {
        customers[customer->getId()] = customer;
    }

    Customer* getCustomer(long id) {
        return customers[id];
    }
};

int main() {
    InventoryManagementSystem ims;

    // Adding products
    Product* product1 = new Product("Laptop", "A high-performance laptop", 1000.0, 10);
    Product* product2 = new Product("Smartphone", "A latest model smartphone", 700.0, 20);
    ims.addProduct(product1);
    ims.addProduct(product2);

    // Adding customers
    Customer* customer1 = new Customer("John Doe", "john@example.com", "1234567890", "123 Elm Street");
    Customer* customer2 = new Customer("Jane Smith", "jane@example.com", "0987654321", "456 Oak Avenue");
    ims.addCustomer(customer1);
    ims.addCustomer(customer2);

    // Placing an order
    unordered_map<Product*, int> productsOrdered = {{product1, 1}, {product2, 2}};
    Order* order1 = new Order(customer1, productsOrdered);
    ims.placeOrder(order1);

    // Updating product quantity
    ims.updateProductQuantity(product1->getId(), 9);

    // Updating order status
    ims.updateOrderStatus(order1->getId(), OrderStatus::PROCESSING);

    // Displaying order details
    Order* retrievedOrder = ims.getOrder(order1->getId());
    cout << "Order ID: " << retrievedOrder->getId() << endl;
    cout << "Customer: " << retrievedOrder->getCustomer()->getName() << endl;
    cout << "Status: " << static_cast<int>(retrievedOrder->getStatus()) << endl;

    return 0;
}
