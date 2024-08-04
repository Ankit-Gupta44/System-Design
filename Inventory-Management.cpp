#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

class Product
{
public:
    int product_id;
    string name;
    string sku;
    int quantity;
    double price;

    // Default constructor
    Product() : product_id(0), name(""), sku(""), quantity(0), price(0.0) {}

    // Parameterized constructor
    Product(int id, const string &name, const string &sku, int quantity, double price)
        : product_id(id), name(name), sku(sku), quantity(quantity), price(price) {}

    void updateStock(int qty)
    {
        quantity += qty;
    }

    void display() const
    {
        cout << "Product(id=" << product_id << ", name=" << name
             << ", sku=" << sku << ", quantity=" << quantity
             << ", price=" << fixed << setprecision(2) << price << ")\n";
    }
};

class InventoryManagementSystem
{
private:
    unordered_map<int, Product> products;
    vector<unordered_map<int, int>> orders;
    int next_product_id;
    int next_order_id;

public:
    InventoryManagementSystem() : next_product_id(1), next_order_id(1) {}

    void addProduct(const string &name, const string &sku, int quantity, double price)
    {
        Product product(next_product_id, name, sku, quantity, price);
        products[next_product_id] = product;
        next_product_id++;
        cout << "Product added: ";
        product.display();
    }

    void updateProduct(int product_id, const string &name = "", const string &sku = "",
                       int quantity = -1, double price = -1.0)
    {
        if (products.find(product_id) == products.end())
        {
            cout << "Product not found.\n";
            return;
        }
        Product &product = products[product_id];
        if (!name.empty())
        {
            product.name = name;
        }
        if (!sku.empty())
        {
            product.sku = sku;
        }
        if (quantity != -1)
        {
            product.quantity = quantity;
        }
        if (price != -1.0)
        {
            product.price = price;
        }
        cout << "Product updated: ";
        product.display();
    }

    void deleteProduct(int product_id)
    {
        if (products.erase(product_id))
        {
            cout << "Product with id " << product_id << " deleted.\n";
        }
        else
        {
            cout << "Product not found.\n";
        }
    }

    void placeOrder(const unordered_map<int, int> &product_orders)
    {
        double total_amount = 0.0;
        for (const auto &in : product_orders)
        {
            if (products.find(in.first) == products.end())
            {
                cout << "Product with id " << in.first << " not found.\n";
                return;
            }
            Product &product = products[in.first];
            if (product.quantity < in.second)
            {
                cout << "Insufficient stock for product " << in.first << ".\n";
                return;
            }
            total_amount += product.price * in.second;
            product.updateStock(-in.second);
        }

        orders.push_back(product_orders);
        cout << "Order placed. Order ID: " << next_order_id << ", Total Amount: $"
             << fixed << setprecision(2) << total_amount << "\n";
        next_order_id++;
    }

    void generateInventoryReport() const
    {
        cout << "Inventory Report:\n";
        for (const auto &in : products)
        {
            in.second.display();
        }
    }

    void generateSalesReport() const
    {
        cout << "Sales Report:\n";
        for (size_t i = 0; i < orders.size(); ++i)
        {
            cout << "Order ID: " << i + 1 << "\n";
            double order_total = 0.0;
            for (const auto &in : orders[i])
            {
                const Product &product = products.at(in.first);
                double item_total = product.price * in.second;
                order_total += item_total;
                cout << "  Product ID: " << in.first << ", Quantity: " << in.second
                     << ", Item Total: $" << fixed << setprecision(2) << item_total << "\n";
            }
            cout << "  Order Total: $" << fixed << setprecision(2) << order_total << "\n";
        }
    }
};

int main()
{
    InventoryManagementSystem ims;
    ims.addProduct("Laptop", "LAP123", 10, 1000.0);
    ims.addProduct("Mouse", "MOU456", 50, 25.0);
    ims.updateProduct(1, "", "", 8);  // Update quantity
    ims.placeOrder({{1, 2}, {2, 5}}); // Place an order
    ims.generateInventoryReport();    // Generate inventory report
    ims.generateSalesReport();        // Generate sales report

    return 0;
}
