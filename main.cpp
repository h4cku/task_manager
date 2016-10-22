#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

#define MAX_LENGTH 100

string DIR_TASK_LIST_DEFAULT = "/home/elvin/codes_github/task_manager/tasks/";
string NAME_FILE_DEFAULT("task_list.dat");


int count_tasks(FILE *f)
{
	int out = 0;
	char aux[MAX_LENGTH];
	while(feof(f)!=0)
	{
		fgets(aux,MAX_LENGTH,f);
		out = out + 1;
	}
	rewind(f);	
}

string read_file(FILE *f)
{
	stringstream s_aux;
	char aux[MAX_LENGTH];
	while(feof(f)!=0)
	{
		fgets(aux,MAX_LENGTH,f);
		s_aux << aux;
	}
	return s_aux.str();
}

vector<string> file_to_vector(FILE *f)
{
	string s_aux;
	vector<string> v;
	char buf[MAX_LENGTH];
	while(!feof(f))
	{
		fgets(buf,MAX_LENGTH,f);
		s_aux = string(buf);
		if(s_aux.length() > 10)
			v.push_back(string(buf));
	}
	return v;
}

void print_tasks(vector<string> v)
{
	cout<<"[T]"<<"ask"<<endl;
	cout<<"[D]"<<"eadline"<<endl;
	cout<<"[P]"<<"riority"<<endl;
	cout<<"[S]"<<"tatus"<<endl;
	cout <<"\n";
	cout<<"ID"<<"\t"<<"T"<<"\t"<<"D"<<"\t"<<"P"<<"\t"<<"S"<<"\t"<<endl;
	for(int i = 0;i<v.size();i++)
		cout << "["<<i+1<<"]\t"<<v.at(i);
	cout << "\n";
}

void print_help()
{
	string help("taskm [command] [option1] [option2] [option3]\nCommands:\nprint:\t\tPrint all the tasks in the\
 list\nhelp:\t\tShow the panel help\nadd:\t\tAdd a new task with name specified in option1, deadline specified\ 
 in option2 and priority specified in option3\nclear:\t\tClear the whole task list\ndelete:\t\tDelete the taks\
 in the index specified in option1\n");
	cout <<help<<endl;
}

void add_task(vector<string> &v, char* name_task, char* deadline, char* priority)
{
	stringstream aux;
	aux<<name_task<<"\t"<<deadline<<"\t"<<priority<<"\tActive"<<endl;
	v.push_back(aux.str());
}

void clear_task_list(vector<string> &v,string dir_file,string name_file)
{
	v.clear();
}

void delete_task(vector<string> &v,int index)
{
	v.erase(v.begin()+index-1);
}

void done_task(vector<string> &v,int index)
{
	string aux = v.at(index - 1);
	int i = aux.find("Active");
	if(i != aux.npos)
		aux.replace(i,6,"Done");
	v.at(index-1) = aux;
}

void active_task(vector<string> &v,int index)
{
	string aux = v.at(index - 1);
	int i = aux.find("Pause");
	if(i != aux.npos)
		aux.replace(i,6,"Active\n");
	v.at(index-1) = aux;
}

void pause_task(vector<string> &v,int index)
{
	string aux = v.at(index - 1);
	int i = aux.find("Active");
	if(i != aux.npos)
		aux.replace(i,6,"Pause\n");
	v.at(index-1) = aux;
}

void update_file(vector<string> v,FILE *f)
{
	for(int i = 0;i<v.size();i++)
	{
		fprintf(f,"%s",v.at(i).c_str());
	}
	if(v.size() == 0)
		fprintf(f,"");
	else
		fprintf(f,"\n");
}

int interpreting_command(char* command)
{
	string com(command);
	if(com.compare("print") == 0)
		return 1;
	if(com.compare("add") == 0)
		return 2;
	if(com.compare("clear") == 0)
		return 3;
	if(com.compare("delete") == 0)
		return 4;
	if(com.compare("help") == 0)
		return 5;
	if(com.compare("done") == 0)
		return 6;
	if(com.compare("pause") == 0)
		return 7;
	if(com.compare("active") == 0)
		return 8;
	return 0;
}

int main(int argc, char* argv[])
{
	FILE *f;
	string name_file(NAME_FILE_DEFAULT);
	string dir_file(DIR_TASK_LIST_DEFAULT);
	f = fopen((dir_file + name_file).c_str(),"a+");
	vector<string> list = file_to_vector(f);
	fclose(f);
	switch(interpreting_command(argv[1]))
	{
		case 1:
			print_tasks(list);
			break;
		case 2:
			add_task(list,argv[2],argv[3],argv[4]);
			break;
		case 3:
			clear_task_list(list,dir_file,name_file);
			break;
		case 4:
			delete_task(list,atoi(argv[2]));
			break;
		case 5:
			print_help();
			break;
		case 6:
			done_task(list,atoi(argv[2]));
			break;
		case 7:
			pause_task(list,atoi(argv[2]));
			break;
		case 8:
			active_task(list,atoi(argv[2]));
			break;
		default:
			break;
	}
	f = fopen((dir_file + name_file).c_str(),"w");
	update_file(list,f);
	fclose(f);
	return 0;
}
