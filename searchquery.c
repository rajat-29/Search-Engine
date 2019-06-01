#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
FILE *file;

struct LinkedList {
  int count;
  char link_word[200];
  char link_url[200];
  struct LinkedList *next;
};

typedef struct LinkedList *node;
struct LinkedList *head = NULL;

node createNode() {
  node temp1;
  temp1 = (node)calloc(1,sizeof(struct LinkedList));
  temp1->next = NULL;
  return temp1;
}


void putinList(int word_count,char *link,char *search_word)
{
  node temp,p,p1;
  if(head == NULL) {
    temp = createNode();
    temp->count = word_count;
    strcpy(temp->link_word,search_word);
    strcpy(temp->link_url,link);
    head = temp;
  }
  else {
    p = head;
    p1 = head;
    while(p->next != NULL)
    {
      p = p->next;
      if(!strcmp(p->link_url,link))
      {
          return;
      }
    }
    while(p1->next != NULL)
    {

      if(word_count > p1->count)
      {
        temp = createNode();
        temp->count = word_count;
        strcpy(temp->link_word,search_word);
        strcpy(temp->link_url,link);
        temp->next = p1->next;
        p1->next=temp;
        return;
      }
p1 = p1->next;
    }
    temp = createNode();
    temp->count = word_count;
    strcpy(temp->link_word,search_word);
    strcpy(temp->link_url,link);
    p1->next=temp;
}
}

void searching_file(char *word,char *filedata)
{
  char *Start, *End, *subString, *link;
  int noOfChars,x;
  for(int i = 0;i<strlen(filedata);i++)
  {
    if(filedata[i] == word[0] && filedata[i+1] == word[1] )
    {
      Start = &filedata[i+2];
      Start = strpbrk(Start, " ");
      Start++;

      End = strpbrk(Start, " ");
      noOfChars = (End-Start);
      subString = (char*)calloc(sizeof(char) , noOfChars);
      memcpy(subString,Start, noOfChars);
      *(subString + noOfChars) = '\0';

      Start = End;
      End = strpbrk(Start, "\n");
      noOfChars = (End-Start);
      link = (char*)calloc(sizeof(char) , noOfChars);
      memcpy(link,Start, noOfChars);
      *(link + noOfChars) = '\0';

      sscanf(subString, "%d", &x);

      putinList(x,link,word);
      free(subString);
      free(link);
    }
  }

}

void single_word(char input[],int start,int end,char *filedata)
{
  char *search = (char*)calloc(sizeof(char),50);
  int j=0;
  for(int i=start;i<=end;i++)
  {
    search[j++] = input[i];
  }
  search[j]='\0';
  searching_file(search,filedata);
}

void seprate_word(char input[],char *filedata)
{
  int i=0,j=0;
  while(input[i]!='\0')
  {
    if(input[i]==' ')
    {
      i--;
      single_word(input,j,i,filedata);
      i++;
      j=i+1;
    }
    i++;
  }
single_word(input,j,i,filedata);
}

char* convertDataInStr()   // function to shift whole data to string
{
    struct stat st;
    stat("indexerList.txt", &st);
    int fileSize = st.st_size, i = 0;	// inbuild function to find filesize
    char *fileContent = (char*)calloc(sizeof(char) , fileSize), ch;
    file = fopen("indexerList.txt", "r");
    ch = getc(file);
    while(ch != EOF) {		// shifting data from file to string
        fileContent[i] = ch;
        ch = getc(file);
        i++;
    }
    fileContent[i] = '\0';
    fclose(file);
    return fileContent;
}

void print()
{
    node temp;
    temp = head;
    while(temp!=NULL)
    {
      printf("%s %s\n",temp->link_word,temp->link_url);
      temp=temp->next;
    }
}

int main()
{
  struct stat st;
  stat("indexerList.txt", &st);
  int fileSize = st.st_size;
  char input[50];
  char *fileData = (char*)calloc(sizeof(char) , fileSize);
  fileData = convertDataInStr();
 printf("ENTER INPUT = ");
fgets(input,50,stdin);
 seprate_word(input,fileData);
  print();
  free(fileData);
}
