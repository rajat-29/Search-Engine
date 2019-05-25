#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BASE_URL "www.chitkara.edu.in"
//STRUCTURE of NODE
typedef struct node
{
		struct node *prev;
	char *node_url;
	int node_depth;
	int node_flag;
	int node_key;
	struct node *next;
}node;
node * head=NULL;
//STRUCTURE of HASH TABLE
struct hash
{
	struct node *first;
	struct node *last;
};
struct hash h[50];
//null_func
void null_func()
{
	head=NULL;
	int i=0;
	for(i=0;i<50;i++)
	{
		h[i].first=NULL;
		h[i].last=NULL;
	}
}
//HASH FUNCTION

int hash_func(char s[])
{
			int i=0,sum=0;
			for(i=0;s[i]!='\0';i++)
			{
				sum=sum+s[i];
			}
			return sum;
}
//check duplicate in linked list
int check_duplicate(node *k)
{
	node *chk=head;
	while(chk!=NULL)
	{
		if(strcmp(chk->node_url,k->node_url)==0)
		{
			return 0;
		}
		chk=chk->next;
	}
	return 1;
}


//NODE INSERT FUNCTION
node* insert_in_linklist(char **result,int dep)
{
	printf("\n****************IN LINKED LIST FUNCTION**********\n");
	  node *temp=NULL,*p=NULL;
		int i=0,index;
		for(i=0;i<51;i++)
		{
			printf("\n 11111111111111  \n");
			temp=(node*)calloc(1,sizeof(node));
			temp->node_url=result[i];
			temp->prev=NULL;
			temp->next=NULL;
			index=hash_func(result[i]);
			index=index%50;
			temp->node_key=index;
			if(head==NULL)
			{
					temp->node_depth=dep;
					temp->node_flag=1;
					head=temp;
					h[index].first=temp;
					h[index].last=temp;
			}
			else
			{
							if(check_duplicate(temp)==1)
							{
								temp->node_depth=dep+1;
							temp->node_flag=0;
								if(h[index].first==NULL)
								{

									h[temp->node_key].first=temp;//insert temp addr at that index and insert temp at the end of linked list
									h[temp->node_key].last=temp;
									p=head;
									while(p->next!=NULL)
									p=p->next;
									p->next=temp;
									temp->prev=p;
								}
									else if(h[index].first!=NULL)
									{
										node *k=NULL,*k1=NULL;
										k=h[temp->node_key].last;
										k1=k->next;
										if(k1==NULL)
										{
											temp->prev=k;
											temp->next=NULL;
										}
									else
									{
										temp->prev=k;
										temp->next=k1;
											k1->prev=temp;
									}

										k->next=temp;
										h[index].last=temp;
									}
							}
								else
								{
									free(temp);
								}
				}
			}

	return head;
}
//LINKED LIST PRINT FUNCTION
void print_linked_list()
{
	printf("\n****************IN PRINT FUNCTION********************\n");

	node *p=head;
	while(p!=NULL)
	{
		printf("%s  %d  %d\n",p->node_url,p->node_depth,p->node_flag);
		p=p->next;
	}
}
//CHECK_URL_FUNCTION
int check_url(char url[])
{
		char url_temp[strlen(url)+50];
		strcpy(url_temp,"wget --spider ");
		strcat(url_temp,url);
		if(!system(url_temp)) //return 0 for system command inbuilt
		{
			return 1;
		}
		else
			return 0;
}
//__CHECK_DEPTH_FUNCTION
int check_depth(char depth[])
{
    int flag=0;
    for(int i=0;i<strlen(depth);i++)
    {
        if(depth[i]>=48&&depth[i]<=57) continue;
        else
        {
        flag=1;
        break;
        }
    }
    if(flag==1)
    {printf("\nENTER DEPTH IN NUMERIC\n");
    return 0;
    }
    else
    {
     int d=atoi(depth);
        if(d>=1&&d<=5){return 1;}
        else
        {
        printf("\nENTER RANGE BTW 1 & 5\n");
        return 0;
        }
    }
}
void testDir(char *dir)
{
  struct stat statbuf;
  if ( stat(dir, &statbuf) == -1 )
  {
    printf( "-----------------\n");
    printf( "Invalid directory\n");
    printf( "-----------------\n");
    exit(1);
  }

  //Both check if there's a directory and if it's writable
  if ( !S_ISDIR(statbuf.st_mode) )
  {
    printf( "-----------------------------------------------------\n");
    printf( "Invalid directory entry. Your input isn't a directory\n");
    printf("-----------------------------------------------------\n");
    exit(1);
  }

  if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR )
  {
    printf( "------------------------------------------\n");
    printf( "Invalid directory entry. It isn't writable\n");
    printf("------------------------------------------\n");
    exit(1);
  }
}
//REMOVE_WHITE_SPACE
void removeWhiteSpace(char* html)
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
//GetNextURL FUNC
int GetNextURL(char* html, char* urlofthispage, char* result, int pos)
{
  char c;
  int len, i, j;
  char* p1;  //!< pointer pointed to the start of a new-founded URL.
  char* p2;  //!< pointer pointed to the end of a new-founded URL.

  // NEW
  // Clean up \n chars
  if(pos == 0) {
    removeWhiteSpace(html);
  }
  // /NEW

  // Find the <a> <A> HTML tag.
  while (0 != (c = html[pos]))
  {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }
  //! Find the URL it the HTML tag. They usually look like <a href="www.abc.com">
  //! We try to find the quote mark in order to find the URL inside the quote mark.
  if (c)
  {
    // check for equals first... some HTML tags don't have quotes...or use single quotes instead
    p1 = strchr(&(html[pos+1]), '=');

    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10))
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;

    p1++;

    p2 = strpbrk(p1, "\'\">");
    if (!p2)
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#')
    { // Why bother returning anything here....recursively keep going...

      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4))
    {
      //! Nice! The URL we found is in absolute path.
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    } else {
      //! We find a URL. HTML is a terrible standard. So there are many ways to present a URL.
      if (p1[0] == '.') {
        //! Some URLs are like <a href="../../../a.txt"> I cannot handle this.
	// again...probably good to recursively keep going..
	// NEW

        return GetNextURL(html,urlofthispage,result,pos+1);
	// /NEW
      }
      if (p1[0] == '/') {
        //! this means the URL is the absolute path
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = 0;
        strncat(result, p1, (p2 - p1));
        return (int)(p2 - html + 1);
      } else {
        //! the URL is a absolute path.
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
          //! urlofthis page is like http://www.abc.com/
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          //! urlofthis page is like http://www.abc.com/~xyz
          //! or http://www.abc.com
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
//PRINT
void print(char **result)
{
	int i=0;
	for(i=0;i<100;i++)
	{
		printf("%s\n",result[i]);
	}
}

//duplicate
void collect_next_url(char *read_file,int file_size,char *url,int dep)
{
	int i,p=0,k;
	char **result=NULL;
	result=(char**)calloc(51,sizeof(char*));
	char *temp;
	result[0]=url;
	for(i=1;i<51&&p<file_size;)
	{
		temp=(char*)calloc(1,150);
		p=GetNextURL(read_file,url,temp,p);
		int flag;
		flag=0;
		for(k=i-1;k>=1;k--)
		{
			if(strcmp(temp,result[k])==0)
			{
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			result[i]=temp;
			i++;
		}
		else
		free(temp);

	}
	insert_in_linklist(result,dep);
}

//EXTRACT file
void extract_file(char *file_name,char *url,int dep)
{
	struct stat st; //variable which will count length of file.
	stat(file_name,&st); // temp.txt is the file where wget fetch the html
	int file_size=st.st_size;
	char *read_file = (char*)calloc(1,file_size);
	FILE *new2=fopen(file_name,"r");
	char ch;
	int i=0;
	while((ch=fgetc(new2))!=EOF)
	{
			read_file[i++]=ch;
	}
	read_file[i]='\0';
	//printf("%s",read_file);//to check whether file's content is in char array or not
	collect_next_url(read_file,file_size,url, dep);//char array passed in which files content is inserted
}
//NEW FILE
void create_permanent_file(int dep,char *url)
{

      static int index=1;
	  char url_info[25]={0};
      strcpy(url_info,url);
      char indexarr[5]={0};
      sprintf(indexarr,"%d",index);
      char file_name[100]={0};
      strcat(file_name,"/home/rajat/search_engine/temp/");
      strcat(file_name,indexarr);
      strcat(file_name,".txt");
      FILE *temp = fopen("/home/rajat/search_engine/temp.txt","r");
      FILE *new = fopen(file_name,"w");
      fprintf(new,"%s\n",url);
      char p;
      while((p=fgetc(temp))!=EOF)
      {
        putc(p,new);
      }
  fclose(temp);
  index++;
	printf("\nYESSSSSSSS\n");
	extract_file(file_name,url,dep);//to extract content of 1.txt file in char array so as to pass in getnexturl function

}
//GETPAGE FUNC
void getpage(char *url,char *dir)
{
  char urlbuffer[400]={0};
  strcat(urlbuffer,"wget -O ");
  strcat(urlbuffer,dir);
  strcat(urlbuffer,"/temp.txt ");
  strcat(urlbuffer,url);
  system(urlbuffer);

}

node* fetch_url(node *ptr,int depth)
{

	while(ptr!=NULL)
	{
		if(ptr->node_depth==depth&&ptr->node_flag==0)
		{
			ptr->node_flag=1;
			return ptr;
		}
		ptr=ptr->next;
	}
	return NULL;
}
//MAIN
int main(int argc , char *argv[])
{
    if(argc!=4)
    printf("\nNo Extra Command Line Argument Passed Other Than Program Name\n");
    else
    {
        int result_check_url=check_url(argv[1]);
          if(result_check_url !=1)
          {
            printf("\nENTER THE CORRECT URL\n");
          }
          else
          {
                    int result_check_depth =check_depth(argv[2]);
                    testDir(argv[3]);
          }

						int dep=atoi(argv[2]);
						null_func();
						getpage(argv[1],argv[3]);
						create_permanent_file(1,argv[1]);
						int depth=1;
						int entered_depth=atoi(argv[2]);
						depth=depth+1;
						while(depth<=entered_depth)
						{
							printf("\n IN PARENT LOOP \n");
								node *crawl=head;
								while(crawl!=NULL)
								{
									printf("\n IN CRAWL \n");
									node *link;
									link=fetch_url(crawl,depth);
									if(link!=NULL)
									{

										if(check_url(link->node_url))
										{
											getpage(link->node_url,argv[3]);
											create_permanent_file(depth,link->node_url);
											printf("\n out if \n");
										}
										crawl=head;
									}
									else
									{
										break;
									}
								}
								printf("\n OUT PARENT LOOP \n");
								depth=depth+1;
						}
						print_linked_list();

    }
}
