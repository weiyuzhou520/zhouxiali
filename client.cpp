#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

const int PORT = 8080;
const int MAX_LINE = 2048;
int main()
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
    {
        cout << "create socket err" << endl;
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connection");
        return -1;
    }

    char buff[2048];
    cout<<"please input str:"<<endl;
    while(true) {
        cin>>buff;
        if (send(s, buff, sizeof(buff), 0) == -1)
        {
            perror("send data err");
            return -1;
        }
        int n = recv(s, buff, sizeof(buff), 0);
        cout<<buff<<endl;
    }


    cout << "send msg " << buff << " successfully" << endl;

    close(s);

    return 0;
}
