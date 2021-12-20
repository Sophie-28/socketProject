/* echo_server_thread.c
 *
 * Copyright (c) 2000 Sean Walton and Macmillan Publishers.  Use may be in
 * whole or in part in accordance to the General Public License (GPL).
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
*/

/*****************************************************************************/
/*** echo_server_thread.c                                                  ***/
/***                                                                       ***/
/*** An echo server using threads.                                         ***/
/***                                                                       ***/
/*** Compile : gcc echo_server_thread.c -o echo_server_thread -lpthread    ***/
/*****************************************************************************/
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
#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>

/* Definations */
#define DEFAULT_BUFLEN 1024
#define PORT 1089
void PANIC(char* msg);
#define PANIC(msg)  { perror(msg); exit(-1); }
using namespace std;

string check(string val);
void useracc(string val,string val2);
/*--------------------------------------------------------------------*/
/*--- Child - echo server                                         ---*/
/*--------------------------------------------------------------------*/

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
        //	printf(line );  
		//cout<<line;   
		check(line);
	//	string h=line;


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
string check(string val)
{
	//	string line;
		string he;
		vector<string> results;
		
	//	cout<<val;
		for(int i=0;i<val.length();i++)
		{
			if(val[i]==' ')
			{
				he+=",";
			}
			else he+=val[i];
		}
		stringstream ss(he);
			while(ss.good())
				{
					string b;
					getline(ss,b,',');
					results.push_back(b);
		
				}
				//line=b;
				string arresults[results.size()];
			for(int i=0;i<results.size();i++)
			{
	//cout<<results.at(i)<<"\n";
	arresults[i]=results[i];
	cout<<arresults[i]<<"\n";
	}
			
		if(arresults[0]=="USER")
	{
		cout<<"user";
		string name=arresults[1];
		string pass=arresults[2];		
		useracc(name,pass);
		//cout<<name<<pass<<"**";
	
	}
	if(arresults[0]=="GET")
	{
		cout<<"get";
	}
	if(arresults[0]=="LIST")
	{
		cout<<"list";
	}
}

void useracc(string val,string val2)
{
//	cout<<"stage0";	
	ifstream file2("//home//student//21810009//pass.txt");
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
				cout<<"stage2";	
				while(getline(file2, line))
				{
					cout<<"stage3";	
					stringstream ss(line);
					getline(ss,b,':');
					getline(ss,a,':'); 
				
					if((b==val) && (a==val2))
					{
					
						cout<<"user found";
						i++;
					}
			}
					cout<<"^&&^"<<b;
					cout<<a;	
		}
		if(i==0)
		{
			cout<<"not ";
		}
 file2.close();
}


/*--------------------------------------------------------------------*/
/*--- main - setup server and await connections (no need to clean  ---*/
/*--- up after terminated children.                                ---*/
/*--------------------------------------------------------------------*/
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
