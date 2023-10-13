#include<bits/stdc++.h>
using namespace std;
#include<conio.h>
#include<stdlib.h>


struct product
{
    //this structure contains list of all products
    int cat1;
    int cat2;
    string name;
    int amount;
    int quantity;
    int sold;
    int times;
    struct product* next;
};

struct customerProductList
{
    //this structure contains list of wishlist and purchased items for consumers
    int cat1;
    int cat2;
    string name;
    int amount;
    int buyed;
    int times;
    struct customerProductList* next;
};

struct payment
{
    //this structure stores amount user had in different payment method
    float BuyNowPayLater;
    float upi;
    float debitCard;
    float cashback;
};

struct customers
{
    //this structure contains list of customers who have subscribed online_bazaar
    int id;
    string emailId;
    string name;
    string address;
    string phoneNo;
    payment balance;
    int times;
    customerProductList* wishlist;
    customerProductList* purchase;
    struct customers* next;
};

vector<vector<product*>> GPUL{};
vector<customers*> GCPL{};
vector<int> globalId{};
vector<string> globalemailId{};
// bool cmp(pair<customerProductList*,int> &p1,pair<customerProductList*,int> &p2)
// {
//     return p1.second >= p2.second;
// }

// bool cmp1(pair<product*,int> &p1,pair<product*,int> &p2)
// {
//     return p1.second>p2.second;
// }

// bool cmp2(pair<customers*,int> &p1,pair<customers*,int> &p2)
// {
//     return p1.second>p2.second;
// }

void createProduct()
{
    // create product list
    GPUL.resize(6);
    int temp[6]={3,2,2,3,3,3};
    for(int i=0;i<6;i++)
    {
        GPUL[i].resize(temp[i]);
        for(int j=0;j<temp[i];j++)
        {
            GPUL[i][j]=NULL;
        }
    }
}

void GlobalProductList()
{
    // read the input from product txt file
    ifstream productfile("Z:/Prateek/codes and contest/c++/online bazaar/Product_file.txt");
    
    int cat1,cat2,amount,quantity,sold;
    string name;
    product* products;
    while(productfile >> cat1 >> cat2 >> name >> amount >> quantity >> sold)
    {
        products = new product();
        products->cat1=cat1;
        products->cat2=cat2;
        products->amount=amount;
        products->name=name;
        products->sold=sold;
        products->quantity=quantity;
        products->times=0;
        products->next=NULL;

        // cout<<cat1<<" "<<cat2<<endl;
        if(GPUL[cat1][cat2]==NULL)
        {
            GPUL[cat1][cat2]=products;
        }
        else
        {
            products->next=GPUL[cat1][cat2];
            GPUL[cat1][cat2]=products;
        }
    }
    return;
}

void createCustomer()
{
    // create customer list
    GCPL.resize(100);
    for(int i=0;i<100;i++)
    {
        GCPL[i]=NULL;
    } 
    return;
}
    
void GlobalCustomerList()
{
    // read the input of customers from customers txt file
    ifstream customerfile("Z:/Prateek/codes and contest/c++/online bazaar/customer_file.txt");

    int id,bnpl,upi,debit;
    string name,email_id,address,phone_no;
    customers* cpl;
    customerProductList* wishlist;
    customerProductList* purchase;
    while(customerfile>>id>>name>>email_id>>address>>phone_no>>bnpl>>upi>>debit)
    {
        cpl=new customers();
        purchase = new customerProductList();
        wishlist = new customerProductList();
        purchase = NULL;
        wishlist = NULL;
        cpl->id = id;
        globalId.push_back(id);
        cpl->name=name;
        cpl->emailId=email_id;
        globalemailId.push_back(email_id);
        cpl->address=address;
        cpl->phoneNo=phone_no;
        cpl->balance.BuyNowPayLater=bnpl;
        cpl->balance.upi=upi;
        cpl->balance.debitCard=debit;
        cpl->balance.cashback=0;
        cpl->wishlist=wishlist;
        cpl->purchase=purchase;
        cpl->times=0;
        cpl->next=NULL;
        if(GCPL[id%100]==NULL)
        {
            GCPL[id%100]=cpl;
        }
        else
        {
            cpl->next=GCPL[id%100];
            GCPL[id%100]=cpl;
        }
    }
    return;
}

customers* getCustomer(int id,string emailId)
{
        customers* customer=GCPL[id%100];
        if(customer==NULL) return NULL;
        while(customer)
        {
            if(customer->id == id && customer->emailId==emailId) break;
            customer=customer->next;
        }
        return customer;
}

product* getProduct(int cat1, int cat2, string name)
{
    product* products=GPUL[cat1][cat2];
    if(products==NULL) return NULL;
    while(products)
    {
        if(products->name == name) break;
        products=products->next;
    }
    return products;
}

bool checkWishlistOrderlist(customerProductList* list, int cat1, int cat2, string name)
{
    if(list) return false;
    while(list)
    {
        if(list->cat1 == cat1 && list->cat2 == cat2 && list->name == name)
        {
            return true;
        }
        list=list->next;
    }
    return false;
}

void removeFromWishlist(customerProductList* wishlist, int cat1, int cat2, string name)
{
    //remove item from wishlist of the user 
    if(wishlist == NULL) 
    {
        cout<<"Item dosn't "<<endl;
        return;
    }
    customerProductList* temp=wishlist;
    customerProductList* prev=NULL;
    while(temp)
    {
        if(temp->cat1==cat1 && temp->cat2 == cat2 && temp->name==name){
            if(prev==NULL)
            {
                wishlist=wishlist->next;
                free(temp);
                return;
            }
            prev->next=temp->next;
            free(temp);
            return;
        }
        prev=temp;
        temp=temp->next;
    } 
    cout<<"Didn't find the "<<name<<" in your wishlist."<<endl;
    return;
}

void AddToWishlist(int id, string emailId, product* products)
{
    //ADDWishList(customer ID, product)
    //adds product to the wishlist
    //it does not increment or decrement the count of product quantity from product list
    customers* customer=getCustomer(id,emailId);
    if(customer == NULL){
        cout<<"No customer exist with that id - "<<id<<" emailID - "<<emailId<<". Please register.";
        return;
    } 
    if(!checkWishlistOrderlist(customer->wishlist, products->cat1, products->cat2, products->name))
    {
        customerProductList* pduct = new customerProductList();
        pduct->name=products->name;
        pduct->cat1=products->cat1;
        pduct->cat2=products->cat2;
        pduct->times=0;
        pduct->amount=products->amount;
        pduct->next=customer->wishlist;
        customer->wishlist=pduct;
        cout<<"Items added to wishlist successfully."<<endl;
    }
}

void order(int id, string emailId, string productName, int quantity)
{                                       //address
    // order(customer name/id/phone/,product name, quantity, address, email id)
    //it places the order on the basis of availability
    //once order is placed count of product should decrease
    
    for(int i=0;i<6;i++)
    {
        int x=GPUL[i].size();
        for(int j=0;j<x;j++)
        {
            product* it = GPUL[i][j];
            while(it)
            {
                if(it->name == productName)
                {
                    if(it->quantity == 0 )
                    {
                        cout<<"Thanks for your order! Unfortunately, the following item " <<it->name<<" from your order is out of stock."<<endl;
                        return ;
                    }
                    else if(it->quantity < quantity)
                    {
                        cout<<"Thanks for your order! Unfortunately, the following item " <<it->name<<" quantity ordered is more than we have in stock.";
                        return ;
                    }
                    else 
                    {
                        customers* temp=GCPL[id%100];
                        while(temp)
                        {
                            if(temp->emailId == emailId)
                            {
                                int toPay =  it->amount - temp->balance.cashback;
                                if(toPay > 0)
                                {
                                    temp->balance.cashback=0;
                                    if(temp->balance.BuyNowPayLater > toPay)
                                        temp->balance.BuyNowPayLater -= toPay;                               
                                    else if(temp->balance.debitCard > toPay)
                                        temp->balance.debitCard -= toPay;
                                    else if(temp->balance.upi > toPay)
                                        temp->balance.upi -= toPay;
                                    else
                                    {
                                        cout<<"Insufficient Amount / Your Balance is Low."<<endl;
                                        return;
                                    }                               
                                }
                                else temp->balance.cashback -= toPay;
                                it->quantity -= quantity;
                                it->sold += quantity;
                                temp->times += 1;
                                it->times +=1;
                                customerProductList* wl=temp->wishlist;
                                removeFromWishlist(wl,it->cat1,it->cat2,it->name);
                                if(temp->purchase != NULL)
                                {
                                        customerProductList* a=temp->purchase;
                                        while(a)
                                        {
                                            if(a->name == it->name)
                                            {    
                                                a->times += 1;
                                                a->buyed +=quantity;
                                                cout<<"Order placed successfully."<<endl;
                                                return;
                                            }    
                                        }
                                }
                                else 
                                {
                                    customerProductList* p=new customerProductList();
                                    p->times=1;
                                    p->amount = it->amount;
                                    p->cat1 = it->cat1;
                                    p->cat2 = it->cat2;
                                    p->name = it->name;
                                    p->buyed = quantity;
                                    p->next = NULL;
                                    if(temp->purchase == NULL) temp->purchase=p;
                                    else 
                                    {
                                        p->next=temp->purchase;
                                        temp->purchase=p;
                                    }   
                                    cout<<"Order Placed successfully."<<endl; 
                                    return;
                                }                                    
                            }
                            temp=temp->next;
                        }  
                    }
                }
                it=it->next;
            }
            
        }
    }
}

void returnProduct(int id, string emailId, int cat1, int cat2, string name, int quantity)
{
    customers* customer=getCustomer(id,emailId);
    customerProductList* order=customer->purchase;
    customerProductList* prev=NULL;
    product* pduct=getProduct(cat1,cat2,name);
    while(order)
    {
        if(order->name == name && order->cat1 == cat1 && order->cat2 == cat2)
        {
            if(order->buyed < quantity)
            {
                cout<<"We are sorry that we can't take your return order as returned quantity is more than what you have ordered."<<endl;
                return;
            }
            else if(order->buyed > quantity)
            {
                order->buyed -= quantity;
                customer->times -= 1;
                customer->balance.cashback += order->amount;
                order->times -= 1;
                pduct->sold  -= quantity;
                pduct->quantity +=quantity;
                pduct->times -= 1;
                cout<<"Return order placed successfully."<<endl;
                return;
            }
            else
            {   
                customer->times -= 1;
                customer->balance.cashback += order->amount;
                if(prev==NULL)
                {
                    customer->purchase=customer->purchase->next;
                    order->next=NULL;
                    free(order);
                    return;
                }
                else
                {
                    prev->next = order->next;
                    order->next=NULL;
                    free(order);
                }
                pduct->sold  -= quantity;
                pduct->quantity +=quantity;
                pduct->times -= 1;
                cout<<"Return order placed successfully."<<endl;
                return;
            }
        }
        prev=order;
        order=order->next;
    }
    cout<<"You havn't placed an order of "<<name<<"."<<endl;
    
}

void printWishlist(int id,string emailId)
{
    customers* customer=getCustomer(id,emailId);
    if(customer->wishlist==NULL)
    {
        cout<<"Your wishlist is empty."<<endl;
    }
    customerProductList* temp=customer->wishlist;
    cout<<"Wishlist - "<<endl;
    while(temp)
    {
        cout<<temp->cat1<<" "<<temp->cat2<<" "<<temp->name<<" "<<temp->amount<<" "<<endl; 
        temp=temp->next;
    }
}

void printOrderList(int id,string emailId)
{
    customers* customer=getCustomer(id,emailId);
    if(customer->purchase==NULL)
    {
        cout<<"You havn't purchased anything yet/Order history is empty."<<endl;
        return;
    }
    customerProductList* temp=customer->purchase;
    while(temp)
    {
        cout<<"name - "<<temp->name<<" cat1 "<<temp->cat1<<" cat2 "<<temp->cat2<<" amount - "<<temp->amount<<endl;       
        temp=temp->next;
    }
    return;
}

void AvailableProductList()
{
    product* tempProduct;
    for(int i=0;i<6;i++)
    {
        int x=GPUL[i].size();
        for(int j=0;j<x;j++)
        {
            tempProduct=GPUL[i][j];
            while(tempProduct)
            {

                if(tempProduct->quantity > 0)
                {
                    cout<<"cat1 - "<<tempProduct->cat1<<"  cat2 - "<<tempProduct->cat2<<"  name - "<<tempProduct->name;
                    for(int i=tempProduct->name.size();i<30;i++) cout<<" ";
                    cout<<"Amount - "<<tempProduct->amount;
                    if(tempProduct->quantity<10) cout<<" ";
                    cout<<"  Available Quantity - "<<tempProduct->quantity<<endl;
                }
                tempProduct=tempProduct->next;
            }
        }
    }
}

void printCustomerInfo(int id,string emailId)
{
    customers* customer=getCustomer(id,emailId);
    if(customer) cout<<"No customer exist with id = "<<id<<" and emailId = "<<emailId<<endl;
    cout<<"Id - "<<customer->id<<"\nemail id - "<<customer->emailId<<"\nname - "<<customer->name<<" \naddress - "<<customer->address<<" \nonline shopping purchase frequency - "<<customer->times<<endl;
    cout<<"Purchase history - "<<endl;
    customerProductList* temp=customer->purchase;
    if(temp) cout<<" No Purchase history."<<endl;
    else
    {
        while(temp)
        {
            cout<<"name - "<<temp->name<<" cat1 "<<temp->cat1<<" cat2 "<<temp->cat2<<" amount - "<<temp->amount<<endl;       
            temp=temp->next;
        }
    }
    return;
}

void itemCustomerFrequency(int id,string emailId)
{
    customers* customer=getCustomer(id,emailId);
    vector<pair<int,customerProductList*>> frequencyMap{};
    customerProductList* temp=customer->purchase;
    if(temp) cout<<"User hasn't buyed anything yet."<<endl;
    while(temp)
    {
        frequencyMap.push_back({temp->times,temp});
        temp=temp->next;
    }
    sort(frequencyMap.begin(),frequencyMap.end());
    for(auto it=frequencyMap.end()-1;it>=frequencyMap.begin();it--)
    {
        cout<<"frequency buyed - "<<it->first<<"   product name - "<<it->second->name<<endl;
    }
    return;
}

void allProductsFrequency()
{
    product* pduct;
    vector<pair<int,product*>> pf{};
    for(int i=0;i<6;i++)
    {
        int x=GPUL[i].size();
        for(int j=0;j<x;j++)
        {
            pduct=GPUL[i][j];
            while(pduct->next)
            {
                pf.push_back({pduct->times,pduct});
                pduct=pduct->next;
            }
        }
    }
    sort(pf.begin(),pf.end());
    for(auto it:pf)
    {
        cout<<"product name - "<<it.second->name;
        int n=it.second->name.size();
        for(int i=n;i<30;i++) cout<<" ";
        cout<<"frequency buyed - "<<it.first<<endl;
    }
    return;
}

void purchaseFrequencyAllCustomer()
{
    customers* customer;
    vector<pair<int,customers*>> pfc{};
    for(auto it:GCPL)
    {
        customer=it;
        while(customer)
        {
            if(customer->times>0) pfc.push_back({customer->times,customer});
            customer=customer->next;
        }
    }
    if(pfc.empty()) 
    {
        cout<<"No Products has been sold yet.";
        return;
    }
    sort(pfc.begin(),pfc.end());
    for(auto it=pfc.end()-1;it>=pfc.begin();it--)
    {
        if(it->first > 0)
        {
            cout<<"customer id - "<<it->second->id<<" customer name - "<<it->second->name;
            int n=it->second->name.size();
            for(int i=n;i<30;i++) cout<<" ";
            cout<<"times - "<<it->first<<endl;
        }
    }
}

void createUserAccount()
{
    cout<<"User Registration - \n";
    customers* customer=new customers();
    while(true)
    {
        //user id check
        cout<<"Enter customer id - ";
        cin>>customer->id;
        int i,n=globalId.size();
        for(i=0;i<n;i++)
        {         
            if(customer->id == globalId[i]) 
            {
                cout<<"Id already exists. Please try again.";
                break;
            }
        }
        if( i == n) break;
    }

    while(true)
    {
        //email id check
        cout<<"\nEnter the email id - ";
        cin>>customer->emailId;
        int i,n=globalemailId.size();
        for(i=0;i<n;i++)
        {         
            if(customer->emailId == globalemailId[i]) 
            {
                cout<<"Email_Id already exists. Please try again.";
                break;
            }
        }
        if(i==n) break;
    }
    
    cout<<"\nEnter the name - ";
    cin>>customer->name;
    cout<<"Enter the address - ";
    cin>>customer->address;
    cout<<"Enter the phone no - ";
    cin>>customer->phoneNo;
    cout<<"Enter the upi amount - ";
    cin>>customer->balance.upi;
    cout<<"Enter the Buy Now Pay Later Amount - ";
    cin>>customer->balance.BuyNowPayLater;
    cout<<"Enter the debit card amount - ";
    cin>>customer->balance.debitCard;
    customer->next=NULL;
    customer->times=0;
    customer->wishlist=NULL;
    customer->purchase=NULL;

    if(GCPL[customer->id%100])
    {
        GCPL[customer->id%100]=customer;
    }
    else
    {
        customer->next=GCPL[customer->id%100];
        GCPL[customer->id%100]=customer;
    }
    fstream customerFile;
    // int line_no;
    customerFile.open("Z:/Prateek/c++ codes/online bazaar/customer_file.txt",fstream::app);
    // while(getline(customerFile,line_no));
    customerFile<<customer->id<<"   "<<customer->name<<"    "<<customer->emailId<<"     "<<customer->address<<"     "<<customer->phoneNo<<"     "<<customer->balance.BuyNowPayLater<<"      "<<customer->balance.upi<<"     "<<customer->balance.debitCard;
    customerFile.close();
}

void userWindow(int id,string emailId)
{
    while(true)
    {
        system("cls");
        customers* customer=getCustomer(id,emailId);
        // order purchase(return) wishlist(remove from wishlist)  
        cout<<"\n1. List of all Products\n2.Order Item\n3.Wishlist\n4.Order History\n9.Exit\nEnter input";
        int a;
        cin>>a;
        if(a==1)
        {
            system("cls");
            AvailableProductList();
            while(true)
            {
            cout<<"\n1. Add to wishlist\n2. order item\n9. Exit\nEnter - ";
            int c;
            cin>>c;
            if(c==1)
            {
                int cat1,cat2;
                string name;
                cout<<"Enter cat1 - ";
                cin>>cat1;
                cout<<"\t\tEnter cat2 - ";
                cin>>cat2;
                cout<<"\t\tEnter Product name - ";
                cin>>name;
                product* pduct=getProduct(cat1,cat2,name);
                AddToWishlist(id,emailId,pduct);
            }
            else if(c==2)
            {   
                cout<<"\nEnter name of product - "; 
                string name;
                cin>>name;
                cout<<"\t\t\tquantity to buy - ";
                int quantity;
                cin>>quantity;
                order(id,emailId,name,quantity);
            }
            else if(c==9)
            {
                break;
            }
            else
            {
                cout<<"\nwrong key entered.";
            }
            char ch=getch();
            while(ch!='9') ch=getch();
            }
        }
        else if(a==2)
        {
            system("cls");
            AvailableProductList();
            cout<<"\nEnter name of product - "; 
            string name;
            cin>>name;
            cout<<" quantity to buy - ";
            int quantity;
            cin>>quantity;
            order(id,emailId,name,quantity);
        }   
        else if(a==3)
        {
            system("cls");
            printWishlist(id,emailId);
            if(customer->wishlist !=NULL) 
            {
                while(true)
            {
                cout<<"1. To remove item from wishlist.\n2. order item from wishlist\n9. return to previous window.\nEnter - ";
                int b;
                cin>>b;
                if(b==1)
                {   
                    int cat1,cat2;
                    string name;
                    cout<<"Enter cat1 - ";
                    cin>>cat1;
                    cout<<"\t\tEnter cat2 - ";
                    cin>>cat2;
                    cout<<"\t\tEnter Product name - ";
                    cin>>name;
                    removeFromWishlist(customer->wishlist,cat1,cat2,name);
                }
                else if(b==2)
                {
                    int quan;
                    string name;
                    cout<<"\t\tEnter Product name - ";
                    cin>>name;
                    cout<<"Enter quantity to order - ";
                    cin>>quan;
                    order(id,emailId,name,quan);
                }
                else if(b==9) break;
                else cout<<"wrong key endtered enter again - ";
                char ch=getch();
                while(ch!='9') ch=getch();
            } 
            } 
        }
        else if(a==4)
        {
            system("cls");
            printOrderList(id,emailId);
            if(customer->purchase!=NULL)
            {
                while(true)
                {
                    cout<<"\n1. Return Order\n9.Exit\nEnter - ";
                    int a;
                    cin>>a;
                    if(a==1)
                    {
                        int cat1,cat2,quan;
                        string name;
                        cout<<"Enter cat1 - ";
                        cin>>cat1;
                        cout<<"\t\tEnter cat2 - ";
                        cin>>cat2;
                        cout<<"\t\tEnter Product name - ";
                        cin>>name;
                        cout<<"\t\tEnter quantity to return - ";
                        cin>>quan;
                        returnProduct(id,emailId,cat1,cat2,name,quan);
                    }
                    else if(a==9) break;
                    else cout<<"\nWrong key entered.";
                    cout<<"\nPress 9 to go back to previous menu.";
                    char ch=getch();
                    while(ch!='9') ch=getch();
                }
            }
        }
        else if(a==9)
        {
            break;
        }
        else
        {
            cout<<"\nWrong key entered. Enter Again.";
            getch();
            continue;
        }
        cout<<"\nPress 9 to go to previous menu  ";
        char ch=getch();
        while(ch!='9') ch=getch();
    }
}

void loginWindow()
{
    while(true)
    {
        system("cls");
        int id;
        string emailId;
        cout<<"Enter your consumer Id - ";
        cin>>id;
        cout<<"\nEnter your email id - ";
        cin>>emailId;
        if( find(globalId.begin(),globalId.end(),id) != globalId.end()  &&  find(globalemailId.begin(),globalemailId.end(),emailId) != globalemailId.end())
        {
            cout<<"\nLogged in successfully.";
            userWindow(id,emailId);
            getch();
        }
        else
        {
            cout<<"\n\nIncorrect customer id or emailid entered \nPress 1 to enter again and 9 to exit - ";
            int x;
            cin>>x;
            if(x==1) continue;
            else if(x==9) break;
        }
    }    
    // for(i=0;i<n;i++)
    // {         
    //     if(id == globalId[i]) 
    //     {
    //         cout<<"Id already exi. Please try again.";
    //         break;
    //     }
    // }

}

void functionwindow()
{
    system("cls");
    string x="";
    cout<<"Enter password - ";
    char ch=getch();
    while(x.size()<7)
    {
        x+=ch;
        cout<<"*";
        ch=getch();
    }
    cout<<"\n";
    if(x=="oprolex")
    {
        while(true)
        {
            system("cls");
            int a;
            cout<<"1. Print particular customer info\n2. List of all available products\n3. List of Frequent Customers\n4. Items purchased frequently by a customer\n5. No. of times each product sold\n9. Exit";
            cout<<"\n Enter - ";
            cin>>a;
            if(a==1)
            {
                cout<<"Information about particular customer \n";
                system("cls");
                int id;
                string emailId;
                cout<<"Enter the customer id - ";
                cin>>id;
                cout<<"Enter email id - ";
                cin>>emailId;
                printCustomerInfo(id,emailId);
            }   
            else if(a==2)
            {
                system("cls");
                cout<<"---------------Available List Of all Products-------------------\n";
                AvailableProductList();
            }
            else if(a==3)
            {
                system("cls");
                cout<<"----------------Every customer shopping frequency--------------------\n";
                purchaseFrequencyAllCustomer();
            }
            else if(a==4)
            {
                system("cls");
                cout<<"---------------Items purchased frequently by a customer-------------------";               
                int id;
                string emailId;
                cout<<"Enter the customer id - ";
                cin>>id;
                cout<<"Enter email id - ";
                cin>>emailId;
                itemCustomerFrequency(id,emailId);
            }
            else if(a==5)
            {
                system("cls");
                cout<<"----------------Frequency of each product sold-----------------------------";
                allProductsFrequency();
            }
            else if(a==9)
            {
                break;
            }
            else
            {
                cout<<"wrong input";
                continue;
            }
            cout<<"\n\n\n____________________________enter 9 to exit to previous window.______________________________\n";
            char ch=getch();
            while(ch != '9') ch=getch();
        }
    }
    else
    {
        cout<<"wrong password entered.";
        cout<<"Returning to previous window.";
    }
    cout<<"--------------------------------------Exiting---------------------------------------------";
}

int main()
{
    // main function
    createProduct();
    createCustomer();
    GlobalProductList();
    GlobalCustomerList();

    while(true)
    {
        system("cls");
        int a;
        cout<<"\t\t\t\t\t\tWelcome To Online Bazaar\n\n";
        cout<<"1-New User\n2-User Login\n3-Member Login(personal)\n9-Exit\nEnter - "; 
        cin>>a;
        if(a==1)
        {
            createUserAccount();
            cout<<"\nAccount created Successfully. Welcome to Online Bazaar.";
            cout<<"\nPress 9 to return to login window.\n";
        }
        else if(a==2)
        {
            loginWindow();
        }
        else if(a==3)
        {
            functionwindow();
        }
        else if(a==9)
        {
            break;
        }
        else
        {
            cout<<"Wrong input. Press 9 to Enter Again.";
        } 
        char ch=getch();
        while(ch != '9') ch=getch();
    }
    cout<<"\n\n\n\t\t\t\t\tThanks for visiting and shopping from online bazaar.\n\n\n";
    return 0;
}    

// void addProduct()
// {
//     cout<<"Adding Product - \n";
//     product* pduct=new product();
//     while(true)
//     {
//         while(true)
//         {
//             cout<<"Enter the main category - \n";
//             cin>>pduct->cat1;
//             if(pduct->cat1<0 || pduct->cat1>5)
//             {
//                 cout<<"Invalid category entered. Enter again.\n\n\n\n"
                 
//             }
//         }
//     } 

// }

//online shopping system to be made using linked list and oops concepts.
// product - cost, category, number of available items etc. ////done

// registered customer details name phone_no address along with history of purchases /////done

// favourite purchases(items which are purchased more often) /////done

// wishlist for every user /////done

//ReturnProduct(customer ID, product)  ///////done
    //it increments the count of product in product list for that product id.
    //cashback the amount of product to user account 

//following functions can be supported 
    //print wish list for a user - //done
    //print product list for available products - //done
    //print customer information for a given customer - //done 
    //print the items those purchased frequently by a customer - //done
    //print the items purchased frequently by all customers - //done
    //print the customer list who purchase more often or print list of 
         //customers in decreasing order of their frequency of purchases - //done        

// favourite payment option 

// after creating a user account, user is taken to the screen 
    // where find all the category products. 