#include "joseUtils.h"
#include "account/account.h"
#include "oidc-agent/oidcd/jose/oidc_jwk.h"
#include "oidc-agent/oidcd/jose/oidc_jws.h"
#include "utils/keySet.h"
#include "utils/listUtils.h"
#include "utils/logger.h"
#include "utils/memory.h"
#include "utils/oidc_error.h"

#include <cjose/cjose.h>

void initCJOSE() { cjose_set_alloc_funcs(secAlloc, secRealloc, _secFree); }

cjose_header_t* cjose_getSignHeader(const char* sign_alg) {
  if (sign_alg == NULL) {
    oidc_setArgNullFuncError(__func__);
    return NULL;
  }
  cjose_err       err;
  cjose_header_t* hdr = cjose_header_new(&err);
  if (hdr == NULL) {
    char* err_msg =
        oidc_sprintf("Error while creating jose header: %s", err.message);
    oidc_setInternalError(err_msg);
    logger(ERROR, err_msg);
    secFree(err_msg);
    return NULL;
  }
  if (!cjose_header_set(hdr, CJOSE_HDR_ALG, sign_alg, &err)) {
    char* err_msg =
        oidc_sprintf("Error while setting sign algorithm: %s", err.message);
    oidc_setInternalError(err_msg);
    logger(ERROR, err_msg);
    secFree(err_msg);
    return NULL;
  }
  return hdr;
}

void secFreeJoseHeader(cjose_header_t* hdr) { cjose_header_release(hdr); }

struct supportedSignAlgorithmsStr {
  char* userinfo;
  char* id_token;
  char* request;
  char* token_endpoint;
};

struct supportedSignAlgorithmsList {
  list_t* userinfo;
  list_t* id_token;
  list_t* request;
  list_t* token_endpoint;
};

list_t* getCommonSupportedSignAlgorithms(
    const struct supportedSignAlgorithmsList s) {
  list_t* tmp     = intersectLists(s.request, s.token_endpoint);
  list_t* commons = tmp;
  return commons;
}

char* chooseSignAlg(const char* supported_sign_algorithms) {
  char* alg;
  return alg;
}

struct keySetSEstr createRSAKeys(struct oidc_account* a) {
  struct keySetPPstr keys_s = createSigningKey();
  struct keySetPPstr keys_e = createEncryptionKey();
  struct keySetSEstr priv   = {.sign = keys_s.priv, .enc = keys_e.priv};
  struct keySetSEstr pub    = {.sign = keys_s.pub, .enc = keys_e.pub};
  account_setJWKS(a, priv);
  return pub;
}

char* jws_signWithJWKString(const char* plain, const char* jwk_str,
                            const char* alg) {
  cjose_jwk_t* jwk  = import_jwk(jwk_str);
  char*        sign = jws_sign(plain, jwk, alg);
  secFreeJWK(jwk);
  return sign;
}

char* jose_sign(const char* plain, const struct oidc_account* account) {
  if (plain == NULL || account == NULL) {
    oidc_setArgNullFuncError(__func__);
    return NULL;
  }
  return jws_signWithJWKString(plain, account_getJWKSign(account),
                               account_getRequestObjectSignAlg(account));
}