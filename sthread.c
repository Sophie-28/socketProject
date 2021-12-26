#include <stdlib.h>
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
//#include <vector>
#include <sstream>
#include <fstream>
#include <dirent.h> 
#include <stdio.h> 
#include <sys/stat.h>
#define DEFAULT_BUFLEN 1024
#define PORT 1099

void PANIC(char* msg);
#define PANIC(msg)  { perror(msg); exit(-1); }
using namespace std;

void check(string val,int numb);
void list(int numb);
void useracc(string val,string val2,int numb);
void get_file(string fname,int numb);
void put_file(string filename,int numb);
void del_file(string filename,int numb);
string tocharfloat(int num);
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
{   int sd,opt,optval;
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

void check(string val,int numb)
{	
//vector <string> input;
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
	while(ss.good())
	{
		string vec;
		getline(ss,vec,',');
		hold[index]=vec;
	//	cout<<hold[index]<<"\t";
		index++;
	//	input.push_back(vec);
	
	}
	
/*	string array_input[input.size()];
	for(int i=0;i<hold.size();i++)
	{
		array_input[i]=input[i];
	}*/
	string file;
	if((hold[0]=="user")||(hold[0]=="USER"))
	{
		cout<<"user";
			string name=hold[1];
		string pass=hold[2];
		useracc(name,pass,numb);
		
	}
	else if((hold[0]=="list")||(hold[0]=="LIST"))
	{
		cout<<"list";
		 
	     list(numb);
	}
		else if((hold[0]=="get")||(hold[0]=="GET"))
	{
		cout<<"get";
		 file=hold[1];
		get_file(file,numb);
	}
		else if((hold[0]=="put")||(hold[0]=="PUT"))
	{
		cout<<"get";
		 file=hold[1];
		put_file(file,numb);
	}
		else if((hold[0]=="del")||(hold[0]=="DEL"))
	{
		cout<<"del";
		 file=hold[1];
		del_file(file,numb);
				
	}
		else if((hold[0]=="quit")||(hold[0]=="QUIT"))
	{
		cout<<"quit";
		cout<<"goodbye";
	exit(1);
				
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
void list(int numb){
	 DIR *d;
  struct dirent *dir;
  struct stat st;
  d = opendir(".");
  
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
//	cout<<"stage0";	
int byte;
int i=0;
		string line;
	ifstream infile;
	string filename ="//home//student//21810009//pass.txt";
	//cout<<" file name"<<filename;
	char msgz[filename.size() + 1];
	  strcpy(msgz, filename.c_str());

	infile.open(msgz);
	if(infile.is_open()){
			string text;
	string text2;
	while(getline(infile,text))
	{
			text2=text2+text+"\n";
				string b;
				string a;
			stringstream ss(text);
					getline(ss,b,':');
					getline(ss,a,':'); 
					cout<<a;
						if((b==val) && (a==val2))
					{		char mess[]="User found";
						 byte=send(numb,mess , sizeof(mess), 0);
				       	i++;
					}
		
	}
	
	
	infile.close();
	}
	if(i==0)
		{
		//	cout<<"not ";
				char mess[]="User not found";
						 byte=send(numb,mess , sizeof(mess), 0);
		}
}
void get_file(string filename,int numb)
{
	//string filename;
	int byte;
	ifstream infile;
	cout<<" file name"<<filename;
	char msgz[filename.size() + 1];
	  strcpy(msgz, filename.c_str());
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
	
	char msgz[filename.size() + 1];
	 strcpy(msgz, filename.c_str());
	if(remove(msgz)==0)
	{
			len="200 File "+filename+" deleted.";
			
	 	//byte=send(numb,len , len.size(), 0);
	}
	else{
		len="404 File "+filename+" is not on the server.";
		//	byte=send(numb,info , sizeof(info), 0);
	}
		char mess[len.size() + 1];
	 strcpy(mess, len.c_str());
	 byte=send(numb,mess , sizeof(mess), 0);
	
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
