#include <bits/stdc++.h>
#include <string.h>
#include <sstream>
#include <stdlib.h>     /* atoi */
#include <iostream>
#include <fstream>
using namespace std;

struct course
{
    char id[6];
    string name;
    string inst_name;
    short weeks;

};
istream& operator >> (istream & in,course &obj )
{
    cout<<"Enter course id: ";
    in>>obj.id;
    cout<<"Enter course name: ";
    in>>obj.name;
    cout<<"Enter instructor name: ";
    in>>obj.inst_name;
    cout<<"Enter weeks: ";
    in>>obj.weeks;

    return in;
}
ostream& operator <<(ostream& out,course obj)
{
    out<<"course id: "<<obj.id<<endl;
    out<<"Enter course name: "<<obj.name<<endl;
    out<<"Enter instructor name: "<<obj.inst_name<<endl;
    out<<"Enter weeks: "<<obj.weeks<<endl;
    return out;
}
struct PK
{
    char id[6];
    string inst_name;
    int offset;
};
struct SK
{
    string inst_name;
    int index;
};
struct SK_inverted_list
{
    char id [6];
    int rrn;
};
class courses
{
private:
    course c;
    int counter,counter2,counter3;
    PK* p;
    SK* s;
    SK_inverted_list *s1;
public:
    ///unrelated functions
    courses();
    void readcourse(int offset);
    void addcourse();
    void get_course(char id[]);
    void removecourse(char key[]);
    void updatecourse(char key[]);
    /// Primary key functions
    void sortindex();
    int PK_binarysearch(char key[]);
    void save_index();
    void load_index();
    void change_PK_state(bool s);
    bool get_PK_state();
    void reconstruct_PK();
    void get_PK();
    ///secondary key functions
    void add_SK(char id[],string name);
    void print_SK();
    void save_SK();
    void load_SK();
    int SK_binarysearch(string key);
    void SK_sort();
    void SK_get_course();
    void delete_course_SK(string name);
    void delete_SK(int i);
    void delete_SK_invertedlist(int i);
    void update_SK();


};
courses::courses()
{
    counter=0;
    counter2=0;
    counter3=0;
    p=new PK[100];
    s=new SK[100];
    s1=new SK_inverted_list[200];
}
void courses::sortindex()
{
    int len = counter - 1;
    for (int i = 0; i<len; i++)
        for (int j = 0; j<len - i; j++)
        {
            if (atoi(p[j].id)>atoi(p[j + 1].id))
            {
                swap(p[j],p[j+1]);
            }
        }
}
void courses::addcourse()
{
re:
    cout<<"Enter new book data \n";
    cin>>c;
    ofstream file ("course_data.txt",ios::binary|ios::app);
    int check= PK_binarysearch(c.id);
    if (check!=-1)
    {
        cout<<"the id exist !!"<<endl
            <<"Enter another one"<<endl;
        goto re;

    }
    file.seekp(0,ios::end);
    int offset=file.tellp();
    if (offset==0)
    {
        file<<0;
    }
    offset=file.tellp();
    char buffer [200];
    strcpy(buffer,c.id);
    strcat(buffer,"|");
    strcat(buffer,c.name.c_str());
    strcat(buffer,"|");
    strcat(buffer,c.inst_name.c_str());
    strcat(buffer,"|");
    char* weeks=new char[1];
    weeks[0]=(char)c.weeks;
    strcat(buffer,weeks);
    strcat(buffer,"|");
    delete[]weeks;
    int len=strlen(buffer);
    file.write((char*)&len,sizeof(int));
    file.write((char*)&buffer,len);
    PK a;
    strcpy(a.id,c.id);
    a.offset=offset;
    a.inst_name=c.inst_name;
    p[counter++]=a;
    sortindex();
    add_SK(c.id,c.inst_name);
    SK_sort();
    file.close();

}
void courses::readcourse(int offset)
{
    course obj;
    int length;
    ifstream file ("course_data.txt",ios::binary);
    file.seekg(offset,ios::beg);
    file.read((char*)&length,sizeof(length));
    char *buffer;
    buffer=new char[length];
    file.read(buffer,length);
    if (buffer[0]=='*')
    {
        cout<<"deleted record !! \n";
        return;
    }
    char b[30];
    char d[30];
    char f[10];
    istringstream stream(buffer);
    stream.getline(obj.id,6,'|');
    stream.getline(b,30,'|');
    stream.getline(d,30,'|');
    stream.getline(f,10,'|');
    obj.name="";
    obj.inst_name="";
    for(int i=0; i<strlen(b); i++)
        obj.name+=b[i];
    for(int i=0; i<strlen(d); i++)
        obj.inst_name+=d[i];
    obj.weeks=(short)f[0];
    cout<<obj<<endl;
    delete []buffer;
}
int courses::PK_binarysearch(char key[])
{
    int size= counter;
    int low =0, high=size-1;
    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (strcmp(p[middle].id, key) == 0)
            return middle;
        else if (atoi(p[middle].id)<atoi(key))
            low = middle + 1;
        else
            high = middle - 1;
    }
    return -1;

}

void courses::save_index()
{
    ofstream file("PK_index.txt",ios::binary|ios::trunc);
    for (int i=0; i<counter; i++)
    {
        PK a;
        a=p[i];
        file.write((char*)&a,sizeof(a));
    }
    change_PK_state(true);
    file.close();
}
void courses::load_index()
{
    ifstream file("PK_index.txt",ios::binary);
    if (!file)
    {
        reconstruct_PK();
    }
    else if (get_PK_state())
    {
        file.seekg(0,ios::beg);
        counter=0;
        while (!file.eof())
        {
            PK a;
            file.read((char*)&a,sizeof(a));
            p[counter++]=a;
        }
    }
    else
        reconstruct_PK();
}
void courses::change_PK_state(bool s)
{
    fstream file("course_data.txt",ios::in|ios::out);
    file.seekp(0,ios::beg);
    file<<s;
    file.close();

}
bool courses::get_PK_state()
{
    bool a;
    ifstream file("course_ data.txt");
    file.seekg(0,ios::beg);
    file>>a;
    file.close();
    return a;
}
void courses::reconstruct_PK()
{
    ifstream file("course_data.txt",ios::in|ios::binary);
    if (!file)
        return;
    counter= 0;
    int offset;
    file.seekg(1,ios::beg);
    while(!file.eof())
    {
        offset=file.tellg();
        int len;
        PK a;
        file.read((char*)&len,sizeof(len));
        char buffer[200];
        file.read((char*)&buffer,len);
        if (buffer[0]=='*')
            continue;
        istringstream stream(buffer);
        stream.getline(a.id,6,'|');
        char s[30];
        char inst[30];
        stream.getline(s,30,'|');
        stream.getline(inst,30,'|');
        a.inst_name="";
        for (int j=0;j<strlen(inst);j++)
            a.inst_name+=inst[j];
        a.offset=offset;
        p[counter++]=a;
    }
    file.close();
    sortindex();
}
void courses:: get_course(char id [])
{
    ifstream file("course_data.txt",ios::binary);
    int index=PK_binarysearch(id);
    if (index==-1)
    {
        cout<<"Not found \n";
        return;
    }
    else
    {
        readcourse(p[index].offset);

    }
}
void courses::get_PK()
{
    if (counter==0)
    {
        cout<<"NO PK index founded"<<endl;
        return;
    }
    cout<<"ID"<<"\t"<<"offset"<<endl;
    for (int i=0; i<counter; i++)
    {
        cout<<p[i].id<<"\t"<<p[i].offset<<endl;
    }
    cout<<endl<<endl;
}
void courses::removecourse(char key[])
{
    fstream file("course_data.txt",ios::in|ios::out|ios::binary);
    int index=PK_binarysearch(key);
    if (index==-1)
    {
        cout<<"not found \n";
        return;
    }
    else
    {
        file.seekp(p[index].offset+4,ios::beg);
        file<<'*';
    }
    cout<<"deleted \n";
    for(int i=index; i<counter; i++)
        p[i]=p[i+1];
    file.close();
}

void courses::updatecourse(char key[])
{
    removecourse(key);
    addcourse();
}
void courses::SK_sort()
{
    int len = counter2 - 1;
    for (int i = 0; i<len; i++)
        for (int j = 0; j<len - i; j++)
        {
            if(s[j].inst_name>s[j+1].inst_name)
                swap(s[j],s[j+1]);
        }
}
int courses::SK_binarysearch(string key)
{
    int size= counter2;
    int low =0, high=size-1;
    while (low <= high)
    {
        int middle = (low + high) / 2;
        if ( key == s[middle].inst_name)
            return middle;
        else if (s[middle].inst_name<key)
            low = middle + 1;
        else
            high = middle - 1;
    }
    return -1;
}
void courses::add_SK(char id[],string name)
{
    SK temp;
    SK_inverted_list tmp2;
    if (counter2==0)
    {
        temp.inst_name=name;
        temp.index=counter3;
        s[counter2++]=temp;
        strcpy(tmp2.id,id);
        tmp2.rrn=-1;
        s1[counter3++]=tmp2;
    }
    else
    {
        int i= SK_binarysearch(name);
        if (i==-1)
        {
            temp.inst_name=name;
            temp.index=counter3;
            s[counter2++]=temp;
            strcpy(tmp2.id,id);
            tmp2.rrn=-1;
            s1[counter3++]=tmp2;
            return;
        }
        else
        {
            strcpy(tmp2.id,id);
            tmp2.rrn=s[i].index;
            s[i].index=counter3;
            s1[counter3++]=tmp2;
        }
    }
}
void courses::print_SK()
{
    if (counter2==0)
    {
        cout<<"No SK founded!!"<<endl;
        return;
    }
    cout<<"inst_name\tindex\n";
    for (int i=0; i < counter2; i++)
    {
        cout<<s[i].inst_name<<"\t"<<s[i].index<<endl;
    }
    cout<<"index\tid\trrn\n";
    for (int i=0; i < counter3; i++)
    {
        cout<<i<<"\t"<<s1[i].id<<"\t"<<s1[i].rrn<<endl;
    }
    cout<<endl<<endl;
}
void courses::save_SK()
{
    ofstream file1("SK_index.txt",ios::binary|ios::trunc);
    ofstream file2("SK_inverted_list.txt",ios::binary|ios::trunc);
    for (int i=0; i<counter2; i++)
    {
        SK a;
        a=s[i];
        file1.write((char*)&a,sizeof(a));
    }
    file1.close();
    for (int i=0; i<counter3; i++)
    {
        SK_inverted_list tmp;
        tmp=s1[i];
        file2.write((char*)&tmp,sizeof(tmp));
    }
    file2.close();
}
void courses::load_SK()
{
    ifstream file1("SK_index.txt",ios::binary);
    ifstream file2("SK_inverted_list.txt",ios::binary);
    if (!file1|!file2)
    {
        if (counter==0)
            return;
        for (int i=0;i<counter;i++)
            add_SK(p[i].id,p[i].inst_name);
        return;
    }
    counter2=0;
    counter3=0;
    while(!file1.eof())
    {
        SK a;
        file1.read((char*)&a,sizeof(a));
        s[counter2++]=a;

    }
    file1.close();
    while (!file2.eof())
    {
        SK_inverted_list tmp;
        file2.read((char*)&tmp,sizeof(tmp));
        s1[counter3++]=tmp;
    }
    file2.close();
}
void courses::SK_get_course()
{
    string name;
    cout<<"Enter instructor name: ";
    cin>>name;
    cout<<endl<<endl;
    int i=SK_binarysearch(name);
    if (i==-1)
    {
        cout<<"not found \n";
        return;
    }
    else
    {
        get_course(s1[s[i].index].id);
        if (s1[s[i].index].rrn==-1)
            return;
        else
        {
            int j=s1[s[i].index].rrn;
            while (true)
            {
                get_course(s1[j].id);
                j=s1[j].rrn;
                if (j==-1)
                    break;
            }
        }
    }
}
void courses::delete_course_SK(string name)
{
    int i=SK_binarysearch(name);
    if (i==-1)
    {
        cout<<"not found \n";
        return;
    }
    else
    {
        removecourse(s1[s[i].index].id);
        delete_SK_invertedlist(s[i].index);
        if (s1[s[i].index].rrn==-1)
        {
            delete_SK(i);
            return;
        }
        else
        {
            int j=s1[s[i].index].rrn;
            while (true)
            {
                removecourse(s1[j].id);
                delete_SK_invertedlist(j);
                j=s1[j].rrn;
                if (j==-1)
                    break;
            }
            delete_SK(i);
        }
    }
}
void courses::delete_SK(int i)
{
    for (int j=i; j<counter2; j++)
        s[j]=s[j+1];
    counter2--;
}
void courses::delete_SK_invertedlist(int i)
{
    for (int j=i; j<counter3; j++)
        s1[j]=s1[j+1];
    counter3--;
}
void courses::update_SK()
{
    string name;
    cout<<"enter instructor name: ";
    cin>>name;
    cout<<"we deleted all "<<name <<" courses"<<endl;
    int n;
    cout<<"Enter the no of new courses \n";
    cin>>n;
    while (n--)
        addcourse();
}
int main()
{
    courses a;
    a.load_index();
    a.load_SK();
    a.get_PK();
    a.print_SK();
    int x;
    char id [6];
    string name;
    while (true)
    {
        cout<<"1-add course "<<endl
            <<"2-print course (id) "<<endl
            <<"3-print course (instructor name) "<<endl
            <<"4-delete course (id) "<<endl
            <<"5-delete course (instructor name) "<<endl
            <<"6-update course (id) "<<endl
            <<"7-update course (instructor name) "<<endl
            <<"Else-Exit "<<endl
            <<"your choice: ";
        cin>>x;
        if (x==1)
        {
            a.addcourse();
        }
        else if (x==2)
        {
            cout<<"Enter id: ";
            cin>>id;
            a.get_course(id);
        }
        else if (x==3)
        {
            a.SK_get_course();
        }
        else if (x==4)
        {
            cout<<"Enter id: ";
            cin>>id;
            a.removecourse(id);
        }
        else if (x==5)
        {
            cout<<"Enter name of instructor: ";
            cin>>name;
            a.delete_course_SK(name);

        }
        else if (x==6)
        {
            cout<<"Enter id: ";
            cin>>id;
            a.updatecourse(id);
        }
        else if (x==7)
        {
            a.update_SK();
        }
        else
        {
            a.save_index();
            a.save_SK();
            break;
        }
    }
    return 0;
}
