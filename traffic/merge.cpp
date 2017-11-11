#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>
#include <math.h>
#include <sstream>
using namespace std;

vector<string> split(string str, char delimiter) 
{
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;
  
	while(getline(ss, tok, delimiter)) 
	{
    	internal.push_back(tok);
  	}
  	return internal;
}
uint ipTint(char *ipstr)  
{  
    if (ipstr == NULL)  
    	return 0;  
  
    char *token;  
    uint i = 3, total = 0, cur;  
  
    token = strtok(ipstr, ".");  
  
    while (token != NULL) 
    {  
        cur = atoi(token);  
        if (cur >= 0 && cur <= 255) 
        {  
            total += cur * pow(256, i);  
        }  
        i --;  
        token = strtok(NULL, ".");  
    }
  
    return total;  
}  

int main(int argc, char ** argv)
{
	FILE * file_tcp = fopen("./traffic_tcp.csv", "r");
	FILE * file_udp = fopen("./traffic_udp.csv", "r");

	FILE * file_merge = fopen("./traffic_sender.dat", "w");

	char temp[210];
	char buf1[210];
	char buf2[210];
	int packet_num = 0;
	
	fscanf(file_tcp, "%s", buf1);
	fscanf(file_udp, "%s", buf2);

	fprintf(file_merge, "s_ip,d_ip,s_port,d_port,pro_type\n");

	while(fscanf(file_tcp, "%s", buf1) != EOF && fscanf(file_udp, "%s", buf2) != EOF)
	// while(fgets(buf1, 200, file_tcp) != NULL && fgets(buf2, 200, file_udp) != NULL)
	// will not read the '\n' !!!
	{

		vector<string> sep1 = split(string(buf1), ',');
		vector<string> sep2 = split(string(buf2), ',');

		if(sep1[0].length() == 0 || sep1[1].length() == 0 || sep1[4].length() == 0)
		{
			continue;
		}

		if(sep2[0].length() == 0 || sep2[1].length() == 0 || sep2[4].length() == 0)
		{
			continue;
		}

		strcpy(temp, sep1[0].c_str());
		uint sip1 = ipTint(temp);

		strcpy(temp, sep1[1].c_str());
		uint dip1 = ipTint(temp);
		uint sport1 = atoi(sep1[2].c_str());
		uint dport1 = atoi(sep1[3].c_str());
		uint type1 = atoi(sep1[4].c_str());


		strcpy(temp, sep2[0].c_str());
		uint sip2 = ipTint(temp);
		strcpy(temp, sep2[1].c_str());
		uint dip2 = ipTint(temp);
		uint sport2 = atoi(sep2[2].c_str());
		uint dport2 = atoi(sep2[3].c_str());
		uint type2 = atoi(sep2[4].c_str());

		if(sip1 != sip2 || dip1 != dip2 || type1 != type2)
		{
			printf("Fatal error!\n");
			continue;
		}

		if(type1 == 6 && (sport1 == 0 || dport1 == 0))
		{
			continue;
		}

		if(type1 == 17 && (sport2 == 0 || dport2 == 0))
		{
			continue;
		}

		if(type1 == 17)
		{
			sport1 = sport2;
			dport1 = dport2;
		}







		fprintf(file_merge, "%u\t%u\t%u\t%u\t%u\n", sip1, dip1, sport1, dport1, type1);
		packet_num ++;
		if(packet_num % 1000000 == 0)
		{
			printf("%dM packets,%u,%u,%u,%hu,%hu\n", packet_num / 1000000, sip1, dip1, (unsigned short)sport1, (unsigned short)dport1, (unsigned short)type1);
		}
	}

	return 0;
}

