#include <stdlib.h>
#include <sstream> 
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <dirent.h> 
#include <stdio.h> 
#include <sys/stat.h>
#include <string>
#include<cmath>
#define DEFAULT_BUFLEN 1024
//#define PORT 1099

void PANIC(char* msg);
#define PANIC(msg)  { perror(msg); exit(-1); }
using namespace std;

string dir_path;
int PORT;
void check(string val,int numb);
void list(int numb);
void useracc(string val,string val2,int numb);
void get_file(string fname,int numb);
void put_file(string filename,int numb);
void del_file(string filename,int numb);
string tocharfloat(int num);
string removespace(string str);
void quit(int numb);
void input_line();
int toint(string numstr);
//void useracc(string val,string val2);

void* Child(void* arg)
{   char line[DEFAULT_BUFLEN];
    int bytes_read;
    int client = *(int *)arg;
    char welmessag[DEFAULT_BUFLEN]="Welcome to Bob's Server \n";

    do
    {
    	bytes_read=send(client,welmessag , sizeof(welmessag), 0); // displayes the welcome message to the client
    	
        bytes_read = recv(client, line, sizeof(line), 0);
        if (bytes_read > 0) {
		check(line,client);
		memset(line,0,sizeof(line));
     if ( (bytes_read=send(client, line, bytes_read, 0)) < 0 ) {
                        printf("Send failed\n");
                    
                        break;
                }
        } else if (bytes_read == 0 ) {
                printf("Connection closed by client\n");
                break;
        } else {
                printf("Connection has problem\n");
                break;
        }
    } while (bytes_read > 0);
    close(client);
    return arg;
}


int main(int argc, char *argv[])
{      

input_line();
 int sd,opt,optval;
    struct sockaddr_in addr;
    unsigned short port=0;
    

    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
                port=atoi(optarg);
                break;
        }
    }

 if ( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
        PANIC("Socket");
    addr.sin_family = AF_INET;

    if ( port > 0 )
                addr.sin_port = htons(port);
    else
                addr.sin_port = htons(PORT);

    addr.sin_addr.s_addr = INADDR_ANY;

   // set SO_REUSEADDR on a socket to true (1):
   optval = 1;
   setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);


    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
        PANIC("Bind");
    if ( listen(sd, SOMAXCONN) != 0 )
        PANIC("Listen");

    printf("System ready on port %d\n",ntohs(addr.sin_port));

    while (1)
    {
        int client;
		socklen_t addr_size = sizeof(addr);
        pthread_t child;

        client = accept(sd, (struct sockaddr*)&addr, &addr_size);
        
        printf("Connected: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        if ( pthread_create(&child, NULL, Child, &client) != 0 )
            perror("Thread creation");
        else
       
            pthread_detach(child);  /* disassociate from parent */
    }
    return 0;
}
void input_line()
{
		 string input;
    getline(cin,input);
    string new_input="";
    string trStr;
    int i=0;
    for(i; i<input.length();i++)
    {
       if(input[i]==' ' && input[i+1]=='-' && (input[i+2]=='d' ||input[i+2]=='p' ||input[i+2]=='u'))
        {
            new_input+=",";
            i=i+3;
        }
        else
        {
            new_input+=input[i];
        }
    }
    vector<string> hold;
    stringstream ss(new_input);

    while (ss.good())
    {
        string substring;
        getline(ss, substring, ',');
        hold.push_back(substring);
    }
    int index = hold.size();
    string arr[index];
    for(int i = 0; i<hold.size(); i++)
    {
        arr[i]=hold[i];
    }
    for(int i = 0; i<(sizeof(arr)/sizeof(arr[0])); i++)
    {

        cout<<arr[i]<<" "<<endl;
    }
    string dir;
    string text;
   int prt;
    dir=removespace(arr[1]);
   // prt=removespace(arr[2]);
    text=removespace(arr[3]);
    
     dir_path=dir;
    cout<<"path :"<<dir_path<<"\n";
   prt= toint(arr[2]);
   PORT=prt;
    //stringstream prt(arr[1]);
    //prt>>PORT;
    //cout<<dir<<"\t"<<prt<<"\t"<<text;
    //	string c="cd " +path;
	
	 char mess[dir.size() + 1];
        strcpy(mess, dir.c_str());
        
         DIR* d;
        // d = opendir(mess);
        if (d)
        {
        
        	
            d = opendir(mess);
            //chdir(mess);
            cout<<"path directry found"<<endl;
           // closedir(d);
           // pas=false;
        }
        else
        {
            cout<<"path directry does not exit"<<endl;
            
        }
}
int toint(string numstr)
	{
		int num=0,size=numstr.size();
		int j=size;
		for(int i=0;i<size;i++)
		{
			num+=(numstr[--j]-'0')*pow(10,i);
		}
		return num;
	}

void check(string val,int numb)
{	

string new_val;

	for(int i=0;i<val.length();i++)
	{
		if(val[i]==' ')
		
			new_val+=",";
		
		else 
		   new_val+=val[i];
		   	
	}
	int index=0;
	string hold[3];
	stringstream ss(new_val);
	vector<string> commands;
	while(ss.good())
	{
		string vec="";
		getline(ss,vec,',');
		if(vec=="")
		{
			commands.push_back(vec);
			hold[index]=new_val;
		}
		commands.push_back(vec);
	//	hold[index]=vec;
	//	cout<<hold[index]<<"\t";
	//	index++;
	//	input.push_back(vec);
	}
	cout<<commands.size()<<endl;
	string c = commands[0];
	string cc=removespace(c);
	for(int i =0; i<cc.length();i++)
	{
		cout<<"-"<<cc[i]<<endl;
	}


	for(int i=0;i<3;i++)
	{
		cout<<hold[i]<<endl;
	}
	string file;
	if((cc=="user")||(cc=="USER"))
	{
		cout<<"user";
			string name=removespace(commands[1]);
		string pass=removespace(commands[2]);
		useracc(name,pass,numb);
		
	}
	if((cc=="list")||(cc=="LIST"))
	{
		cout<<"list";
		 
	     list(numb);
	}
 	if((cc=="get")||(cc=="GET"))
	{
		cout<<"get";
			file=commands[1];
		 string file2=removespace(file);
		get_file(file2,numb);
	}
		if((cc=="put")||(cc=="PUT"))
	{
		cout<<"get";
			file=commands[1];
		 string file2=removespace(file);
		put_file(file2,numb);
	}
		if((cc=="del")||(cc=="DEL"))
	{
		cout<<"del";
		 	file=commands[1];
		 string file2=removespace(file);
		del_file(file2,numb);
				
	}
		if((cc=="quit")||(cc=="QUIT"))
	{
	quit(numb);
				
	}
	
	
}
string tocharfloat(int num)
{
stringstream ss;
ss << num;
string str;
ss>>str;
return str;
	}
string removespace(string str){
    
   string result="";
    int i=0;
    while (str[i]!='\0')
    {
        if(!isspace(str[i])){
            result+=str[i];

        }
        i++;
    }
   return result;
}
void list(int numb){
	char text1[dir_path.size()+1];
  strcpy(text1, dir_path.c_str());
	 DIR *d;
  struct dirent *dir;
  struct stat st;
  d = opendir(text1);
  string strsize;
  string y;
  int byte;
  string space=" ";
  string nextline="\n";
  if (d)
  {
  	string d_name;
  	
    while ((dir = readdir(d)) != NULL)
    {
    	int size;
      stat(dir->d_name,&st);
      size=st.st_size;
      strsize=tocharfloat(size);
 y=y+dir->d_name+space +strsize+nextline; 
   }
 // cout<<y;
 char text[y.size()+1];
  strcpy(text, y.c_str());
  // cout<<text;
  byte=send(numb,text , sizeof(text), 0); 
   

    closedir(d);
  } 
}
void useracc(string val,string val2,int numb)
{
int byte;
int i=0;
		
	ifstream infile;
	FILE *filePointer;
	char line[DEFAULT_BUFLEN];
	if ((filePointer = fopen("pass.txt", "r")) == NULL)
	        {
	        	cout<<" could not read file !"<<endl;
	        	
			}
        
	    while (fgets(line, DEFAULT_BUFLEN, filePointer) != NULL) {
         //cout<<line<<endl;
        
        vector<string> getUser;
        stringstream ss(line);

        while (ss.good())
        {
            
            string substring="";
            getline(ss, substring, ':');
            getUser.push_back(substring);
        }
        
        string name=removespace(getUser[0]);
        string password=removespace(getUser[1]);

    
		if(val==name && val2==password)
		{
			cout<<"user exist"<<endl;
		//	cout<<name;
		//	cout<<password;
				char mess[]="User found\n";
				 byte=send(numb,mess , sizeof(mess), 0);
				 i++;
		}
		infile.close();
	}
	if(i==0)
		{
		//	cout<<"not ";
				char mess[]="User not found\n";
						 byte=send(numb,mess , sizeof(mess), 0);
		}
//	return;
	//if(i==0)
		//{
		//	cout<<"not ";
	//			char mess[]="User not found";
		//				 byte=send(numb,mess , sizeof(mess), 0);
	//	}
}
void get_file(string filename,int numb)
{
	//string filename;
	int byte;
	ifstream infile;
	string filename2=dir_path+"/"+filename;
	char msgz[filename2.size() + 1];
	  strcpy(msgz, filename2.c_str());
	infile.open(msgz);
	if(infile.is_open()){
			string text;
	string text2;
	while(getline(infile,text))
	{
		text2=text2+text+"\n";
		
	}
	
	char message[text2.size() + 1];
	  strcpy(message, text2.c_str());
	 // cout<<message<<"\n";
	
	byte=send(numb,message , sizeof(message), 0); 
	infile.close();
	}
	else{
		char info[]="404 File foobar not found.";
		byte=send(numb,info , sizeof(info), 0); 
	}

}
void del_file(string filename,int numb)
{
	int byte;
	string len;
	
	string filename2=dir_path+"/"+filename;
	char msgz[filename2.size() + 1];
	  strcpy(msgz, filename2.c_str());
	if(remove(msgz)==0)
	{
			len="200 File "+filename+" deleted.\n";
			
	 	//byte=send(numb,len , len.size(), 0);
	}
	else{
		len="404 File "+filename+" is not on the server.\n";
		//	byte=send(numb,info , sizeof(info), 0);
	}
		char mess[len.size() + 1];
	 strcpy(mess, len.c_str());
	 byte=send(numb,mess , sizeof(mess), 0);
	
}
void quit(int numb)
{
	int byte;
	string message="Goodbye!";
		char mess[message.size() + 1];
	 strcpy(mess, message.c_str());
	 byte=send(numb,mess , sizeof(mess), 0);
	 close(numb);
	
}

void put_file(string filename,int numb)	
{
		char charac[filename.size() + 1];
	  strcpy(charac, filename.c_str());
	  ofstream file;
	  file.open(charac);
	  char buff[1024]={0};
	  int size=0;
	  if(file.is_open()){
	  	memset(buff,0,sizeof(buff));
	  	while((size=recv(numb,buff,sizeof(buff),0))>0){
	  		file.write(buff,size);
	  		if(size<sizeof(buff))
	  		{
	  			break;
			  }
		  }
		  cout<<"saved";
		  file.close();
	  }
	  else{
	  	cout<<"failed";
	  }
/*	struct stat st;
	FILE *fp=fopen(charac,"a");
	if (fp==NULL)
	{
		cout<<"Open error";
	}
	int size;
	unsigned char data[256]={0};
	while(1)
	{
		
		
      stat(filename,&st);
      size=st.st_size;
      fread(data,1,256,fp); 
	}
	if(size>0)
	{
		write(numb,data,size);
	}*/
/*	int ch=0;
	int words;
	char buffer[255];
	read(numb,&words,sizeof(int));
	while(ch!=words)
	{
	write(numb,buffer,sizeof(buffer));
		cout<<fp <<buffer;
		ch++;
	}
	cout<<"successfull";
	close(&fp);*/
	}	
