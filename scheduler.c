#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
struct
{
  char username[20];
  char password[20];
}user[100];
struct
{
  char message[2000];
  char cdate[30];
  char duedate[30];
}note[100]; 
int usercount; //total users
int currentuser; // logged in user
int notecount; //total notes
static int login_status=0;
void login();
void signup();
void retrieve();
void retrieveNote();
void save();
void saveNote();
void showcal();
void viewNote();
void addNote();
void editNote();
void delNote();

void main()
{
  int choice;
  retrieve();  // retrieve all data from files 
  printf("\
    ******************************\n\
    SCHEDULER PROGRRAM\n\
    ******************************\n");
  printf("\
  ***MENU***\n\
  1.login\n\
  2.signup\n");
  scanf("%d",&choice);
  while(login_status!=1)
    switch(choice)
    {
      case 1:
        login();
        retrieveNote(); //retrieve user notes
        break;
      case 2:
        signup();
        break;
      default:
        printf("Exiting...\n");
        exit(0);
    }
  printf("\
  ****************************\n\
  WELCOME %s\n\
  ****************************\n",user[currentuser].username);
  showcal("cal");
  while(1)
  {
    printf("\
    **************\n\
    OPTIONS:\n\
    1. View all notes\n\
    2. View first N notes\n\
    3. Add note\n\
    4. Edit note\n\
    5. Delete note\n\
    **************\n");
    scanf("%d",&choice);
    switch(choice)
    {
      case 1://view all notes
        viewNote(0);
        break;
      case 2://view N no. of notes
        printf("enter N:");
        scanf("%d",&choice); //using choice integer var
        viewNote(choice);
        break;
      case 3://add note
        addNote();
        break;
      case 4: //edit note
        editNote();
        break;
      case 5://delete note
        delNote();
        break;
      default:
        saveNote();
        printf("Exiting...\n");
        exit(0);
    }
  }
}

void login()
{
  char u[20],p[20]; //entered by user.
  int found=-1;
  if(usercount==0)
  {
    printf("no users yet.\n");
    exit(1);
  }
  printf("username:");
  scanf("%s",&u);
  for(int i=0;i<usercount;i++) //find user
    if(strcmp(u,user[i].username)==0)
    {
      found=i; //if found get position
      break;
    }  
  if(found==-1) //if not found then exit
  {
    printf("incorrect username.\n");
    return;
  }
  printf("password:");
  scanf("%s",&p);
  if(strcmp(p,user[found].password)==0)
  {
    printf("user logged in.\n");
    currentuser=found;
    login_status=1;
    return;
  }
  else
  {
    printf("incorrect password.\n");
    return;
  }
}
void signup()
{
  printf("username:");
  scanf("%s",&user[usercount].username);
  printf("password:");
  scanf("%s",&user[usercount].password);
  for(int i=0;i<usercount;i++) //find user
    if(strcmp(user[usercount].username,user[i].username)==0)
    {
      printf("user already exist.\n");
      return;
    }  
  usercount++;
  save(); //saving users in file
  printf("user successfully created. please login again.\n");
  printf("exiting...\n");
  exit(0);
}
void retrieve()
{
  //retrieve users file
  FILE *fp=fopen("users","r");
  if(fp==NULL)
    return;
  fscanf(fp,"%d",&usercount); //retrieve usercount
  for(int i=0;i<usercount;i++) //retrieve username and password
    fscanf(fp,"%s%s",&user[i].username,&user[i].password);
  fclose(fp);
}
void retrieveNote()
{
   //retrieve notes
  FILE *fp=fopen(user[currentuser].username,"r");
  if(fp==NULL)
    return;
  fscanf(fp,"%d",&notecount); //retrieve notecount
  while(getc(fp)!='\n');//clearing \n
  for(int i=0;i<notecount;i++)
  {
    fscanf(fp,"%[^\n]%*c",note[i].cdate);
    fscanf(fp,"%[^\n]%*c",note[i].duedate);
    fscanf(fp,"%[^\n]%*c",note[i].message);
  }
  fclose(fp);
}
void save()
{
  //save users file
  FILE *fp=fopen("users","w+");
  fprintf(fp,"%d\n",usercount); //saving usercount
  for(int i=0;i<usercount;i++) //saving username and passwords
    fprintf(fp,"%s %s\n",user[i].username,user[i].password);
  printf("users saved.\n");
  fclose(fp);
}
void saveNote()
{
  //save notes
  FILE *fp=fopen(user[currentuser].username,"w+");
  fprintf(fp,"%d\n",notecount); //saving notecount
  for(int i=0;i<notecount;i++)
  {
    fprintf(fp,"%s\n",note[i].cdate);
    fprintf(fp,"%s\n",note[i].duedate);
    fprintf(fp,"%s\n",note[i].message);
  }
  printf("notes saved.\n");
  fclose(fp);
}
void showcal(char cmd[])
{
  FILE * f = popen(cmd, "r" );
  char *p=NULL;
  char c;
  int i=0;
  if ( f == 0 )
    fprintf( stderr, "Could not print calender\n" );
  const int BUFSIZE = 1000;
  char buf[ BUFSIZE ];
  if(strcmp(cmd,"cal")==0)
    printf("____________________\n");
  else
    printf("____________________\
  ____________________\
  ____________________\n");  
  while( fgets(buf, BUFSIZE, f) ) 
  {
    if(i%2==0)
      printf("\033[0;32m");
    if(i%2==1)
      printf("\033[0;0m");
    printf("%s",buf);
    i++;
  }
  if(strcmp(cmd,"cal")==0)
    printf("____________________\n");
  else
    printf("____________________\
  ____________________\
  ____________________\n");  
  pclose(f);
}
void viewNote(int n)
{
  printf("\033[0;32m");
  if(notecount==0)
  {
    printf("all clear!! (no notes avaialble).\n");
    printf("\033[0;0m");
    return;
  }
  if(n==0 || n>notecount)
    n=notecount;
  printf("----------------------------------------------\n");
  printf("%25s\n","NOTES");
  printf("----------------------------------------------\n");
  for(int i=0;i<n;i++)
    printf("NOTE NUMBER: %d\nDATE OF NOTE: %s\nDUE DATE: %s\nMESSAGE: %s\n\n\n", i+1, note[i].cdate, note[i].duedate, note[i].message);
  printf("----------------------------------------------\n");
  printf("\033[0;0m");
  showcal("cal -A 2");
  printf("***Enter 'q' to go to menu***\n");
  for(char c='\0';c!='q';)
    c=getchar();
}
void addNote()
{
  if(notecount>=100)
  {
    printf("no space left. delete old notes to add more.\n");
    return;
  }
  showcal("cal -A 2");
  time_t curtime;
  time(&curtime); //set time object
  strcpy(note[notecount].cdate,ctime(&curtime)); //store current date
  for(int i=0,j=0;note[notecount].cdate[i]!='\0';i++,j++)
  {
    if(note[notecount].cdate[j]=='\n')
      j++;
    note[notecount].cdate[i]=note[notecount].cdate[j];
  }
  while(getchar()!='\n'); //clearing inputstream
  printf("enter note:");
  scanf("%[^\n]%*c",&note[notecount].message);
  printf("enter due date:");
  scanf("%[^\n]%*c",&note[notecount].duedate);
  printf("note has been added.\n");
  notecount++;
}
void editNote()
{
  int notenum;
  time_t curtime;
  printf("enter note number:");
  scanf("%d",&notenum);
  if(notenum==0)
    return;
  if(notenum>notecount)
  {
    printf("invalid note number.\n");
    return;
  }
  notenum--; // to get index of note.
  time(&curtime); //set time object
  strcpy(note[notenum].cdate,ctime(&curtime)); //store current date
  for(int i=0,j=0;note[notenum].cdate[i]!='\0';i++,j++)
  {
    if(note[notenum].cdate[j]=='\n')
      j++;
    note[notenum].cdate[i]=note[notenum].cdate[j];
  }
  while(getchar()!='\n'); //clearing inputstream
  printf("enter note:");
  scanf("%[^\n]%*c",&note[notenum].message);
  printf("enter due date:");
  scanf("%[^\n]%*c",&note[notenum].duedate);
  printf("note has been updated.\n");
}
void delNote()
{
  int num;
  printf("enter note number:");
  scanf("%d",&num);
  if(num==0)
    return;
  if(num>notecount)
  {
    printf("enter valid note number.\n");
    return;
  }
  num--; // note 1 is stored is note 0
  for(int i=num;i<notecount-1;i++)
  {
    strcpy(note[i].message,note[i+1].message);
    strcpy(note[i].cdate,note[i+1].cdate);
    strcpy(note[i].duedate,note[i+1].duedate);
  }
  notecount--; //decrement note counter
  printf("note number %d has been deleted.\n",num+1);
}