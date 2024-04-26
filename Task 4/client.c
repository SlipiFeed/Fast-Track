#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/sctp.h>
#include <ctype.h>
#include <sys/types.h>
#include <limits.h>
#include <RRCSetupRequest.h>
#include <RRCSetupComplete.h>

#define PORT 8080
#define IP_ADDRESS "127.0.0.1"
#define MAX_BUFFER_SIZE 1024

uint8_t* createEncodedSetupRequest(ssize_t* requestSize)
{
    RRCSetupRequest_t* rrcRequest = calloc(1, sizeof(RRCSetupRequest_t));
    asn_enc_rval_t ec;
    if (!rrcRequest) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    BIT_STRING_t randomValue;
    uint8_t randomBuffer[] = {'1', '2', '3'};
    randomValue.buf = randomBuffer;
    randomValue.size = 3; 
    randomValue.bits_unused = 0;

    rrcRequest->rrcSetupRequest.ue_Identity.present = InitialUE_Identity_PR_randomValue;
    rrcRequest->rrcSetupRequest.ue_Identity.choice.randomValue = randomValue;
    rrcRequest->rrcSetupRequest.establishmentCause = EstablishmentCause_mt_Access;

    uint8_t buffer[MAX_BUFFER_SIZE];
    ec = uper_encode_to_buffer(&asn_DEF_RRCSetupRequest, NULL, rrcRequest, buffer, MAX_BUFFER_SIZE);

    if (ec.encoded == -1) {
        fprintf(stderr, "Could not encode RRCSetupRequest (at %s)\n", ec.failed_type ? ec.failed_type->name : "unknown");
        exit(EXIT_FAILURE);
    }

    *requestSize = (ec.encoded + 7) / 8;
    uint8_t* dynamicBuffer = malloc(*requestSize);
    if (!dynamicBuffer) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memcpy(dynamicBuffer, buffer, *requestSize);

    return dynamicBuffer;
}

int main(int ac, char **av) {
    int connSock, in;
    struct sockaddr_in servaddr = {0};
    struct sctp_sndrcvinfo sndrcvinfo = {0};
    struct sctp_event_subscribe events = {0};
    int flags;
    
    connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if(connSock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    memset(&events, 0, sizeof(events));
    events.sctp_data_io_event = 1;
    setsockopt(connSock, SOL_SCTP, SCTP_EVENTS, (const void *)&events, sizeof(events));

    if(connect(connSock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        close(connSock);
        exit(EXIT_FAILURE);
    }

    ssize_t request_size;
    uint8_t *request = createEncodedSetupRequest(&request_size);
    
    sctp_sendmsg(connSock, (void *)request, request_size, NULL, 0, 0, 0, 0, 0, 0);

    char recv_buffer[MAX_BUFFER_SIZE];
    flags = 0;
    socklen_t from_len = sizeof(servaddr);        
    bzero(recv_buffer, MAX_BUFFER_SIZE);

    in = sctp_recvmsg(connSock, (void *)recv_buffer, sizeof(recv_buffer), (struct sockaddr *)&servaddr, &from_len, &sndrcvinfo, &flags);
    if(in > 0) {
        printf("Received request: %s\n", recv_buffer);
        RRCSetupComplete_t *receivedMessage = NULL;
        asn_dec_rval_t rval = uper_decode_complete(NULL, &asn_DEF_RRCSetupComplete, (void **)&receivedMessage, recv_buffer, from_len);
        if(rval.code == RC_OK && receivedMessage != NULL) {
            xer_fprint(stdout, &asn_DEF_RRCSetupComplete, receivedMessage);
        }
        ASN_STRUCT_FREE(asn_DEF_RRCSetupComplete, receivedMessage);
    }

    close(connSock);
    ASN_STRUCT_FREE(asn_DEF_RRCSetupRequest, request);

    return 0;
}