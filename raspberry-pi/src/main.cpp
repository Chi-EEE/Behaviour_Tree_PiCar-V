#include <iostream>
#include <thread>
#include "sockpp/tcp_acceptor.h"
#include "sockpp/version.h"

void run_echo(sockpp::tcp_socket sock)
{
	ssize_t n;
	char buf[512];

	while ((n = sock.read(buf, sizeof(buf))) > 0)
		sock.write_n(buf, n);

	std::cout << "Connection closed from " << sock.peer_address() << std::endl;
}

int main(int argc, char* argv[])
{
	std::cout << "Sample TCP echo server for 'sockpp' "
		<< sockpp::SOCKPP_VERSION << '\n' << std::endl;

	in_port_t port = (argc > 1) ? atoi(argv[1]) : 12345;

	sockpp::initialize();

	sockpp::tcp_acceptor acc(port);

	if (!acc) {
		std::cerr << "Error creating the acceptor: " << acc.last_error_str() << std::endl;
		return 1;
	}
	std::cout << "Awaiting connections on port " << port << "..." << std::endl;

	while (true) {
		sockpp::inet_address peer;

		// Accept a new client connection
		sockpp::tcp_socket sock = acc.accept(&peer);
		std::cout << "Received a connection request from " << peer << std::endl;

		if (!sock) {
			std::cerr << "Error accepting incoming connection: " 
				<< acc.last_error_str() << std::endl;
		}
		else {
			// Create a thread and transfer the new stream to it.
			std::thread thr(run_echo, std::move(sock));
			thr.detach();
		}
	}

	return 0;
}



