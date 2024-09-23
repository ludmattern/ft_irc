#include "Server.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    std::string password = argv[2];
    try
    {
        Server server(port, password);
       server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
