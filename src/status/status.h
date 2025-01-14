#include <stdint.h>
#include <mbedtls/x509_crt.h>
#include <mbedtls/pk.h>
#include "../packets.h"

typedef uint16_t StatusHandle;

bool status_init(const char *path, uint16_t port);
void status_cleanup(void);

bool status_ssl_init(mbedtls_x509_crt certs[2], mbedtls_pk_context keys[2], const char *domain, const char *path, uint16_t port);
void status_ssl_cleanup(void);

void status_internal_init(void);
StatusHandle status_entry_new(ServerCode code, uint8_t playerCap);
void status_entry_set_playerCount(StatusHandle index, uint8_t count);
void status_entry_set_level(StatusHandle index, const char *name, float nps);
void status_entry_free(StatusHandle index);
