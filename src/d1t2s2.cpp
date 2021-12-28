#include <sys/wait.h>
#include <bits/stdc++.h>
using namespace std;
int main (){
  pid_t pid; /* fork a child process */
  pid = fork ();
  if (pid < 0)
    { /* error occurred */
      fprintf (stderr, "Fork Failed");
      return 1;
    }
  else if (pid == 0)
    { /* 子进程 */
      execlp ("/bin/sleep", "fuck", "1",NULL);
    }
  else
    { /* 父进程 */ /* 父进程将一直等待，直到子进程运行完毕*/
      wait (NULL);
      printf ("Child Complete");
    }
  return 0;
}
