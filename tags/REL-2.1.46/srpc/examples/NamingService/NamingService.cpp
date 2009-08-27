// NameService.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "NamingServiceServer.h"
#include "NamingServiceClient.h"
#include "NamingServiceNetwork.h"
#include <ctime>

using namespace srpc;
using namespace ns;

void rpcing(NamingServiceClient& clientService)
{
    clientService.bind("alias", "kcando");
    assert(clientService.getName() == "alias");
    assert(clientService.getValue() == "kcando");
    assert(clientService.isSucceeded() == true);

    clientService.resolve("alias");
    assert(clientService.getName() == "alias");
    assert(clientService.getValue() == "kcando");
    assert(clientService.isSucceeded() == true);

    clientService.bind("name", "Kim ByeongSu");
    assert(clientService.getName() == "name");
    assert(clientService.getValue() == "Kim ByeongSu");
    assert(clientService.isSucceeded() == true);

    clientService.resolve("name");
    assert(clientService.getName() == "name");
    assert(clientService.getValue() == "Kim ByeongSu");
    assert(clientService.isSucceeded() == true);

    clientService.rebind("alias", "kcandoall");
    assert(clientService.getName() == "alias");
    assert(clientService.getValue() == "kcandoall");
    assert(clientService.isSucceeded() == true);

    clientService.resolve("alias");
    assert(clientService.getName() == "alias");
    assert(clientService.getValue() == "kcandoall");
    assert(clientService.isSucceeded() == true);

    clientService.unbind("alias");
    assert(clientService.getName() == "alias");
    assert(clientService.isSucceeded() == true);

    clientService.resolve("alias");
    assert(clientService.isSucceeded() == false);

    clientService.unbind("alias");
    clientService.unbind("name");
}


void giveAndTake(bool useBitStream)
{
    /// 네트웍 초기화
    NamingServiceNetwork clientNetwork(useBitStream);
    NamingServiceNetwork serverNetwork(useBitStream);
    serverNetwork.setOppositeNetwork(&clientNetwork);
    clientNetwork.setOppositeNetwork(&serverNetwork);
    
    /// 서버 서비스 초기화
    boost::scoped_ptr<NamingService> serverService(
        new NamingServiceServer(&serverNetwork)); 

    /// 클라이언트 서비스 초기화
    boost::scoped_ptr<NamingService> clientService(
        new NamingServiceClient(&clientNetwork));

    const clock_t start = clock();
    for (int i = 0; i < 10000; ++i) {
        rpcing(static_cast<NamingServiceClient&>(*clientService));
    }
    const Float32 elapsed = Float32(clock() - start) / CLOCKS_PER_SEC;

    std::cout << (useBitStream ? "Bit" : "Byte") <<
        " Stream, Client(sent/received): " <<
        static_cast<int>(clientNetwork.getSentBytes()) << "/" <<
        static_cast<int>(clientNetwork.getReceivedBytes()) <<
        " bytes, Server(sent/received): " <<
        static_cast<int>(serverNetwork.getSentBytes()) << "/" <<
        static_cast<int>(serverNetwork.getReceivedBytes()) <<
        " bytes, Elapsed: " << elapsed << " seconds.\n";
}


int main(int /*argc*/, char* /*argv*/[])
{
    giveAndTake(true);
    giveAndTake(false);

    return 0;
}
