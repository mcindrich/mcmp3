#include <mcmp3/mp-server.h>
#include <cstring>
#include <iostream>
#include <thread>
using namespace std;

namespace mp {
    static void inline parse_message(mp::Server* server, mp_signal_t signal, 
                                    char* msg) {
        mp_signal_t* sig = server->getSignalPointer();
        HCHANNEL* channel = server->getMusicChannel();
        HSAMPLE* sample = server->getMusicSample();

        switch(signal) {
            case mp_signal_t_none:
                break;
            case mp_signal_t_stop:
                if(*sig != mp_signal_t_none && *sig != mp_signal_t_stop) {
                    *sig = mp_signal_t_stop;
                    BASS_ChannelStop(*channel);
                }
                break;
            case mp_signal_t_change_song: // msg needed
                BASS_ChannelStop(*channel);
                *sample = BASS_SampleLoad(0, msg, 0, 0, 1, BASS_SAMPLE_MONO);
                *channel = BASS_SampleGetChannel(*sample, FALSE); 
                BASS_ChannelPlay(*channel, FALSE); 
                *sig = mp_signal_t_play;
                break;
            case mp_signal_t_pause:
                BASS_ChannelPause(*channel);
                *sig = mp_signal_t_pause;
                break;
            case mp_signal_t_play:
                BASS_ChannelPlay(*channel, FALSE); 
                *sig = mp_signal_t_play;
                break;
        }
    }

    static void mcmp3_server_connection_handler(void* vptr_server) {
        mp::Server* server = (mp::Server*)vptr_server;
        int sock = server->getNewSocket();
        int read_size, ret_value = 0, ret_sig;
        char client_message[2000];
        mp_signal_t *server_sig = server->getSignalPointer();
        mp::Log* logger = server->getLogger();

        while(true) {
            memset(client_message, 0, 2000);

            ret_value = read(sock, &ret_sig, sizeof(ret_sig));
            read_size = recv(sock, client_message, 2000, 0);

            if(ret_value) {
                if(ntohl(ret_sig) == mp_signal_t_end_connection) {
                    break;
                } else if(ntohl(ret_sig) == mp_signal_t_close_server) {
                    *server_sig = mp_signal_t_close_server;
                    break;
                }

                parse_message(server, (mp_signal_t)ntohl(ret_sig), 
                                client_message);

                send(sock, client_message, strlen(client_message), 0);

            } else {
                // error
            }
            // send data back
            /*send(sock, client_message, strlen(client_message), 0);
            memset(client_message, 0, strlen(client_message));*/

            //parse_message(mcmp_s, ntohl(sig), client_message);
        }
        //std::cout << "Client disconnected\n";
        logger->logMessage("Client disconnected");
        if(read_size == 0)
            fflush(stdout);
        else if(read_size == -1)
            perror("recv failed");
        close(server->getNewSocket());
        return;
    }

    Server::Server():m_song_namet(nullptr) {

    }

    mp::Log* Server::getLogger() {
        return &m_logger;
    }

    char* Server::getCurrentSongName() {
        return m_song_name;
    }

    int Server::getNewSocket() const {
        return *m_new_socket;
    }

    HSAMPLE* Server::getMusicSample() {
        return &m_sample;
    }

	HCHANNEL* Server::getMusicChannel() {
        return &m_channel;
    }
    mp_signal_t* Server::getSignalPointer() {
        return &m_signal;
    }

    bool Server::init(char* ip_addr, int port) {
        m_socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (m_socket_desc == -1) {
            return true;
        }
        
        m_server.sin_family = AF_INET;
        m_server.sin_addr.s_addr = inet_addr(ip_addr);
        m_server.sin_port = htons(port);
        
        if(bind(m_socket_desc, (struct sockaddr *)&m_server, 
            sizeof m_server) < 0) {
            return true;
        }
        listen(m_socket_desc, 3);
        m_sockaddr_size = sizeof(struct sockaddr_in);
        
        // init bass library
        // needs support for config file ==> freq etc.
        BASS_Init(-1, 44100, 0, 0, NULL);
        m_logger.logMessage((char*) "Server initialized. \
        Bass library initialized.");

        return false;
    }

    void Server::run() {
        m_logger.logMessage("Starting the server");
        while((m_client_socket = accept(m_socket_desc, 
                (struct sockaddr *)&m_client,
                (socklen_t*)&m_sockaddr_size))) {

            m_logger.logMessage("Client connected");
            
            m_new_socket = (int*) malloc(sizeof(int));
            *m_new_socket = m_client_socket;

            std::thread new_thread(mcmp3_server_connection_handler, this);
            new_thread.join();

            if(m_signal == mp_signal_t_close_server) {
                m_logger.logMessage("Recieved shutdown signal");
                m_logger.logMessage("Closing the server");
                break;
            }
        }
    }

    Server::~Server() {
        BASS_Free();
    }

}