
// 参考 https://blog.csdn.net/analogous_love/article/details/53319815

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <sstream>
#include <iomanip> // setw setfill

#include <string.h>
#include <errno.h>
#include <time.h>

#include <pthread.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define WORKER_THREAD_NUM 5

int g_epollfd = 0;
bool g_bstop = false;
int g_listenfd = 0;
pthread_t g_acceptthreadid = 0;
pthread_t g_threadid[WORKER_THREAD_NUM] = {0};

pthread_cond_t g_acceptcond;
pthread_mutex_t g_acceptmutex;

pthread_cond_t g_cond;   //= PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_mutex; //= PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t g_clientmutex;

std::list<int> g_listClient;

void prog_exit(int signo)
{
    ::signal(SIGINT, SIG_IGN);
    //::signal(SIGKILL, SIG_IGN);//该信号不能被阻塞、处理或者忽略
    ::signal(SIGTERM, SIG_IGN);

    std::cout << "program recv signal " << signo << " to exit." << std::endl;
    g_bstop = true;

    ::epoll_ctl(g_epollfd, EPOLL_CTL_DEL, g_listenfd, NULL);
    ::shutdown(g_listenfd, SHUT_RDWR);
    ::close(g_listenfd);
    ::close(g_epollfd);

    ::pthread_cond_destroy(&g_acceptcond);
    ::pthread_mutex_destroy(&g_acceptmutex);

    ::pthread_cond_destroy(&g_cond);
    ::pthread_mutex_destroy(&g_mutex);

    ::pthread_mutex_destroy(&g_clientmutex);
}

bool create_server_listener(const char *ip, short port)
{
    g_listenfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (g_listenfd == -1)
        return false;
    int on = 1;
    ::setsockopt(g_listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    ::setsockopt(g_listenfd, SOL_SOCKET, SO_REUSEPORT, (char *)&on, sizeof(on));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    if (::bind(g_listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        return false;

    if (::listen(g_listenfd, 50) == -1)
        return false;

    g_epollfd = ::epoll_create(1);
    if (g_epollfd == -1)
        return false;
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = g_listenfd;
    if (::epoll_ctl(g_epollfd, EPOLL_CTL_ADD, g_listenfd, &event) == -1)
        return false;

    return true;
}

void release_client(int clientfd)
{
    if (::epoll_ctl(g_epollfd, EPOLL_CTL_DEL, clientfd, NULL) == -1)
        std::cout << "release client socket failed as call epoll_ctl failed" << std::endl;
    ::close(clientfd);
}

void *accept_thread_func(void *arg)
{   
    (void)arg;

    while (!g_bstop)
    {
        ::pthread_mutex_lock(&g_acceptmutex);
        ::pthread_cond_wait(&g_acceptcond, &g_acceptmutex);
        // ::pthread_mutex_unlock(&g_acceptmutex);

        struct sockaddr_in client_addr;
        socklen_t client_addrlength = 0;
        int newfd = ::accept(g_listenfd, (struct sockaddr *)&client_addr, &client_addrlength);
        ::pthread_mutex_unlock(&g_acceptmutex);

        if (newfd == -1)
            continue;
        std::cout << "new client connected: " << ::inet_ntoa(client_addr.sin_addr)
                  << " : " << ::ntohs(client_addr.sin_port) << std::endl;

        // 将新socket设置为non-blocking
        int old_option = ::fcntl(newfd, F_GETFL, 0);
        int new_option = old_option | O_NONBLOCK;
        if (::fcntl(newfd, F_SETFL, new_option) == -1)
        {
            std::cout << "fcntl error, old option = " << old_option
                      << ", new option = " << new_option << std::endl;
            continue;
        }

        struct epoll_event event;
        memset(&event, 0, sizeof(event));
        event.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
        event.data.fd = newfd;
        if (::epoll_ctl(g_epollfd, EPOLL_CTL_ADD, newfd, &event) == -1)
        {
            std::cout << "epoll_ctl error, fd = " << newfd << std::endl;
        }
    }

    return NULL;
}

void *worker_thread_func(void *arg)
{
    (void)arg;
    while (!g_bstop)
    {
        int clientfd;
        ::pthread_mutex_lock(&g_clientmutex);
        while (g_listClient.empty())
            ::pthread_cond_wait(&g_cond, &g_clientmutex);
        clientfd = g_listClient.front();
        g_listClient.pop_front();
        ::pthread_mutex_unlock(&g_clientmutex);

        std::cout << std::endl;

        std::string strClientmsg;
        char buff[256];
        bool bError = false;
        while (true)
        {
            memset(buff, 0, sizeof(buff));
            int nRecv = ::recv(clientfd, buff, sizeof(buff) - 1, 0);
            if (nRecv == -1)
            {
                if (errno == EWOULDBLOCK)
                    break;
                else
                {
                    std::cout << "recv error, client disconnected, fd = " << clientfd << std::endl;
                    release_client(clientfd);
                    bError = true;
                    break;
                }
            }
            else if (nRecv == 0)
            {
                std::cout << "peer closed, client disconnected, fd = " << clientfd << std::endl;
                release_client(clientfd);
                bError = true;
                break;
            }
            strClientmsg += buff;
        }

        if (bError)
            continue;
        std::cout << "client msg: " << strClientmsg;
        time_t now = time(NULL);
        struct tm *nowStr = localtime(&now);
        std::ostringstream ostimestr;
        ostimestr << "[" << nowStr->tm_year + 1900 << "-"
                  << std::setw(2) << std::setfill('0') << nowStr->tm_mon + 1 << "-"
                  << std::setw(2) << std::setfill('0') << nowStr->tm_mday << " "
                  << std::setw(2) << std::setfill('0') << nowStr->tm_hour << ":"
                  << std::setw(2) << std::setfill('0') << nowStr->tm_min << ":"
                  << std::setw(2) << std::setfill('0') << nowStr->tm_sec << "]server reply: ";
        strClientmsg.insert(0, ostimestr.str());

        while (true)
        {
            int nSent = ::send(clientfd, strClientmsg.c_str(), strClientmsg.size(), 0);
            if (nSent == -1)
            {
                if (errno == EWOULDBLOCK)
                {
                    ::sleep(10);
                    continue;
                }
                else
                {
                    std::cout << "send error, fd = " << clientfd << std::endl;
                    release_client(clientfd);
                    break;
                }
            }

            std::cout << "send: " << strClientmsg;
            strClientmsg.erase(0, nSent);

            if (strClientmsg.empty())
                break;
        }
    }
    return NULL;
}

void daemon_run()
{
    int pid;
    signal(SIGCHLD, SIG_IGN);
    //1）在父进程中，fork返回新创建子进程的进程ID；
    //2）在子进程中，fork返回0；
    //3）如果出现错误，fork返回一个负值；
    pid = fork();
    if (pid < 0)
    {
        std::cout << "fork error" << std::endl;
        exit(-1);
    }
    //父进程退出，子进程独立运行
    else if (pid > 0)
    {
        exit(0);
    }
    //之前parent和child运行在同一个session里,parent是会话（session）的领头进程,
    //parent进程作为会话的领头进程，如果exit结束执行的话，那么子进程会成为孤儿进程，并被init收养。
    //执行setsid()之后,child将重新获得一个新的会话(session)id。
    //这时parent退出之后,将不会影响到child了。
    setsid();
    int fd;
    fd = open("/dev/null", O_RDWR, 0);
    if (fd != -1)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
    }
    if (fd > 2)
        close(fd);
}

int main(int argc, char* argv[])
{  
    short port = 0;
    int ch;
    bool bdaemon = false;
    while ((ch = getopt(argc, argv, "p:d")) != -1)
    {
        switch (ch)
        {
        case 'd':
            bdaemon = true;
            break;
        case 'p':
            port = atol(optarg);
            break;
        }
    }
 
    if (bdaemon)
        daemon_run();
 
 
    if (port == 0)
        port = 12345;
     
    if (!create_server_listener("0.0.0.0", port))
    {
        std::cout << "Unable to create listen server: ip=0.0.0.0, port=" << port << "." << std::endl;
        return -1;
    }
 
    
    //设置信号处理
    signal(SIGCHLD, SIG_DFL);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, prog_exit);
    //signal(SIGKILL, prog_exit);//该信号不能被阻塞、处理或者忽略
 
    signal(SIGTERM, prog_exit);
 
    ::pthread_cond_init(&g_acceptcond, NULL);
    ::pthread_mutex_init(&g_acceptmutex, NULL);
 
    ::pthread_cond_init(&g_cond, NULL);
    ::pthread_mutex_init(&g_mutex, NULL);
 
    ::pthread_mutex_init(&g_clientmutex, NULL);
     
    ::pthread_create(&g_acceptthreadid, NULL, accept_thread_func, NULL);
    //启动工作线程
    for (int i = 0; i < WORKER_THREAD_NUM; ++i)
    {
        ::pthread_create(&g_threadid[i], NULL, worker_thread_func, NULL);
    }
 
    while (!g_bstop)
    {       
        struct epoll_event ev[1024];
        int n = ::epoll_wait(g_epollfd, ev, 1024, 10);
        if (n == 0)
            continue;
        else if (n < 0)
        {
            std::cout << "epoll_wait error" << std::endl;
            continue;
        }
 
        int m = std::min(n, 1024);
        for (int i = 0; i < m; ++i)
        {
            //通知接收连接线程接收新连接
            if (ev[i].data.fd == g_listenfd)
                pthread_cond_signal(&g_acceptcond);
            //通知普通工作线程接收数据
            else
            {               
                pthread_mutex_lock(&g_clientmutex);              
                g_listClient.push_back(ev[i].data.fd);
                pthread_mutex_unlock(&g_clientmutex);
                pthread_cond_signal(&g_cond);
                //std::cout << "signal" << std::endl;
            }
        }
    }
    
    return 0;
}
