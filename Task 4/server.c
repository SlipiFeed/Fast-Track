#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <RRCSetupRequest.h>
#include <RRCSetupComplete.h>
#include <netinet/sctp.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int listenSock, connSock;
    struct sockaddr_in servaddr;
    char buffer[MAX_BUFFER_SIZE];
    struct sctp_sndrcvinfo sndrcvinfo;
    struct sctp_event_subscribe events;
    int flags;
    
    listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (listenSock < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    
    bzero((void *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    if (bind(listenSock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
        close(listenSock);
        exit(EXIT_FAILURE);
    }
    
    memset(&events, 0, sizeof(events));
    events.sctp_data_io_event = 1;
    if (setsockopt(listenSock, SOL_SCTP, SCTP_EVENTS, &events, sizeof(events)) < 0) {
        perror("setsockopt error");
        close(listenSock);
        exit(EXIT_FAILURE);
    }
    
    if (listen(listenSock, 5) < 0) {
        perror("listen error");
        close(listenSock);
        exit(EXIT_FAILURE);
    }
    
    printf("Server is listening on port %d\n", PORT);
    
    for (;;) {
        connSock = accept(listenSock, NULL, NULL);
        if (connSock < 0) {
            perror("accept error");
            continue; 
        }
        
        bzero(buffer, MAX_BUFFER_SIZE);
        flags = 0;
        ssize_t numBytes = sctp_recvmsg(connSock, (void *)buffer, MAX_BUFFER_SIZE, NULL, 0, &sndrcvinfo, &flags);
        
        if (numBytes > 0) {
            printf("Received request: %s\n", buffer);
            RRCSetupRequest_t *receivedMessage = NULL;
            asn_dec_rval_t rval = uper_decode_complete(NULL, &asn_DEF_RRCSetupRequest, (void **)&receivedMessage, buffer, numBytes);
            
            if(rval.code == RC_OK && receivedMessage != NULL) {
                xer_fprint(stdout, &asn_DEF_RRCSetupRequest, receivedMessage);
                
                RRCSetupComplete_t *request = calloc(1, sizeof(RRCSetupComplete_t));
                asn_enc_rval_t ec;
                if (!request) {
                    perror("calloc");
                    exit(EXIT_FAILURE);
                }
                request->rrcSetupComplete.ue_Identity.present = InitialUE_Identity_PR_randomValue;
                request->rrcSetupComplete.ue_Identity.choice.randomValue = receivedMessage->rrcSetupRequest.ue_Identity.choice.randomValue;

                uint8_t buffer[MAX_BUFFER_SIZE];
                ec = uper_encode_to_buffer(&asn_DEF_RRCSetupRequest, NULL, request, buffer, MAX_BUFFER_SIZE);

                if (ec.encoded == -1) {
                    fprintf(stderr, "Could not encode RRCSetupComplete (at %s)\n", ec.failed_type ? ec.failed_type->name : "unknown");
                    exit(EXIT_FAILURE);
                }

                ssize_t requestSize = (ec.encoded + 7) / 8;
                uint8_t* dynamicBuffer = malloc(requestSize);
                if (!dynamicBuffer) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }
                memcpy(dynamicBuffer, buffer, requestSize);

                if (sctp_sendmsg(connSock, (void *)dynamicBuffer, requestSize, NULL, 0, 0, 0, 0, 0, 0) < 0) {
                    perror("sctp_sendmsg error");
                }
                ASN_STRUCT_FREE(asn_DEF_RRCSetupRequest, receivedMessage);

            } else {
                fprintf(stderr, "Failed to decode the message\n");
            }
        } else if (numBytes < 0) {
            perror("recvmsg error");
        } else {
            printf("Client disconnected\n");
        }
        
        close(connSock);
    }
    
    close(listenSock);
    return 0;
}