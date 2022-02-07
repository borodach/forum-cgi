// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__9C45F458_29AA_11D5_9477_00C0262C68EE__INCLUDED_)
#define AFX_NODE_H__9C45F458_29AA_11D5_9477_00C0262C68EE__INCLUDED_

#include <fstream.h>
#include <time.h>
#include <string.h>

#define MAX_LINKS 4

#define LNK_FILE "links.dat"
#define MSG_FILE "msg.dat"


struct SNodeInfo
	{
	public:
	long links[MAX_LINKS+1];	//������ �� ������, ������������� -1
								//���� ������ ��������, �� �������� �� - �
								//��� ������ �� ���������� ������� ������
	
//	long parent;				//������ �� ������������ ����, 
								//������������� ��������, ���� ��� 
								//����������� ������ ������
	long message;				//����� ��������� � MSG_FILE


	};
ostream & operator <<(ostream &s,SNodeInfo &d);
istream & operator >>(istream &s,SNodeInfo &d);

struct SMsgData
	{
	char  flag;		//�����
	char* title;	//title
	char* name;		//name
	char* mail;		//mail
	char* msg;		//message
	time_t tm;		//����� ������ ���������
	
	long owner,parent;	//����� ��������� � ��� ������
	SMsgData()
		{
		title=name=mail=msg=NULL;
		tm=time(NULL);
		}
	void Reset()
	{
		if(title!=NULL) delete title;
		if(name!=NULL)	delete name;
		if(mail!=NULL)	delete mail;
		if(msg!=NULL)	delete msg;
		msg=mail=name=title=NULL;

	}
	~SMsgData()
	{
		Reset();
	}
	};


ostream & operator <<(ostream &s,SMsgData &d);
istream & operator >>(istream &s,SMsgData &d);

struct SNodeList:SNodeInfo
	{
	SNodeList *nxt;
	SNodeList(){nxt=NULL;}
	~SNodeList()
		{
		if(nxt!=NULL) delete nxt;
		}
	};

class CNode  
{
public:
SMsgData *message;
SNodeList *list;
long addr;
void flushFlags(ostream &s);	//���������� ����� �������� ������

void Reset();
CNode()
	{
	list=NULL;
	message=NULL;
	Reset();
	}
~CNode() 
	{
	Reset();
	}
void Find(istream &s,istream &s1,long number,int load_message=0);
//��������� ����, exception ��� ������
//��� ������� �� ���������
//0 - ���, 1 - �����, 2 - �����, ���� � ���������
int Create(	iostream &s,
			iostream &s1,
			char *title,
			char *name,
			char* mail,
			char* msg,
			int parent,
			time_t *tim=NULL,
			int fl=2);//������� ����, exception ��� ������	

SMsgData* getMessage()		//���������� ��������
	{
	return message;
	}

int getFlags()				//���������� ����� ������
	{
	return message->flag;
	}
					
void setDeleted()
	{
	message->flag=3;
	}
void setEnabled()
	{
	message->flag=0;
	}

};

#endif // !defined(AFX_NODE_H__9C45F458_29AA_11D5_9477_00C0262C68EE__INCLUDED_)
