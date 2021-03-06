#ifndef OIDC_FILE_IO_H
#define OIDC_FILE_IO_H

#include "list/list.h"
#include "utils/oidc_error.h"

char*        getOidcDir();
oidc_error_t createOidcDir();
oidc_error_t writeOidcFile(const char* filename, const char* text);
oidc_error_t appendOidcFile(const char* filename, const char* text);
char*        readOidcFile(const char* filename);
int          oidcFileDoesExist(const char* filename);
int          removeOidcFile(const char* filename);
char*        concatToOidcDir(const char* filename);
void         updateIssuerConfig(const char* issuer_url, const char* shortname);
list_t*      getLinesFromOidcFile(const char* filename);

#endif  // OIDC_FILE_IO_H
