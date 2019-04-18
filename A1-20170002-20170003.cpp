#include <iostream>
#include <fstream>
#include <strstream>
#include <cstring>
#include <string>

using namespace std;
struct book
{
    char ISBN[5];
    char title[30];
    char authorName[20];
    char price[10];
    char year[10];
    char  numOfpages[10];
};
istream& operator>>(istream& in, book& obj)
{
    cout << "Enter ISBN: ";
    in>>obj.ISBN ;
    cout << "Enter Title: ";
    in>>obj.title;
    cout << "Enter Author Name: ";
    in>>obj.authorName;
    cout << "Enter Price: ";
    in>> obj.price;
    cout << "Enter Year: ";
    in>>obj.year;
    cout << "Enter Number of pages: ";
    in>>obj.numOfpages;
    return in;

}
ostream& operator << (ostream & out, book obj )
{

    out<<"Book ISBN: "<<obj.ISBN<<endl;
    out<<"Book title: "<<obj.title<<endl;
    out<<"Book author: "<<obj.authorName<<endl;
    out<<"Book price: "<<obj.price<<endl;
    out<<"Book year: "<<obj.year<<endl;
    out<<"Book pages: "<<obj.numOfpages<<endl;

    return out;
}

void addBook(ofstream& file, book& obj)
{

    int length= strlen(obj.ISBN);
    file.write(( char *)&length,sizeof(length));
    file.write(obj.ISBN,length);




    length= strlen(obj.title);
    file.write(( char *)&length,sizeof(length));
    file.write(obj.title,length);



    length= strlen(obj.authorName);
    file.write(( char *)&length,sizeof(length));
    file.write(obj.authorName,length);



    length= strlen(obj.price);
    file.write(( char *)&length,sizeof(length));
    file.write(obj.price,length);




    length= strlen(obj.year);
    file.write(( char *)&length,sizeof(length));
    file.write(obj.year,length);



    length= strlen(obj.numOfpages);
    file.write(( char *)&length,sizeof(length));
    file.write(obj.numOfpages,length);


    file<<'$';

}


void readbook(ifstream & file, book & p )
{

    int length;
    char * s;
    file.read((char *)&length, sizeof(length));
    s=new char [length];
    file.read(s,length);
    istrstream strbuf(s);
    strbuf . getline(p.ISBN,length+1);
    delete [] s;


    char * s1;

    file.read((char *)&length, sizeof(length));
    s1=new char [length];
    file.read(s1,length);
    istrstream strbuf1(s1);
    strbuf1 . getline(p.title,length+1);
    delete [] s1;


    char * s2;
    file.read((char *)&length, sizeof(length));
    s2=new char [length];
    file.read(s2,length);
    istrstream strbuf2(s2);
    strbuf2 . getline(p.authorName,length+1);
    delete [] s2;


    char * s3;
    file.read((char *)&length, sizeof(length));
    s3=new char [length];
    file.read(s3,length);
    istrstream strbuf3(s3);
    strbuf3 . getline(p.price,length+1);
    delete [] s3;

    char *s4;
    file.read((char *)&length, sizeof(length));
    s4=new char [length];
    file.read(s4,length);
    istrstream strbuf4(s4);
    strbuf4 . getline(p.year,length+1);
    delete [] s4;


    char * s5;
    file.read((char *)&length, sizeof(length));
    s5=new char [length];
    file.read(s5,length);
    istrstream strbuf5(s5);
    strbuf5. getline(p.numOfpages,length+1);
    delete [] s5;

    file.seekg(1L,ios::cur);


}
bool check_eq(string a,char b[])
{
    for(int i=0; i<a.length(); i++)
    {
        if (a[i]!=b[i])
            return 0 ;
    }
    return 1;
}
void print_book (string title,ifstream& file,int x)
{

    file.seekg(0,ios::beg);
    book p;
    for (int i=0; i<x; i++)
    {
        readbook(file,p);
        if (check_eq(title,p.title))
            cout<<p;
    }
}
void print_all(ifstream & file, int x)
{
    file.seekg(0,ios::beg);

    book p;
    for  (int i=0; i<x; i++)
    {
        readbook(file,p);
        cout<<p;
        cout<<endl;
    }
}

void remove_book (string ISBN,ifstream & file,ofstream& file2, int x )
{

    book * p;
    p=new book[x];
    for (int i=0; i<x; i++)
    {
        readbook(file,p[i]);

    }
    file.close();
    file2.open("data.txt", ios::out|ios::binary |ios::trunc);
    for (int i=0; i<x; i++)
    {
        if (check_eq(ISBN,p[i].ISBN))
            continue;
        else
            addBook(file2,p[i]);

    }

    file2.close();
}


void update_book (string ISBN,ifstream & file,ofstream& file2, int x, book &obj )

{

    remove_book (ISBN, file, file2, x );
    ofstream f("data.txt",ios::in|ios::binary|ios::app);
    addBook(f,obj);
}


int main()
{
    ofstream file;
    file.open("data.txt",ios::out |ios::binary );
    cout<<"add 3 books to file : "<<endl;
    book obj;
    for(int i=0; i<3; i++)
    {
        cin>>obj;
        addBook(file,obj);

    cout<<endl<<endl;
    }
    file.close();

    cout<<"print 3 books: "<<endl;
    ifstream file2("data.txt",ios::in |ios::binary );
    print_all(file2,3);
        file2.close();

    cout<<"remove book with isbn 7 then print : \n";
    ofstream file3;
    ifstream file4("data.txt",ios::in |ios::binary );
    remove_book("7",file4,file3,3);
    cout<<endl<<endl;

    ifstream file5("data.txt",ios::in |ios::binary );
    print_all(file5,2);
    cout<<endl<<endl;

cout<<"update the book with isbn 13: "<<endl;

    book obj3;
    cout<<endl<<endl;
    cin>>obj3;

     ifstream file6("data.txt",ios::in |ios::binary );
     ofstream file7;
     update_book("13",file6,file7,2,obj3);


    ifstream file8("data.txt",ios::in |ios::binary );
     print_all(file8,2);
     file8.close();

    return 0;
}
