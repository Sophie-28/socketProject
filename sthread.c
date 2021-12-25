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
#define PORT 1090

void PANIC(char* msg);
#define PANIC(msg)  { perror(msg); exit(-1); }
using namespace std;

void check(string val);
void list(void);
void useracc(string val,string val2);
void get_file(string fname);
void put_file(string filename);
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
		check(line);
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
        printf("Connected: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));//
        if ( pthread_create(&child, NULL, Child, &client) != 0 )
            perror("Thread creation");
        else
       
            pthread_detach(child);  /* disassociate from parent */
    }
    return 0;
}
void check(string val)
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
		cout<<hold[index]<<"\t";
		index++;
	//	input.push_back(vec);
	
	}
	
/*	string array_input[input.size()];
	for(int i=0;i<hold.size();i++)
	{
		array_input[i]=input[i];
	}*/
	if((hold[0]=="user")||(hold[0]=="USER"))
	{
		cout<<"user";
			string name=hold[1];
		string pass=hold[2];
		useracc(name,pass);
		
	}
	else if((hold[0]=="list")||(hold[0]=="LIST"))
	{
		cout<<"list";
	
			list();
	}
		else if((hold[0]=="get")||(hold[0]=="GET"))
	{
		cout<<"get";
		string file=hold[1];
		get_file(file);
	}
		else if((hold[0]=="del")||(hold[0]=="DEL"))
	{
		cout<<"del";
	
			
	}
		else if((hold[0]=="quit")||(hold[0]=="QUIT"))
	{
		cout<<"quit";
	
			
	}
	
}
void list(void){
	 DIR *d;
  struct dirent *dir;
  struct stat st;
  d = opendir(".");
  if (d)
  {
  	string d_name;
  	
    while ((dir = readdir(d)) != NULL)
    {
    	int size;
      stat(dir->d_name,&st);
      size=st.st_size;
    cout<< dir->d_name<<" "<<size<<"\n";
    }

    closedir(d);
  }
}
void useracc(string val,string val2)
{
//	cout<<"stage0";	
	ifstream file2("/home/student/21810009/pass.txt");
		if(!file2.is_open())
		{
			cout<<"Failed to open\n";
		}	
	//	cout<<"stage1";	
			string b,a;
			string line;
			int i=0;
			while(file2.good())
			{
				//cout<<"stage2";	
				while(getline(file2, line))
				{
					cout<<"stage3";	
					stringstream ss(line);
					getline(ss,b,':');
					getline(ss,a,':'); 
						cout<<"\n^&&^"<<b;
					cout<<a;
				
					if((b==val) && (a==val2))
					{
					
						cout<<"userfound\n";
						i++;
							cout<<line<<"\n";
					}
				
			}
				
						
		}
		if(i==0)
		{
			cout<<"not ";
		}
 file2.close();
}
void get_file(string filename)
{
	//string filename;
	ifstream infile;
	cout<<" file name"<<filename;
	char msgz[filename.size() + 1];
	  strcpy(msgz, filename.c_str());
	infile.open(msgz);
	string text;
	while(getline(infile,text))
	{
		cout<<text<<"\n";
	}
	infile.close();
}
