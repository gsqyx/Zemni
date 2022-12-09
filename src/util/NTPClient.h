#ifndef ZEMNI_NTPCLIENT_H
#define ZEMNI_NTPCLIENT_H

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <chrono>
#include <string>
#include <glog/logging.h>
#include "../Config.h"

#define NTP_TIMESTAMP_DELTA 2208988800ull
#define LI(packet)   (uint8_t) (((packet).li_vn_mode & 0xC0) >> 6) // (li   & 11 000 000) >> 6
#define VN(packet)   (uint8_t) (((packet).li_vn_mode & 0x38) >> 3) // (vn   & 00 111 000) >> 3
#define MODE(packet) (uint8_t) (((packet).li_vn_mode & 0x07) >> 0) // (mode & 00 000 111) >> 0

namespace Zemni
{
    using std::string;

    /**
     * Set NTP hostname first.
     */
    class NTPClient
    {
    private:
        NTPClient()
        {
            struct hostent *server;      // Server data structure.
            string ntp_host(NTP_HOST);
            host_name = ntp_host.c_str();
            server = gethostbyname(host_name); // Convert URL to IP.
            CHECK_NOTNULL(server);
            bzero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
            serv_addr.sin_port = htons(portno);
        }

    public:
        NTPClient(NTPClient const &) = delete;

        void operator=(NTPClient const &) = delete;

        static NTPClient &getInstance()
        {
            static NTPClient client;
            return client;
        }

    public:
        time_t getSecondTimestamp()
        {
            ntp_packet packet = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            packet = getNtpPacket();
            return (time_t) (packet.txTm_s - NTP_TIMESTAMP_DELTA);
        }

        time_t getMillisecondTimestamp()
        {
            ntp_packet packet = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            packet = getNtpPacket();
            auto txTm = (time_t) (packet.txTm_s - NTP_TIMESTAMP_DELTA);
            double milliseconds = (double) txTm * 1000.0;
            // Add fractional part.
            milliseconds += ((double) packet.txTm_f / 4294967295) * 1000.0;
            return (time_t) milliseconds;
        }

    private:
        const char *host_name{};
        struct sockaddr_in serv_addr{}; // Server address data structure.
        int portno = 123; // NTP UDP port number.

        // Structure that defines the 48 byte NTP packet protocol.
        typedef struct
        {

            uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
            // li.   Two bits.   Leap indicator.
            // vn.   Three bits. Version number of the protocol.
            // mode. Three bits. Client will pick mode 3 for client.

            uint8_t stratum;         // Eight bits. Stratum level of the local clock.
            uint8_t poll;            // Eight bits. Maximum interval between successive messages.
            uint8_t precision;       // Eight bits. Precision of the local clock.

            uint32_t rootDelay;      // 32 bits. Total round trip delay time.
            uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
            uint32_t refId;          // 32 bits. Reference clock identifier.

            uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
            uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

            uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
            uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

            uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
            uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

            uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
            uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.

        } ntp_packet;              // Total: 384 bits or 48 bytes.

        void error(char *msg)
        {
            perror(msg); // Print the error message to stderr.
            exit(0); // Quit the process.
        }

        ntp_packet getNtpPacket()
        {
            int sockfd, n; // Socket file descriptor and the n return result from writing/reading from the socket.
            // Create and zero out the packet. All 48 bytes worth.

            ntp_packet packet = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            memset(&packet, 0, sizeof(ntp_packet));

            // Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3. The rest will be left set to zero.

            *((char *) &packet + 0) = 0x1b; // Represents 27 in base 10 or 00011011 in base 2.

            sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Create a UDP socket.
            CHECK(sockfd >= 0) << "ERROR opening socket";

            if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
                error(const_cast<char *>("ERROR connecting"));

            // Send it the NTP packet it wants. If n == -1, it failed.
            n = static_cast<int>(write(sockfd, (char *) &packet, sizeof(ntp_packet)));
            CHECK(n >= 0) << "ERROR writing to socket";

            // Wait and receive the packet back from the server. If n == -1, it failed.
            n = static_cast<int>(read(sockfd, (char *) &packet, sizeof(ntp_packet)));
            CHECK(n >= 0) << "ERROR reading from socket";

            close(sockfd);

            // These two fields contain the time-stamp seconds as the packet left the NTP server.
            // The number of seconds correspond to the seconds passed since 1900.
            // ntohl() converts the bit/byte order from the network's to host's "endianness".
            packet.txTm_s = ntohl(packet.txTm_s); // Time-stamp seconds.
            packet.txTm_f = ntohl(packet.txTm_f); // Time-stamp fraction of a second.
            return packet;
        }
    };
}
#endif //ZEMNI_NTPCLIENT_H
