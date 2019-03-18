#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

#define urlLength 100
#define Base_Url "www.chitkara.edu.in"

typedef struct LinkList  // structure for linked list
{



	 int Link_key;
     int Link_flag;
        char *Link_url;
        struct LinkList *Link_next;
        int Link_depth;



}LinkList;


struct Hashing  // structure for hashing
{
	struct LinkList *first;
	struct LinkList *last;
	int count;
};
struct Hashing hash[50];
LinkList *Head=NULL;
LinkList *last=NULL;


char* get_next_url(struct LinkList *head,int depth)
{
        struct LinkList *temp=head;
        while(temp!=NULL)
        {
                if(temp->Link_flag==0)
                {
                        temp->Link_flag=1;
                        return temp->Link_url;
                }
                temp=temp->Link_next;
        }
        return NULL;
}

int Find_Key(char *link)
{
	int i=0,key=0;
	while(link[i]!='\0')
	{
	  key+=(int)link[i++];
	}
	while(key>100)
	{
	  key/=10;
	}
	return key;
}

void putInList(char **result,int depth)  // function to put all the links in the linked list!!
{
	LinkList *temp=NULL,*p=NULL,*trav;
int i;
        for(i=0;i<10;i++)
        {
                temp=(struct LinkList*)calloc(1,sizeof(struct LinkList));
                temp->Link_key=Find_Key(result[i]);
                int key=temp->Link_key;
                temp->Link_url=result[i];
                temp->Link_depth=depth+1;
                if(Head==NULL)
                {
                    temp->Link_next=NULL;
                    temp->Link_flag=1;
                    temp->Link_depth=depth;
                    Head=temp;                      //   temp->Link_flag=1;
                    last=temp;
                    hash[key].first=temp;
                    hash[key].last=temp;
                    hash[key].count=1;
                }
                else
                {
                        if(hash[key].first==NULL)
                        {
                               last->Link_next=temp;
                               temp->Link_next=NULL;
                               temp->Link_flag=0;
                               last=temp;
                               hash[key].first=temp;
                               hash[key].last=temp;
                               hash[key].count=1;
                        }
                        else
                        {
                            trav=hash[key].first;
                            if((strcmp(hash[key].first->Link_url,temp->Link_url)==0)||(strcmp(hash[key].last->Link_url,temp->Link_url)==0))
                                {
                                    free(temp);
                                    continue;
                                }
                            int f=0;
                            while(trav->Link_next!=NULL && trav->Link_key==key)
                            {
                                if(strcmp(trav->Link_url,temp->Link_url)==0)
                                {
                                    free(temp);
                                    f=1;
                                    break;
                                }
                                trav=trav->Link_next;
                            }
                            if(f)
                            continue;
                            if(trav->Link_next==NULL)
                            last=temp;
                            temp->Link_flag=0;
                            temp->Link_next=trav->Link_next;
                            trav->Link_next=temp;
                            hash[key].last=temp;
                            hash[key].count++;
                        }
                }
        }
LinkList *print = Head;
while(print!=NULL)
{
printf("%s %d %d\n",print->Link_url,print->Link_flag,print->Link_depth);
print=print->Link_next;
}
}

void removeWhiteSpace(char* html)  // function will remove all the white spaces from the temp file to make link searching bit faster!!
{
  int i;
  char *buffer = malloc(strlen(html)+1), *p=malloc (sizeof(char)+1);
  memset(buffer,0,strlen(html)+1);
  for (i=0;html[i];i++)
  {
    if(html[i]>32)
    {
      sprintf(p,"%c",html[i]);
      strcat(buffer,p);
    }
  }
  strcpy(html,buffer);
  free(buffer); free(p);
}

int GetNextURL(char* html, char* urlofthispage, char* result, int pos) // function will fetch the url and its line no one by one!! we used line no to make searching faster!!
{
  char c;
  int len, i, j;
  char* p1;
  char* p2;

  if(pos == 0) {
    removeWhiteSpace(html);
  }

  while (0 != (c = html[pos]))
  {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }

  if (c)
  {
    p1 = strchr(&(html[pos+1]), '=');

    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10))
    {
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;

    p1++;

    p2 = strpbrk(p1, "\'\">");
    if (!p2)
    {
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#')
    {
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4))
    {
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    } else {
      if (p1[0] == '.') {
        return GetNextURL(html,urlofthispage,result,pos+1);
      }
      if (p1[0] == '/') {
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = 0;
        strncat(result, p1, (p2 - p1));
        return (int)(p2 - html + 1);
      } else {
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
          return (int)(p2 - html + 1);
        }
        strcpy(result, urlofthispage);
        result[i + 1] = 0;
        strncat(result, p1, p2 - p1);
        return (int)(p2 - html + 1);
      }
    }
  }
  return -1;
}

char* convertData()  // function will contain all the data in the temp file in char array for searching links in it!!
{
	struct stat st;
	stat("temp.txt", &st);
	int File_Size = st.st_size, i=0;
	char *File_Content = (char*)malloc(sizeof(char) * File_Size);
	char ch;
	FILE *file = fopen("temp.txt", "r");
	ch = getc(file);
	while(ch != EOF)
	{
	  File_Content[i] = ch;
	  ch = getc(file);
	  i++;
	}
	 File_Content[i] = '\0';
	 fclose(file);
	return File_Content;
}

void Fetch_Url(char *url,int dept)  // function will take url from nextGenurl function and put it in the array and check duplicay wheater url exits in array or not!!
{
    struct stat st;
    stat("temp.txt", &st);
    int File_Size = st.st_size;
    File_Size++;

	char *data = (char*)malloc(File_Size*sizeof(char));
	data = convertData();

	char *result = (char*)malloc(File_Size*sizeof(char));
        int ans=0,flag=0,l=0;
        char **links;

    links=(char **)malloc(sizeof(char *)*100);
    for(int i=0;i<100;i++)
      links[i] = (char *)malloc(sizeof(char)*200);

	ans=GetNextURL(data,url,result ,0);
	strcpy(links[l++],result);

    while(l<10)
    {
      ans=GetNextURL(data,url,result ,ans);

      for(int j=0;j<l;j++)
      {
        if(strcmp(result,links[j])==0)
        {
          flag=1;
          break;
        }
      }
      if(flag==0){
       strcpy(links[l++],result);
      }
      else{
       ans=GetNextURL(data,url,result ,ans);
       flag=0;
      }
    }
	putInList(links,dept);
}

char Transfer_File()  // function to shift data from temp file to new actual file
{
	char File_Name[10],ch;
	static int file_no=1;
	sprintf(File_Name,"%d",file_no);
	strcat(File_Name, ".txt");
        FILE *oldFileName = fopen("temp.txt" , "r");
	FILE *newFileName = fopen(File_Name, "w");
	ch = getc(oldFileName);
	while(ch != EOF)
	{
	   putc(ch, newFileName);
	   ch = getc(oldFileName);
	}
	file_no++;
	printf("New File Created");
	fclose(oldFileName);
}

void get_Page(char *url)  // function to fetch url from user and contact in urlBuffer and fetch page source code and add it to temp file!!
{
    char urlBuffer[urlLength + 300] = {0};
    strcat(urlBuffer, "wget -O ");
    strcat(urlBuffer, "./temp.txt ");
    strcat(urlBuffer, url);
    system(urlBuffer);
    Transfer_File();
}

int Check_Argument(int check)  // function to check wheater user has entered total 3 arguments or not!!
{
    if(check==4)
        return 1;
        printf("ARGUMENTS ARE NOT PROPER");
        exit(0);
}

int Check_Depth(char* argv[])  // function to check wheater depth entered in postive or negative!!
{
    int depth;
    sscanf(argv[2], "%d" , &depth);
    if(depth<0)
    {
        printf("DEPTH CAN'T BE NEGATIVE");
        exit(0);
    }
    return 1;
}

int Check_Url(char* argv[])  // function to check wheater url is correct or not!!
{
    char *b = (char*)malloc(sizeof(char)*300);
    strcat(b,"wget --spider ");
    strcat(b,argv[1]);
    if(!system(b))
    {
        if(strcmp(Base_Url,argv[1]))
        {
            printf("URL IS NOT CORRECT");
            exit(0);
        }
        else
            return 1;
    }
}

int Check_Dir(char* argv[]) // function to check dir is available or not and correct dir path is entered or not!!
{
    struct stat statbuf;
    if( stat(argv[3],&statbuf) == -1)
    {
        printf("Ivalid directory\n");
        exit(0);
    }
    if( !S_ISDIR(statbuf.st_mode))
    {
        printf("Invalid directory entry.Your input is't a directory\n");
        exit(0);
    }
    if( (statbuf.st_mode & S_IWUSR) !=S_IWUSR)
    {
        printf("Invalid directory entry. It isn't writable\n");
        exit(0);
    }
    return 1;
}

void Check_Arguments(int argc,char* argv[])  // function to check whether all arguments are correct or not!!
{
    	if(Check_Argument(argc))
        {
            if(Check_Depth(argv))
            {
                if(Check_Url(argv))
                {
                    if(Check_Dir(argv))
                    {
                        printf("All Arguments are correct\n");
                    }
                }
            }
        }
}

int main(int argc,char* argv[])
{
    Check_Arguments(argc,argv);
    get_Page(argv[1]);
    Fetch_Url(argv[1],1);

    LinkList *print1 = Head;

    printf("\n \n \n -------------------------------------- \n \n \n");

    /*for(int i=0;i<1;i++)
    {
      while(print1!=NULL)
      {
      if(print1->Link_flag==0)
      {
        get_Page(print1->Link_url);
        Fetch_Url(print1->Link_url,print1->Link_depth);
        break;
      }
        print1=print1->Link_next;
      }
    }*/

    int depth=2;
        char *url=NULL;
        while(depth<3)
        {
                while((url=get_next_url(Head,depth))!=NULL)
                {
                        get_Page(url);
                        Fetch_Url(url,depth+1);
                }
                depth++;
        }
}
