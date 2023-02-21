#include<iostream>
#include<ctime>
#include<sstream>
#include<iomanip>
#include<list>
#include<string>
#include<conio.h>
#include<stdlib.h>
#include<fstream>
using namespace std;
#define MAX_Products 10




class User
{
    int UserID;
    string Password;

protected:

    bool VerifyLogin(int userID, string password)
    {
        if (UserID == userID && Password == password) return true;
        return false;
    }

    template<typename A>
    void SetPassword(A password)
    {Password = password;}

    template<typename T, int x>
    void SetUserID(T userID)
    {UserID = userID + x;}
};



class Customer;



class product
{
    public:
        product(){};
    //TODO create a constructor to be called when driver code is creating defualt products
    //*parametrized constructor to be called by driver class
        product(int id,string name,float price,string desc,int quantity)
        {
            product_id=id;
            product_name=name;
            this->price=price;
            description=desc;
            quantity_available=quantity;
        }
        //* member functions
        bool check_availabilty();
        void display_detail();


        //* member variables
        int product_id;
        string product_name;
        float price;
        string description;
        int quantity_available;
};
void product::display_detail()
{
    cout<<setw(50)<<setfill('-')<<'-'<<endl;
    cout<<product_name<<endl;
    cout<<"About:---"<<description<<endl;
    if (check_availabilty())
    {
        cout<<"Stock-  Available"<<endl;
    }
    else
    {
        cout<<"Out-of-stock"<<endl;
    }
    cout<<"Price: Rs."<<price<<endl<<endl;
    cout<<"Product id: "<<product_id<<endl;
    cout<<setw(50)<<setfill('-')<<'-'<<endl;
}
bool product::check_availabilty()
{
    if(quantity_available>0)
    {
        return true;
    }
    else
    {
        return false;
    }
}




class Data 
{
    list <Customer> customer_list;
    int customer_Count=0;//defualt no.

public:
    list <product> product_list;//using STL
    int ProductCount;//default no.

    // Data(){};
    void addproduct();
    void DisplayAllProducts();
    void add_customer();
    void add_customer(string, string, string, string);
    void getCustomer();//* to test it
    Customer* VerifyUserLogin(int id,string pass);
};

void Data::addproduct()
{
    //* this will be a function used by the admin of the website
    int id,quantity;
    float price;
    string name,desc;
    cout<<setw(50)<<setfill('-')<<'-'<<endl;
    cout<<"Enter the product id: "<<endl;
    cin>>id;
    cout<<"Enter the product name: ";
    cin.ignore();
    getline(cin,name);
    cout<<"Enter the description of the product: ";
    cin.ignore();
    getline(cin,desc);
    cout<<"Enter the price in rs.: "<<endl;
    cin>>price;
    cout<<"Enter the quantity available: "<<endl;
    cin>>quantity;
    cout<<setw(50)<<setfill('-')<<'-'<<endl;
    cout<<"Product added successfully !!!"<<endl;
    // product p (id ,name,price,quantity,desc,quantity);
    product*p = new product(id,name,price,desc,quantity);
    product_list.push_back(*p);
    ProductCount++;
}

void Data::DisplayAllProducts()
{
    cout<<endl<<"--------------The list of all the products is: --------------"<<endl;
    for(std::list<product>::iterator itr=product_list.begin();itr!=product_list.end();itr++)
    {
        (*itr).display_detail();
    }
}






class Order{

    friend class Customer;
    int OrderID;
    int OrderTotal;
    int ItemList[MAX_Products];
    string ShippingDate;
    string DateOfOrder;
    string OrderStatus;
    string ShippingAddreess;
    Data* DataBase;

    public:

        void SetOrderDetails(int id, int total, int list[], string address, Data* dt)
        {
            time_t ttime = time(0);
            tm *local_time = localtime(&ttime);
            stringstream s;
            int day=(local_time->tm_mday);
            int month=1 + local_time->tm_mon;
            int year=1900 + local_time->tm_year;
            DataBase = dt;
            OrderID=id;
            OrderTotal=total;
            OrderStatus = "Yet to be delivered.";
            s<<day<<'-'<<month<<'-'<<year;
            s>>DateOfOrder;
            s.clear();
            if(day>27)
            {
                month++;
                day=day-30;
            }

            s<<day+3<<'-'<<month<<'-'<<year;
            s>>ShippingDate;
            ShippingAddreess=address;
            for (int i = 0; i < MAX_Products; i++)
                ItemList[i]=list[i];
        }

        void DisplayOrderDetails()
        {
            int i = 0;
            cout<<"Order ID: "<<OrderID;
            cout<<"\nDate of Order: "<<DateOfOrder;
            cout<<"\nShipping Address: "<<ShippingAddreess;
            cout<<"\nShipping Date: "<<ShippingDate;
            cout<<"\nItems in order :- \n\n";
            for(std::list<product>::iterator itr=DataBase->product_list.begin();itr!=DataBase->product_list.end();itr++)
            {
                if (ItemList[i] != 0)
                {
                    cout<<"Product ID: "<<(*itr).product_id;
                    cout<<"\nProduct Name: "<<(*itr).product_name;
                    cout<<"\nPrice: Rs. "<<(*itr).price;
                    cout<<"\nQuantity in Order: "<<ItemList[i];
                    cout<<"\n\n";
                }
                i++;
            }
            cout<<"\nTotal: "<<OrderTotal;
            cout<<"\nStatus: "<<OrderStatus;
        }
};


class ShoppingCart;

class master : public Data
{
    ShoppingCart* obj;
    public:
        void ProductInit();
        void LoginScreen();
        void menu0();
        void menu1();
        void menu2();
        void  GoToPayment();
};



class ShoppingCart 
{
    friend class Customer;
    Data* DataBase;

public:
    int productQuantity[MAX_Products] = {0};
    virtual void NewOrder()=0;
    // virtual void display(int i){};
    virtual void display(string s)=0;


    virtual void display(){
        int i = 0;
        cout<<"\n\nItems in cart :-\n\n";
        for(std::list<product>::iterator itr=DataBase->product_list.begin();itr!=DataBase->product_list.end();itr++)
        {
            if (productQuantity[i] != 0)
            {
                cout<<"Product ID: "<<(*itr).product_id;
                cout<<"\nProduct Name: "<<(*itr).product_name;
                cout<<"\nPrice: Rs. "<<(*itr).price;
                cout<<"\nQuantity in cart: "<<productQuantity[i];
                cout<<"\n\n";
            }
            i++;
        }
        cout<<"Total: "<<GetTotal()<<"\n\n";
    }
    void UpdateQuantity(int i, int quan)
    {productQuantity[i] = quan;}

    int GetTotal(){
        int i = 0, sum = 0;
        for(std::list<product>::iterator itr=DataBase->product_list.begin();itr!=DataBase->product_list.end();itr++)
        {
            sum += productQuantity[i]*((*itr).price);
            i++;
        }
        return sum;
    }
};





class Customer: User, public ShoppingCart
{

    friend class Data;

    string Customer_Name;
    string Address;
    string Email;
    Order Orders[10];
    int OrderCount = 0;
    Data* dt;

public:
    Customer(){};// this is the defualt constructor
    bool Login(int userID, string password)
    {return VerifyLogin(userID, password);}


    void Register(string name, string address, string email, int id, string password, Data* dt)
    {
        Customer_Name = name;
        Address = address;
        Email = email;
        SetUserID<int, 1001>(id);
        SetPassword<string>(password);
        this->dt = dt;
        DataBase = dt;
    }


    void NewOrder()
    {
        Orders[OrderCount].SetOrderDetails (OrderCount + 1, GetTotal(), productQuantity, Address, dt);
        cout<<"The details of your order are: "<<endl;
        display(OrderCount);
        OrderCount++;

        for (int i = 0; i < MAX_Products; i++)
            UpdateQuantity(i, 0);
    }


    void display(int i)
    {Orders[i].DisplayOrderDetails();}

    void display(string x)
    {
        if(OrderCount==0)
        {
            cout<<"No order history found"<<endl;
            return;
        }
        for (int i = 0; i < OrderCount; i++)
        {
            cout<<setw(50)<<setfill('-')<<'-'<<endl;
            Orders[i].DisplayOrderDetails();
            cout<<"\n";
            cout<<setw(50)<<setfill('-')<<'-'<<endl;
            
        }
        return;
        
    }

};





void Data::add_customer(string name, string address, string email, string password)
{
    Customer*cust=new Customer();
    cust->Register(name, address, email, customer_Count, password, this);
    customer_list.push_back(*cust);
    customer_Count++;
}

void Data::add_customer()
{
    
    string name, address, email, password;
    cout<<"\nEnter the customer name: ";
    cin.ignore();
    getline(cin, name);
    cout<<"Enter the customer address: ";
    cin.ignore();
    getline(cin, address); 
    cout<<"Enter the customer email: ";
    cin.ignore();
    getline(cin, email);
    cout<<"Enter the password that you want to create: ";
    cin>>password;
    
    cout<<"\nYour User ID is: "<<customer_Count+1001<<"\nPlease remember this for future reference.\n\n";
    cout<<"Login with your new User ID and Password  to continue.";
    
    Customer*cust=new Customer();
    cust->Register(name, address, email, customer_Count, password, this);
    customer_list.push_back(*cust);
    customer_Count++;

}


void Data::getCustomer()
{
    cout<<setw(50)<<setfill('-')<<'-'<<endl;
    std::list<Customer>::iterator itr=customer_list.end();
    cout<<"Welcome "<<(*--itr).Customer_Name<<"to our ecommerce platform!!!!!"<<endl;
    cout<<setw(50)<<setfill('-')<<'-'<<endl;

}


Customer* Data::VerifyUserLogin(int id,string pass)
{
    for(std::list<Customer>::iterator itr=customer_list.begin();itr!=customer_list.end();itr++)
    {
        if((*itr).Login(id,pass)==true)
        {
            Customer* ptr;
            ptr = &(*itr);
            cout<<"Welcome, "<<(*itr).Customer_Name<<"!"<<endl;
            return ptr; // this will call login function for every Customer object in the Customer list....if true is returned i.e. the id and password match, then this will also return true

        }
    }
    return NULL;
}





template <class T>
class payment
{
    T choice;
    T total;
    bool status = false;

public:
    payment(T tot)
    {
        total=tot;
    }
    void getchoice();
    void upi()
    {
        char n[6];
        cout << "Your total bill is: " << total << endl;
        cout << "To pay via upi:" << endl
             << "Enter your upi id:";
        for(int i=0;i<6;i++)
        {
            n[i]=getch();
            printf("*");
        }
 
        cout<<endl;;
        status = true;
    }
    void card()
    {
        int i;
        char j[3];
        cout << "Your total bill is: " << total << endl;
        cout << "To pay via card:" << endl
             << "Enter your card number:" << endl;
        cin >> i;
        cout << "Enter CVV:";
        for(int k=0;i<3;i++)
        {
            j[k]=getch();
            printf("*");
        }
 
    cout<<endl;
        status = true;
    }
    void cash()
    {
        cout << "Your total bill is: " << total << endl;
        status = true;
    }
    bool getstatus()
    {
        return status;
    }
};
template <class T>
void payment<T>::getchoice()
{
    cout << "Enter 1 for upi, 2 for card and 3 for cash:" << endl;
    cin >> choice;
    if (choice == 1) upi();
    else if (choice == 2) card();
    else if (choice == 3) cash();
    else cout << "Invalid choice !";

    if (status == true)
    {
        system("cls");
        cout << "Thank You for the payment! Your order has been confirmed and you will receive it within a few days..."<<endl;;
    }
}





void master::menu0()
{
    int ans;
    cout<<"1. Display all products"<<endl;
    cout<<"2. See shopping cart"<<endl;
    cout<<"3. See order History"<<endl;
    cout<<"4. Logout "<<endl;
    cout<<"Enter Choice: ";
    cin>>ans;
    switch(ans)
    {
    case 1:
        menu1();
    
        break;
    case 2:
        obj->display();
        menu2();
        break;
    case 4:
        exit(0);
        break;
    case 3:
        obj->display("all");
        cout<<"Press any key to go back to main menu"<<endl;
        getch();
        menu0();

    }
}

void master::menu1()
{
    int id;
    int choice;
    system("cls");
    DisplayAllProducts();
    ch:
    cout<<"1. Add a product to your cart"<<endl;
    cout<<"2. Go back to main menu"<<endl;
    cout<<"\nEnter choice: ";
    cin>>choice;
    try
    {
        if(choice!=1 && choice!=2)
        {
            throw "Invalid choice";
            
        }
    }
    catch(const char*excep)
    {
        cerr<<excep<<endl;
        goto ch;
    }  
    if(choice==2)
    {
        system("cls");
        menu0();
        return;
    }
    cout<<"Enter product ID: ";
    cin>>id;
    cout<<endl;
    std::list<product>::iterator itr=product_list.begin();
    advance(itr,id-1);
    //stores quantity 
    int quantity_avail=(*itr).quantity_available;
    cout<<"Quantity Available: "<<quantity_avail<<endl;
    cout<<"Do you want to add to cart ?"<<endl;
    cout<<"1. Yes"<<endl;
    cout<<"2. No"<<endl;
    cout<<"\nEnter choice: ";
    
    cin>>choice;
    
    if(choice==1)
    {
        int quantity;
        cout<<"Enter the quantity: ";
        cin>>quantity;
        cout<<endl;
        if(quantity>quantity_avail)
        {
            cout<<"Not available in the required quantity "<<endl;
        }
        else{
            obj->UpdateQuantity(id-1,quantity);
            (*itr).quantity_available-=quantity;
            cout<<"Successfully Added to cart !!!"<<endl;
        }
        getch();
        menu1();
    }
    else if(choice==2)
    {
        getch();
        menu1();
    }
}

void master::LoginScreen(){
    string ans;
    cout<<"Are you an existing Customer(y/n)? ";
    cin>>ans;
    cout<<endl;
    if(ans=="n")
    {
        cout<<"Creating new profile:"<<endl;
        cout<<"Please enter the details given below :-"<<endl;
        add_customer();
        getch();
        system("cls");
        LoginScreen(); 
    }
    else if(ans=="y")
    {
        int id;
        string pass;
        cout<<"Please enter your user id: ";
        cin>>id;
        cout<<"Please enter your password: ";
        cin>>pass;
        system("cls");
        obj=VerifyUserLogin(id,pass);// this will take to next step if this returns true
        if (obj==NULL)
        {
            cout<<"Incorrect User ID and/or Password. "<<endl;
            cout<<"Please try again using different User ID and Password, or Register a new account.";
            cout<<"\n\nPress any key to continue...";
            getch();
            system("cls");
            LoginScreen();
        }
        menu0();
    }
}


void master::ProductInit()
{
    //* creating default Customer base
    
    ifstream cus;
    cus.open("customer_file.txt");
    string str;
    getline (cus, str);
    stringstream ss;
    ss<<str;
    int num;
    ss>>num; ss.clear();

    for(int i = 0; i < num; i++)
    {
        getline(cus, str);
        string name, address, email, pass;
        getline (cus, name);
        getline (cus, address);
        getline (cus, email);
        getline (cus, pass);
        add_customer(name, address, email, pass);
    }
    cus.close();


    ifstream pr;
    pr.open("product_file.txt");
    getline (pr, str);
    ss<<str;
    ss>>num; ss.clear();

    for(int i = 0; i < num; i++)
    {
        getline(pr, str);
        int id, price, quan;
        string name, desc;
        product* p = new product;
        product_list.push_back(*p);
        std::list<product>::iterator itr=product_list.end();
        itr--;
        getline (pr, str);
        ss<<str;
        ss>>id; ss.clear();
        (*itr).product_id = id;
        getline (pr, name);
        (*itr).product_name = name;
        getline (pr,str);
        ss<<str;
        ss>>price; ss.clear();
        (*itr).price = price;
        getline (pr, desc);
        (*itr).description = desc;
        getline (pr, str);
        ss<<str;
        ss>>quan; ss.clear();
        (*itr).quantity_available = quan;
    }
    pr.close();
        
}


void master::menu2()
{
    int choice;
    cout<<"1. Proceed to payment"<<endl;
    cout<<"2. Add more products"<<endl;
    cout<<"3. Update quantity"<<endl;
    cout<<"4. Go back to main menu"<<endl;
    cout<<"\nEnter choice: ";
    cin>>choice;
    if(choice==1)
    {
        GoToPayment();
    }
    else if(choice==2)
    {
        menu1();
    }
    else if(choice==3)
    {
        int id,quantity,quantity_difference;
        cout<<"Enter the Id of the product whose quantity you want to update: ";
        cin>>id;
        cout<<"Enter the new quantity: ";
        cin>>quantity;
        quantity_difference = quantity-obj->productQuantity[id-1];
        std::list<product>::iterator itr=product_list.begin();
        advance(itr,id-1);
        int quantity_avail=(*itr).quantity_available;
        if(quantity_difference>quantity_avail)
        {
            cout<<"not available in the required quantity "<<endl;
        }
        else
        {
            obj->UpdateQuantity(id-1,quantity);
            (*itr).quantity_available-=quantity_difference;
            cout<<"Successfully Added to cart !!!"<<endl;
        }
        getch();
        obj->display();
        menu2();
    }
    else if(choice==4)
    {
        system("cls");
        menu0();
    }   
}
void master::GoToPayment()
{
    payment<int> pay(obj->GetTotal());
    pay.getchoice();
    if(pay.getstatus()==true)
    {
        obj->NewOrder();
        cout<<"\nPress any key to go back to main menu"<<endl;
        getch();
        menu0();

    }
}



int main()
{

    master object;
    object.ProductInit();
    object.LoginScreen();
    return 0;
}
