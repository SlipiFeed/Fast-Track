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
#include <RRCConnectionSetup.h>

#define PORT 8080
#define IP_ADDRESS "127.0.0.1"
#define MAX_BUFFER_SIZE 1024

uint8_t* createEncodedSetupRequest(ssize_t* requestSize)
{
    RRCConnectionSetup_t* rrcRequest = calloc(1, sizeof(RRCConnectionSetup_t));
    asn_enc_rval_t ec;
    if (!rrcRequest) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    
    rrcRequest->rrc_TransactionIdentifier = 1;
    rrcRequest->criticalExtensions.present = RRCConnectionSetup__criticalExtensions_PR_NOTHING;
    rrcRequest->criticalExtensions.choice.c1.present = RRCConnectionSetup__criticalExtensions__c1_PR_NOTHING;

    uint8_t buffer[MAX_BUFFER_SIZE];
    ec = uper_encode_to_buffer(&asn_DEF_RRCConnectionSetup, NULL, rrcRequest, buffer, MAX_BUFFER_SIZE);

    if (ec.encoded == -1) {
        fprintf(stderr, "Could not encode RRCConnectionSetup (at %s)\n", ec.failed_type ? ec.failed_type->name : "unknown");
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
    ssize_t request_size;
    uint8_t *request = createEncodedSetupRequest(&request_size);

    return 0;
}