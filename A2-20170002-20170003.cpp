#include <iostream>
#include <fstream>
#include <strstream>
#include <cstring>
#include <string>
using namespace std;
const char DELETE_FLAG = '*';
const char ACTIVE_FLAG = ' ';
const char delimeter = '|';

struct book
{
    char check=ACTIVE_FLAG;
    int RRN=-1;
    char ISBN[5];
    char title[30];
    char authorName[20];
    char price[10];
    char year[10];
    char  numOfpages[10];

};


bool check_eq(string a,char b[])
{
    for(int i=0; i<a.length(); i++)
    {
        if (a[i]!=b[i])
            return 0 ;
    }
    return 1;
}


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



void writebook(ofstream &outfile, book &obj)
{

    outfile.put(obj.check);

    outfile.write((char*)& obj.RRN, sizeof(int));

    outfile.write(obj.ISBN,5 );
    outfile.put(delimeter);

    outfile.write(obj.title, 30);
    outfile.put(delimeter);

    outfile.write(obj.authorName, 20);
    outfile.put(delimeter);

    outfile.write(obj.price, 10);
    outfile.put(delimeter);

    outfile.write(obj.year, 10);
    outfile.put(delimeter);

    outfile.write(obj.numOfpages, 10);
    outfile.put(delimeter);

}

void readbook(ifstream &infile, book & obj)
{

    int g=infile.tellg();
    if (g==0)
        infile.seekg(sizeof(int),ios::cur);
    infile.get(obj.check);
    if (obj.check==DELETE_FLAG)
    {
        cout<<"deleted record"<<endl;
    }

    infile.read((char*)&obj.RRN, sizeof(int));

    infile.read(obj.ISBN, 5);
    infile.seekg(1,ios::cur);


    infile.read(obj.title, 30);
    infile.seekg(1,ios::cur);


    infile.read(obj.authorName, 20);
    infile.seekg(1,ios::cur);


    infile.read(obj.price, 10);
    infile.seekg(1,ios::cur);


    infile.read(obj.year, 10);
    infile.seekg(1,ios::cur);


    infile.read(obj.numOfpages, 10);
    infile.seekg(1,ios::cur);
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
        cout<<p<<endl;

    }
}

void remove_book (string ISBN,ifstream & file,ofstream& file2, int x )
{

    file.seekg(0,ios::beg);
    book * p;
    p=new book[x];
    int header, nxtdelet;
    file.read((char*)&header,sizeof(int));
    for (int i=0; i<x; i++)
    {
        readbook(file,p[i]);
        if (check_eq(ISBN,p[i].ISBN))
        {
            nxtdelet=i;
            p[i].check=DELETE_FLAG;
            p[i].RRN=header;
        }
    }
    file.close();

    file2.open("a2.txt", ios::out|ios::binary |ios::trunc);
    file2.write((char*)&nxtdelet,sizeof(int));
    for (int i=0; i<x; i++)
    {

        writebook(file2,p[i]);
    }
    file2.close();




}


void update_book (string ISBN,book & obj,ifstream & file,ofstream& file2, int x )
{

    file.seekg(0,ios::beg);
    book * p;
    p=new book[x];
    int header, nxtdelet;
    file.read((char*)&header,sizeof(int));
    for (int i=0; i<x; i++)
    {
        readbook(file,p[i]);
        if (check_eq(ISBN,p[i].ISBN))
        {
            nxtdelet=i;

        }
    }
    file.close();

    file2.open("a2.txt", ios::out|ios::binary |ios::trunc);
    file2.write((char*)&header,sizeof(int));
    for (int i=0; i<x; i++)
    {
        if (i==nxtdelet)
        {
            writebook(file2,obj);
            continue;
        }

        writebook(file2,p[i]);
    }
    file2.close();




}


void compact_file(ifstream & file,ofstream& file2, int x )
{

    file.seekg(0,ios::beg);
    book * p;
    p=new book[x];
    int header;
    file.read((char*)&header,sizeof(int));
    for (int i=0; i<x; i++)
    {
        readbook(file,p[i]);

    }
    file.close();
    file2.open("a2.txt", ios::out|ios::binary |ios::trunc);
    header=-1;
    file2.write((char*)&header,sizeof(int));
    for (int i=0; i<x; i++)
    {
        if (p[i].check==ACTIVE_FLAG)

            writebook(file2,p[i]);
    }
    file2.close();
    cout<<"File compacted successfully \n";

}


int main()
{
   /* ofstream  f("a2.txt",ios::out|ios::binary);
    ifstream  f1;
    string a;
    book obj;
    f.seekp(0,ios::end);
    int pos = f.tellp();
    if(pos <= 0)
    {
        int header = -1;
        f.write((char*) & header, sizeof(int));
    }
    cout<<"enter 3 books: "<<endl;
    for(int i=0; i<3; i++)
    {
        cin>>obj;
        writebook(f,obj);
        cout<<endl<<endl;
    }
    f.close();

    cout<<"print 3 books: "<<endl;
    ifstream file2("a2.txt",ios::in |ios::binary );
    print_all(file2,3);
    file2.close();*/
   /* string a;
    cout<<"remove book with isbn then print : \n";
    ofstream file3;
    ifstream file4("a2.txt",ios::in |ios::binary );
    cout<<"enter isbn: ";
    cin>>a;
    remove_book(a,file4,file3,3);
    cout<<endl<<endl;*/
  /*  ofstream  f("a2.txt",ios::out|ios::binary|ios::app);
    book obj;
    cin>>obj;
        writebook(f,obj);



    ifstream file5("a2.txt",ios::in |ios::binary );
    print_all(file5,3);
    cout<<endl<<endl;
*/
 /*   string a;
    cout<<"update the book with isbn: "<<endl;
    cout<<"enter isbn: ";
    cin>>a;
    book obj3;
    cout<<"enter the data of new book: ";
    cin>>obj3;

    ifstream file6("a2.txt",ios::in |ios::binary );
    ofstream file7;
    update_book(a,obj3,file6,file7,4);


    ifstream file8("a2.txt",ios::in |ios::binary );
    print_all(file8,3);
    file8.close();
*/
    cout<<endl<<endl;
    ifstream file9("a2.txt",ios::in |ios::binary );
    ofstream file10;
    compact_file(file9,file10,4);
    file9.close();
    file10.close();

/*

    string a="7";
    ifstream file11("a2.txt",ios::in |ios::binary );
    print_book(a,file11,4);
    file11.close();
*/



    return 0;
}
