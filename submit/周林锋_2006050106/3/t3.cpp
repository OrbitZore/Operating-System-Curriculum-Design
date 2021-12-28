#include <bits/stdc++.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
int MSGKEY;
struct msgT
{
  long mtype;
  char mtext[8];
};
int msgqid, i;
std::atomic<int> cnt{0};
void
CLIENT ()
{
  int i;
  msgqid = msgget (MSGKEY, 0777);
  for (i = 10; i >= 1; i--)
    {
      msgT msg;
      msg.mtype = i;
      std::cin>>msg.mtext;
      printf ("%d (client) sent \n",++cnt);
      msgsnd (msgqid, &msg, strlen(msg.mtext), 0);
      if (i!=10)  sleep(1);
    }
  exit (0);
}
void
SERVER ()
{
  msgqid = msgget (MSGKEY, 0777 | IPC_CREAT);
  msgT msg;
  do
    {
      msg.mtext[msgrcv (msgqid, &msg, 8, 0, 0)]=0;
      printf ("%d (Server) recieved %s\n",++cnt,msg.mtext);
    }
  while (msg.mtype != 1);
  msgctl (msgqid, IPC_RMID, 0);
  exit (0);
}
int
main ()
{
    MSGKEY=214;
  while ((i = fork ()) == -1)
    ;
  if (!i)
    SERVER ();
  while ((i = fork ()) == -1)
    ;
  if (!i)
    CLIENT ();
  wait (0);
  wait (0);
}
