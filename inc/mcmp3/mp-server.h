#ifndef _MCMP3_SERVER_H_
#define _MCMP3_SERVER_H_

#include <mcmp3/mp-types.h>
#include <mcmp3/mp-log.h>
#include <bass.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

namespace mp {
	class Server {
		public:
			Server();
			bool init(char*, int);
			int getNewSocket() const;
			mp_signal_t* getSignalPointer();
			mp::Log* getLogger();
			HSAMPLE* getMusicSample();
			HCHANNEL* getMusicChannel();

			char* getCurrentSongName();
			void run();
			~Server();
		private:
			// logger ==> send messages about events
			mp::Log m_logger;

			int m_socket_desc;
			int m_client_socket;
			int m_sockaddr_size;
			int *m_new_socket;
			char* m_ip;
			int m_port;
			struct sockaddr_in m_server;
			struct sockaddr_in m_client; 
			mp_signal_t m_signal;
			HSAMPLE m_sample;
			HCHANNEL m_channel;

			char* m_song_name;
	};
}

#endif