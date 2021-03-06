#ifndef OIDC_PROXY_HANDLER_H
#define OIDC_PROXY_HANDLER_H

#include "utils/oidc_error.h"

oidc_error_t updateRefreshToken(const char* shortname,
                                const char* refresh_token);
char*        getAutoloadConfig(const char* shortname, const char* issuer,
                               const char* application_hint);
char*        getDefaultAccountConfigForIssuer(const char* issuer_url);

#endif  // OIDC_PROXY_HANDLER_H
