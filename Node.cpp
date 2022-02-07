// Node.c: implementation of the CNode class.
//
//////////////////////////////////////////////////////////////////////

#include "Node.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//запись и чтение строк для двоичного файла
void putString(ostream &stream,char* str)	
	{
	if(str==NULL) throw 1;
	int len=strlen(str);
	stream.write((char*)(&len),sizeof(len));
	if(!stream) throw 3;
	if(len) stream.write(str,len);
	if(!stream) throw 3;
	}
char* getString(istream &stream)
	{
	int len;
	stream.read((char*)(&len),sizeof(len));
	if(!stream) throw 3;
	char *str=new char[len+1];
	str[len]=0;
	if(len) stream.read(str,len);
	if(!stream) throw 3;
	return str;
	}



ostream & operator <<(ostream &s,SNodeInfo &d)
	{
	s.write((char*)(&d),sizeof(SNodeInfo));
	if(!s) throw 3;
	return s;
	}
istream & operator >>(istream &s,SNodeInfo &d)
	{
	s.read((char*)(&d),sizeof(SNodeInfo));
	if(!s) throw 3;
	return s;
	}

ostream & operator <<(ostream &s,SMsgData &d)
	{
	
	s.write((char*)(&(d.flag)),sizeof(d.flag));
	if(!s) throw 3;
	
	s.write((char*)(&(d.parent)),sizeof(d.parent));
	if(!s) throw 3;
	
	
	s.write((char*)(&(d.tm)),sizeof(d.tm));
	if(!s) throw 3;

	s.write((char*)(&(d.owner)),sizeof(d.owner));
	if(!s) throw 3;

	putString(s,d.title);
	putString(s,d.name);
	putString(s,d.mail);
	putString(s,d.msg);

	
	

	return s;
	}

istream & operator >>(istream &s,SMsgData &d)
	{
	
	s.read((char*)(&(d.flag)),sizeof(d.flag));
	if(!s) throw 3;
	
	s.read((char*)(&(d.parent)),sizeof(d.parent));
	if(!s) throw 3;

	s.read((char*)(&(d.tm)),sizeof(d.tm));
	if(!s) throw 3;

	s.read((char*)(&(d.owner)),sizeof(d.owner));
	if(!s) throw 3;


	d.title=getString(s);
	d.name=getString(s);
	d.mail=getString(s);
	d.msg=getString(s);

	
	

	return s;
	}

void loadFlags(istream &s,SMsgData &d)
	{
	s.read((char*)(&(d.flag)),sizeof(d.flag));
	if(!s) throw 3;
	
	s.read((char*)(&(d.parent)),sizeof(d.parent));
	if(!s) throw 3;
	
	}
void loadHead(istream &s,SMsgData &d)
	{
	s.read((char*)(&(d.flag)),sizeof(d.flag));
	if(!s) throw 3;
	
	s.read((char*)(&(d.parent)),sizeof(d.parent));
	if(!s) throw 3;

	s.read((char*)(&(d.tm)),sizeof(d.tm));
	if(!s) throw 3;

	s.read((char*)(&(d.owner)),sizeof(d.owner));
	if(!s) throw 3;

	d.title=getString(s);
	}

void CNode::Reset()
	{
	if(message!=NULL)	delete message;
	if(list!=NULL)		delete list;
	message=NULL;
	list=NULL;
	}
//
void CNode::Find(istream &s,istream &s1,long number,int load_message)
	{
	
	addr=number;
	long pos=number*sizeof(SNodeInfo);
	s.seekg(pos,ios::beg);
	if(!s) throw 3;
	Reset();
	list=new SNodeList();
	s>>*list;
	if(list->message<0) 
	{
		Reset();
		throw "Ошибка";	//это продолжение таблицы ссылок
	}
	SNodeList *t=list;
	SNodeList *p;
	//загружаем список продолжений таблицы ссылок
	while(t->links[MAX_LINKS]>-1)
		{
		p=new SNodeList();
		pos=t->links[MAX_LINKS]*sizeof(SNodeInfo);
		s.seekg(pos,ios::beg);
		s>>*p;
		t->nxt=p;
		t=p;
		}
	
	if(load_message<0) return;
	message=new SMsgData();
	s1.seekg(list->message,ios::beg);
	if(!s1) throw 3;
	if(load_message==0)
		{
		s1>>*message;
		}
	if(load_message==1)
		{
		loadFlags(s1,*message);
		}
	if(load_message==2)
		{
		loadHead(s1,*message);
		}
	}
//файлы должны быть открыты в монопольном режиме
int CNode::Create(	iostream &s,
					iostream &s1,
					char *title,
					char *name,
					char* mail,
					char* msg,
					int parent,
					time_t *tim,
					int fl)//создает узел, exception при ошибке	
	{
	
	

	if(parent<0)	//добавляем вопрос
		{
		
		s.seekp(0,ios::end);
		if(!s) throw 3;
		long p1=(s.tellp())/sizeof(SNodeInfo);
		SNodeList tmp;
		SMsgData md;
		md.flag=fl;
		md.mail=mail;
		md.title=title;
		md.msg=msg;
		md.name=name;
		md.owner=p1;
		md.parent=-1;
		if(tim!=NULL) md.tm=*tim;

		s1.seekp(0,ios::end);
		if(!s) throw 3;
		long pp=s1.tellp();
		s1<<md;
		//записали сообщение

		//чтобы деструктор не вызывал delete
		md.mail=NULL;
		md.title=NULL;
		md.msg=NULL;
		md.name=NULL;
		
//		tmp.parent=p1;		//если parent == текущему адресу, то нет родителя
		tmp.message=pp;
		for(int i=0;i<=MAX_LINKS;i++)tmp.links[i]=-1;
		
		s<<tmp;
	
		return addr=p1;
		}


	//ищем родителя

	long pos=parent*sizeof(SNodeInfo);
	long	parent_pos=pos,
			new_pos;
	SNodeList tmp;

	s.seekg(pos,ios::beg);
	if(!s) throw 3;
	Reset();
	//читаем родителя
	s>>tmp;
	if(tmp.message<0)
	{
		throw "Ошибка.";
	}

	//читаем список прдолжений таблицы ссылок родителя
	while(tmp.links[MAX_LINKS]>-1)
		{
		pos=tmp.links[MAX_LINKS]*sizeof(SNodeInfo);
		s.seekg(pos,ios::beg);
		if(!s) throw 3;
		s>>tmp;
		}
	//tmp - parent
	//pos - parent address
	//i - index in parents links
	int i=0;
	if(tmp.links[MAX_LINKS-1]==-1)	//есть место в массиве ссылок
		{
		for(i=0;(i<MAX_LINKS)&&(tmp.links[i]>-1);i++);
		
		//ответ будет в конце
		s.seekp(0,ios::end);
		if(!s) throw 3;
		new_pos=s.tellp();
		
		tmp.links[i]=new_pos/(sizeof(SNodeInfo));
		//записали адрес ответа
		s.seekp(pos,ios::beg);
		if(!s) throw 3;
		s<<tmp;
		//s.close();
		//записали измененную таблицу
		}
	else	//нет места в массиве ссылок
		{
		
		//ответ будет в конце
		s.seekp(0,ios::end);
		if(!s) throw 3;
		new_pos=s.tellp();	//куда добавляем продолжение таблицы ссылок
		
		SNodeInfo new_node=tmp;		//сформировали продолжение таблицы
		new_node.links[0]=new_pos/(sizeof(SNodeInfo))+1;	
		//записали адрес ответа
		
		//new_node.parent=-parent_pos-1;	//запомнили родителя
		new_node.message=-1;

		for(int i=1;i<=MAX_LINKS;i++)new_node.links[i]=-1;	
		s<<new_node;					
		//записали продолжение таблицы ссылок
		
		
		//записали адрес продолжения таблицы ссылок
		tmp.links[MAX_LINKS]=new_pos/(sizeof(SNodeInfo));
		s.seekp(pos,ios::beg);
		if(!s) throw 3;
		s<<tmp;
	//	s.close();
		//exit(1);
	
		}

	s.seekp(0,ios::end);
	if(!s) throw 3;
	new_pos=s.tellp()/(sizeof(SNodeInfo));

	//Запись текста собщения
	SMsgData md;
	md.flag=fl;		//запрещено, не удалено
	md.mail=mail;
	md.title=title;
	md.msg=msg;
	md.name=name;
	md.parent=parent;
	md.owner=new_pos;
	if(tim!=NULL) md.tm=*tim;
	
	s1.seekp(0,ios::end);
	if(!s1) throw 3;
	long pp=s1.tellp();
	s1<<md;		//запись сообщения

	//чтобы деструктор не вызывал delete
	md.mail=NULL;
	md.title=NULL;
	md.msg=NULL;
	md.name=NULL;

	//tmp.parent=parent;	//запомнили родителя
	tmp.message=pp;
	for(i=0;i<=MAX_LINKS;i++) tmp.links[i]=-1;;
	s<<tmp;
	return addr=new_pos;

	}
					
void CNode::flushFlags(ostream &s)
	{
	s.seekp(list->message,ios::beg);
	if(!s) throw 3;
	s.write((char*)(&(message->flag)),sizeof(message->flag));
	if(!s) throw 3;
	}
/*void main()
	{
	CNode node;
	fstream s0(LNK_FILE,ios::out|ios::in);
	fstream s1(MSG_FILE,ios::out|ios::in);
	
	node.Create(s0,s1,"title1","name1","mail1","message1",-1);
	node.Create(s0,s1,"title2","name2","mail2","message2",-1);
//	node.Create(s0,s1,"title12","name12","mail12","message2",-1);
	node.Create(s0,s1,"title11","name11","mail11","message11",0);
	node.Create(s0,s1,"title12","name12","mail12","message12",0);
	node.Create(s0,s1,"title13","name13","mail13","message13",0);
	node.Create(s0,s1,"title14","name14","mail14","message14",0);
	node.Create(s0,s1,"title15","name15","mail15","message15",0);
	
//	node.Find(s0,s1,0);


	s0.close();
	s1.close();
	}
*/