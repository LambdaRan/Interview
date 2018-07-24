#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

static void PrintfPwd(const passwd* pwdInfo)
{
     if(!pwdInfo)
          return;
     printf("name: \t%s\n", pwdInfo->pw_name);
     printf("passwd: \t%s\n", pwdInfo->pw_passwd);
     printf("uid: \t%d\n", pwdInfo->pw_uid);
     printf("gid: \t%d\n", pwdInfo->pw_gid);
     printf("gecos: \t%s\n", pwdInfo->pw_gecos);
     printf("dir: \t%s\n", pwdInfo->pw_dir);
     printf("shell: \t\%s\n", pwdInfo->pw_shell);
}


int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

     uid_t uid = getuid();

     long len = sysconf(_SC_GETPW_R_SIZE_MAX);
     printf("_SC_GETPW_R_SIZE_MAX:%ld\n", len);
     if(len == -1)
          len = 16384;

     char* buffer = new char[len];
     if(!buffer)
     {
          printf("buffer create failed. error[%d]\n", errno);
          return 1;
     }

     struct passwd pwd;
     struct passwd* result = NULL;

     int ret = getpwuid_r(uid, &pwd, buffer, len, &result);
     if(!result)
     {
          printf("getpwuid failed. ret[%d]\n", ret);
          return 1;
     }

     printf("----------pwd---------- \n");
     PrintfPwd(&pwd);

     printf("\n\n----------result----------\n");
     PrintfPwd(result);

     printf("\n\n----------buffer----------\n");
     printf("buffer: %s\n", buffer);
    //printf("buffer: %s\n", buffer+strlen(buffer)+1);
    for (int i = 0; i < 256; ++i)
    {
        printf("%c", buffer[i]);
    }
    printf("strlen(buffer):%ld\n", strlen(buffer));
    printf("&pwd: %p\n", &pwd);
    printf("&(*result):%p\n", &(*result));
     return 0;
}