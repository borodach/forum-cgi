// Manager.cpp: implementation of the CManager class.
//
//////////////////////////////////////////////////////////////////////

#include "Manager.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char *env1,*env2,*nm;

CManager::CManager()
{
lnk=lnk1=msg=msg1=NULL;
}

CManager::~CManager()
{

}

void CManager::ShowNode(int n)
{
CNode node;
try
	{
	node.Find(*lnk,*msg,n);
	}
catch(...)
	{
//	fstream.
	throw;
	}
if(((!admin)&&(node.getFlags()&2))||	//не проверен
			(node.getFlags()&1))		//удален
			throw 5;
cout<<"<table><tr><td>";
cout<<"<STRONG>Name:</STRONG></td><td>"<<node.message->name<<"</td></tr>"<<endl;
cout<<"<tr><td><STRONG>mail:</STRONG></td><td> <a href=\"mailto:"
<<node.message->mail<<"\">"<<node.message->mail<<"</a></td></tr>"<<endl;
cout<<"<tr><td><STRONG>Title:</STRONG></td><td>"<<node.message->title<<"</td></tr>"<<endl;
cout<<"<tr><td><STRONG>Date:</STRONG></td><td>"<<ctime(&(node.message->tm))<<"</td></tr>"<<endl;
cout<<"</table><HR>";
cout<<"<STRONG>Message:</STRONG>"<<node.message->msg<<"<HR>"<<endl;
SNodeList* l=node.list;
cout<<"<UL>";

cout<<"<form method=post>";
cout<<"<textarea cols=40 name=\"ans\"> </textarea>";
cout<<"<textarea cols=40 name=\"ans0\"> </textarea>";
cout<<"<input type=\"submit\">";
cout<<"</form>";

while(l!=NULL)
	{
	for(int i=0;(i<MAX_LINKS)&&(l->links[i]>-1);i++)
	{
		cout<<"<UL>";
		ShowTree(l->links[i],0);
		cout<<"</UL>\n";
	}
	l=l->nxt;
	}
cout<<"</UL>\n";
}

void CManager::ShowAll()
{
lnk->seekg(0,ios::end);
int j=lnk->tellg()/sizeof(SNodeInfo);
CNode node;
cout<<"<UL>";
for(int i=0;i<j;i++)
	{
	try
	{
	node.Find(*lnk,*msg,i,1);
	}
	catch(char *)
	{
		continue;
	}
	catch(...)
	{
		throw;
	}
	if(node.message->parent>-1) continue;
	if(((!admin)&&(node.getFlags()&2))||	//не проверен
			(node.getFlags()&1))			//удален
		continue;
	cout<<"<UL>";
	ShowTree(i,0);
	cout<<"</UL>\n";
	}
cout<<"</UL>\n";
}

void CManager::ShowNew()
{
lnk->seekg(0,ios::end);
int j=lnk->tellg()/sizeof(SNodeInfo);
CNode node;
for(int i=0;i<j;i++)
	{
	try
	{
		node.Find(*lnk,*msg,i,1);
	}
	catch(char *)
	{
		continue;
	}
	catch(...)
	{
		throw;
	}
	//пропускаем удаленные и не новые
	if((node.getFlags()&3)!=2) continue;
	ShowNode(i);
	}
}

void CManager::gc()
{
//загружаем в список все неудаленные сообщения,
//в списке - SMsgData + номер parent
lnk->seekg(0,ios::end);
int size=lnk->tellg()/sizeof(SNodeInfo);
CNode node;
//size максимальное число блоков
struct info
{
	long new_addr,old_owner;
	char del;
};
info *old=new info[size];
int sz=0;
try
{
msg->seekg(0,ios::beg);
SMsgData d;
int i;
int dl=0;
while(1)
{	
	try
	{
	*msg>>d;
	dl=d.flag&1;
	}
	catch(...)
	{
		if(msg->eof()) {msg->clear();break;}
		else throw;
	}
	long addr=-1;
	//Ищем новый адрес родителя
	if(d.parent>0)
	{
		for(i=0;i<sz;i++)
		{
			if(old[i].old_owner==d.parent) break;
		}
		if(!dl) dl=old[i].del;
		addr=old[i].new_addr;
	}
	int j=sz;

	for(j;(j>0)&&(old[j-1].old_owner>d.owner);j--) old[j]=old[j-1];
	old[j].old_owner=d.owner;
	old[j].del=dl;
	sz++;
	if(!dl) old[j].new_addr=node.Create(*lnk1,*msg1,d.title,d.name,d.mail,d.msg,addr,&(d.tm),d.flag);
	//вписали новый адрес
	d.Reset();
}

}
catch(...)
{
	delete old;
	throw;
}
delete old;

}

void CManager::ShowTree(long n,int num)
{

CNode node;
node.Find(*lnk,*msg,n,2);	
	if(((!admin)&&(node.getFlags()&2))||	//не проверен
			(node.getFlags()&1))		//удален
			return;
//for(int j=0;j<num;j++)cout<<'\t';
cout<<"<LI> <A HREF=\""<<nm<<"?1&"<<n<<"\">"<<node.message->title<<"</A>\n";
SNodeList* l=node.list;
if(l->links[0]>-1) 
{
	cout<<"<UL>\n";
}
else return;
while(l!=NULL)
	{
	for(int i=0;(i<MAX_LINKS)&&(l->links[i]>-1);i++)
		ShowTree(l->links[i],num+1);
	l=l->nxt;
	}
cout<<"</UL>\n";
}

#include<stdio.h>

void main(int ac,char*args[])
	{
	//char bf[128];

	cout<<"Content-type: text/html\n\n";
	cout<<"<body BGCOLOR=\"808080\">";
	//cin>>bf;
	//cout<<bf<<"\t";
	fstream s0(LNK_FILE,ios::out|ios::in|ios::binary);
	fstream s1(MSG_FILE,ios::out|ios::in|ios::binary);

	fstream ss0("1.dat",ios::trunc|ios::out|ios::in|ios::binary);
	fstream ss1("2.dat",ios::trunc|ios::out|ios::in|ios::binary);
	
	
	CNode node;/*
	node.Create(s0,s1,"title1","name1","mail1","message1",6);
	node.Create(s0,s1,"title2","name2","mail2","message2",6);
	node.Create(s0,s1,"title13","name1","mail13","message3",-1);
	node.Create(s0,s1,"title11","name11","mail11","message11",4);
	node.Create(s0,s1,"title12","name12","mail12","message12",4);
	node.Create(s0,s1,"title13","name13","mail13","message13",4);
	node.Create(s0,s1,"title14","name14","mail14","message14",4);
	node.Create(s0,s1,"title15","name15","mail15","message15",4);
	*/
/*
	node.Find(s0,s1,1);
	node.setDeleted();
	node.flushFlags(s1);
	*/
	CManager m;
	m.lnk=&s0;
	m.msg=&s1;
	m.lnk1=&ss0;
	m.msg1=&ss1;
	m.admin=1;
	env1=getenv("CONTENT_LENGTH");
	if(env1!=NULL) 
	{
		cout<<"ans: "<<env1<<endl;
		char*bf=new char[atoi(env1)+1];
		for(int i=0; i<atoi(env1); i++) bf[i]=getc(stdin);
		bf[i]=0;
		cout<<bf<<endl;
		delete bf;

	}
	env1=getenv("QUERY_STRING");
	nm=getenv("SCRIPT_NAME");
	if(nm==NULL)nm="";

	//m.admin=0;
	//m.gc();
	int c=0,d=0;
	if((env1==NULL)||strlen(env1)==0)
	{
		
		m.ShowAll();
	}
	else
	{	
		for(int i=0;(env1[i]!='&')&&(env1[i]!=0);i++) c=c*10+(env1[i]-'0');
		
		if(env1[i]=='&')i++;
		for(;(env1[i]!=0);i++) d=d*10+(env1[i]-'0');
		
		
		if(c==1)m.ShowNode(d);
	}
	
	


//	CNode node;
/*	node.Create(s0,s1,"title1","name1","mail1","message1",-1);
	
	node.Create(s0,s1,"title2","name2","mail2","message2",-1);
    node.Create(s0,s1,"title13","name1","mail13","message3",-1);
	node.Create(s0,s1,"title11","name11","mail11","message11",2);
	node.Create(s0,s1,"title12","name12","mail12","message12",2);
	node.Create(s0,s1,"title13","name13","mail13","message13",2);
	node.Create(s0,s1,"title14","name14","mail14","message14",2);
	node.Create(s0,s1,"title15","name15","mail15","message15",2);
	
	node.Create(s0,s1,"title11","name11","mail11","message11",4);
	node.Create(s0,s1,"title12","name12","mail12","message12",4);
	node.Create(s0,s1,"title13","name13","mail13","message13",4);
	node.Create(s0,s1,"title14","name14","mail14","message14",4);
	node.Create(s0,s1,"title15","name15","mail15","message15",4);
*/

	s0.close();
	s1.close();
	ss0.close();
	ss1.close();
	int a;
	cout<<"</body>"<<endl;
//cin>>a;	
}