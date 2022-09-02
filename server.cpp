#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <thread>
#include <signal.h>
#include <string>
#include <arpa/inet.h>

using namespace std;

const int PORT = 8080;
const int QUEUE_SIZE = 20;
int s;

void handleConn(int conn, sockaddr_in addr)
{
    cout << "handle connection: " << conn << endl;
    cout << "client from: " << inet_ntoa(addr.sin_addr) << ":" << addr.sin_port << endl;
    
    char buff[4096];
    string demo = "ACK";
    while (true)
    {
        // 接受信息
        int n = recv(conn, buff, sizeof(buff), 0);
        if (n == -1)
        {
            close(conn);
            cerr << "recv data err" << endl;
            return;
        }
        if (n == 0)
        {
            break;
        }
        string str = buff;
        string res = demo + str;
        cout<<res<<endl;
        for (int i = 0; i < res.length(); i++)
        {
			buff[i] = res[i];
		}
        buff[res.length()] = '\0';
        if (send(conn,buff,sizeof(res),0) == -1 )
        {
            perror("send data err");
        }
    }
    close(conn);
}

void handleSign(int sig)
{
    cout << "receive sig: " << sig;
    cout << " server exists" << endl;
    exit(EXIT_SUCCESS);
    close(s);
}

int main(int argc, char** argv)
{
    signal(SIGINT, handleSign);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
    {
        cout << "create socket err" << endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (sockaddr *)&addr, sizeof(addr)) == -1)
    {
        cout << "bind socket err" << endl;
        exit(EXIT_FAILURE);
    }

    if (listen(s, QUEUE_SIZE) == -1)
    {
        cout << "listen socket err" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Listen on " <<inet_ntoa(addr.sin_addr)<< ":" << PORT << endl;

    while (true)
    {
        sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        int conn = accept(s, (sockaddr *)&client_addr, &length);
        handleConn(conn,addr);
    }

    return 0;
}
