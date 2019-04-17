  #include<stdio.h>
  #include<stdlib.h>
  #include<string.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  FILE *file;

  struct WordCount{		// structure for filename and count of word in the file
    char filename[200];
    int count;
    struct WordCount *next;
  };

  struct WordNode{		// structure for word to store in linklist
    char word[200];
    int count;
    struct WordNode *next;
    struct WordCount *start;
  };

  struct buffer{			// buffer array to maintain indexes (In hashing key is used as first index of word)
    struct WordNode *start;	// key are stored as a,b,c,d etc
  }Buffer[123];


  struct WordCount *createWordCount(char *file){	//function will create a new countnode to store filename and count
    struct WordCount *q = (struct WordCount*)malloc(sizeof(struct WordCount));
    strcpy(q->filename,file);
    q->count = 1;
    q->next = NULL;
    return q;
  }

  struct WordNode *createWordNode(char *arr){	//function will create a new wordnode to store word
    struct WordNode *ptr = (struct WordNode*)malloc(sizeof(struct WordNode));
    strcpy(ptr->word,arr);
    ptr->count=1;
    ptr->next = NULL;
    return ptr;
  }

  void addtoList(char *arr, char* file){		//function will add words to linklist
    struct WordNode *ptr = Buffer[arr[0]].start;
    if(ptr==NULL){
      ptr = createWordNode(arr);		// create new wordnode
      ptr->start=createWordCount(file);	// create new wordcount node
      Buffer[arr[0]].start = ptr;


    }else{
      while(1){
        if(!strcmp(ptr->word,arr)){	//if word found if will execute
          (ptr->count)++;
          struct WordCount *p = ptr->start;
          while(1){
            if(!strcmp(file,p->filename)){	//traverse wordcount to find filename
              p->count++;

              return;
            }
            if(p->next==NULL){
              break;
            }
            p=p->next;
          }
          p->next = createWordCount(file);	//create new wordcount node

          return;
        }
        if(ptr->next==NULL){
          break;
        }
        ptr=ptr->next;
      }
      struct WordNode *p1 = createWordNode(arr);		//if word is no found will create new word node
      p1->start = createWordCount(file);
      ptr->next = p1;

    }
  }

  char* convertDataInStr(char *fileName)   // function to shift whole data to string
  {
      struct stat st;
      stat(fileName, &st);
      int fileSize = st.st_size, i = 0;	// inbuild function to find filesize
      char *fileContent = (char*)malloc(sizeof(char) * fileSize), ch;
      file = fopen(fileName, "r");
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

  void word_count(char *str,char *filename) //function take whole string as argument n convert it in words
  {
    int i=0,flag=0,j=0;
    char arr[50];
    while(str[i]!='\0'){
      if(str[i]==' '){
        if(flag==1){
          arr[j]='\0';
          addtoList(arr,filename);	//function add the word to the linklist
          j=0;
          flag=0;
        }
      }
      else{
        arr[j++]=str[i];
        if(flag==0){
          flag=1;
        }
      }
      i++;
   }
  }

  void find_Title(char *fileData,char *filename,char *url)  // function to find title
  {
      char *titleStart, *titleEnd, *subString;
      int noOfChars;

      for(int i = 0;i<strlen(fileData);i++) {
          if(fileData[i] == '<' && fileData[i+1] == 't') {
              titleStart = &fileData[i];
              titleStart = strpbrk(titleStart, ">");	//strpbrk will return the position of >
              titleStart++;
              break;
          }
      }
      titleEnd = strpbrk(titleStart, "<");
      noOfChars = (titleEnd-titleStart);
      subString = (char*)malloc(sizeof(char) * noOfChars);
      memcpy(subString, titleStart, noOfChars);
      *(subString + noOfChars) = '\0';
      word_count(subString,url);		// calling wordcount to seprate string in words
      free(subString);
  }

  void find_Meta(char *fileData,char *filename,char *url) // function to find meta tag
  {
  	char *metaStart, *metaEnd, *subString;
    int noOfChars;
  	for(int i = 0;i<strlen(fileData);i++) {
          	if(fileData[i] == '<' && fileData[i+1] == 'm') {
                  metaStart = (strpbrk(&fileData[i], "\"\'") + 1);	//strpbrk will return the position of /
                  if(metaStart) {
                       metaEnd = strpbrk(metaStart, "/>");
                       if(*metaStart == 'd' || *metaStart == 'a' || *metaStart == 'k') {
                          metaStart = strpbrk(metaStart, "=");	// jump to position of =
                          metaStart += 2;
                          noOfChars = (metaEnd-metaStart);
                          subString = (char*)malloc(sizeof(char) * noOfChars);
                          memcpy(subString, metaStart, noOfChars);
                          *(subString + noOfChars) = '\0';
                          //printf("%s\n\n",subString);
                          word_count(subString,url);		// calling wordcount to seprate string in words
                          free(subString);
                       }
                  }
          	}
      	}
  }

  void find_Heading(char *fileData,char *filename,char *url) // function to find all Heading
  {
  	char *headStart, *headEnd, *subStr;
  	int noOfChars;

  	for(int i = 0;i<strlen(fileData);i++) {
          	if(fileData[i] == '<' && fileData[i+1] == 'h') {
              	   headStart = &fileData[i];
  		   headStart = strpbrk(headStart, ">");		//strpbrk will return the position of >
             	   headStart++;
  		   headEnd = strpbrk(headStart, "<");
              noOfChars = (headEnd-headStart);		//noofChars will be the total length of string
              subStr = (char*)malloc(sizeof(char) * noOfChars);
              memcpy(subStr, headStart, noOfChars);      //memcpy will copy string starting from headStart till noOfChars
              *(subStr + noOfChars) = '\0';
              word_count(subStr,url);	       // calling wordcount to seprate string in words
              free(subStr);
          	}
      	}
  }

  void print()		//function to print the linklist
  {
    for(int i=65;i<123;i++){
      struct WordNode *ptr = Buffer[i].start;	// traverse wordnode (a,b,c,d---)
      if(ptr!=0){
        while(ptr->next!=NULL){
          // printf("%c %s %d\n",i,ptr->word,ptr->count);
          struct WordCount *p = ptr->start;	// traverse wordcount node to for page name and count
          while(p!=NULL){
            printf("%s %d %s ",ptr->word,p->count,p->filename);
            p=p->next;
          }
          ptr=ptr->next;
        }
      }
    }
  }

  void saveData()		// function to save data to file
  {
    FILE *f = fopen("indexerList.txt","a");  //create a new file to save word count
    for(int i=65;i<123;i++){
      struct WordNode *ptr = Buffer[i].start;  // traverse wordnode (a,b,c,d---)
      if(ptr!=0){
        while(ptr->next!=NULL){
          // printf("%c %s %d\n",i,ptr->word,ptr->count);
          struct WordCount *p = ptr->start;	// traverse wordcount node to for page name and count
          while(p!=NULL){
            fprintf(f,"%s %d %s ",ptr->word,p->count,p->filename);
            p=p->next;
          }
          ptr=ptr->next;
        }
      }
    }
  }

  char* find_url(char *filename)  //find url from the page
  {
    char *url = (char*)malloc(sizeof(char) * 200),ch;
    int i=0;
      file = fopen(filename, "r");
      ch = getc(file);
      while(ch!='<') {
        url[i++] = ch;
        ch = getc(file);
      }
      url[i] = '\0';
      return url;
  }

int main(int argc,char* argv[]) {

  char File_Name[10],ch,*url;
  static int file_no=1;
  for(int i=0;i<4;i++)
  {
    sprintf(File_Name,"%d",file_no);  // name5 of the file
    strcat(File_Name, ".txt"); // adding txt format

    struct stat st;
    stat(File_Name, &st);
    int fileSize = st.st_size;

    file_no++;                  // increase file no
    char *fileData = (char*)malloc(sizeof(char) * fileSize);
    char *url1 = (char*)malloc(sizeof(char) * 200);  // file size
    fileData = convertDataInStr(File_Name);  // convert data into string

    url1 = find_url(File_Name); //find url
    find_Title(fileData,File_Name,url1);  // search tile tag
    find_Meta(fileData,File_Name,url1);  // search the meta tag
    find_Heading(fileData,File_Name,url1);  // search heading tag

    print();			// print the link list
    saveData();			// save linlist in file*/
  }
}
