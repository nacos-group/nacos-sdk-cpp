#ifndef __HTTP_STAT_H_
#define __HTTP_STAT_H_
//Copied from JAVA source code HttpURLConnection.java
// REMIND: do we want all these??
// Others not here that we do want??

/* 2XX: generally "OK" */

/**
 * HTTP Status-Code 200: OK.
 */
#define  HTTP_OK 200

/**
 * HTTP Status-Code 201: Created.
 */
#define  HTTP_CREATED 201

/**
 * HTTP Status-Code 202: Accepted.
 */
#define  HTTP_ACCEPTED 202

/**
 * HTTP Status-Code 203: Non-Authoritative Information.
 */
#define  HTTP_NOT_AUTHORITATIVE 203

/**
 * HTTP Status-Code 204: No Content.
 */
#define  HTTP_NO_CONTENT 204

/**
 * HTTP Status-Code 205: Reset Content.
 */
#define  HTTP_RESET 205

/**
 * HTTP Status-Code 206: Partial Content.
 */
#define  HTTP_PARTIAL 206

/* 3XX: relocation/redirect */

/**
 * HTTP Status-Code 300: Multiple Choices.
 */
#define  HTTP_MULT_CHOICE 300

/**
 * HTTP Status-Code 301: Moved Permanently.
 */
#define  HTTP_MOVED_PERM 301

/**
 * HTTP Status-Code 302: Temporary Redirect.
 */
#define  HTTP_MOVED_TEMP 302

/**
 * HTTP Status-Code 303: See Other.
 */
#define  HTTP_SEE_OTHER 303

/**
 * HTTP Status-Code 304: Not Modified.
 */
#define  HTTP_NOT_MODIFIED 304

/**
 * HTTP Status-Code 305: Use Proxy.
 */
#define  HTTP_USE_PROXY 305

/* 4XX: client error */

/**
 * HTTP Status-Code 400: Bad Request.
 */
#define  HTTP_BAD_REQUEST 400

/**
 * HTTP Status-Code 401: Unauthorized.
 */
#define  HTTP_UNAUTHORIZED 401

/**
 * HTTP Status-Code 402: Payment Required.
 */
#define  HTTP_PAYMENT_REQUIRED 402

/**
 * HTTP Status-Code 403: Forbidden.
 */
#define  HTTP_FORBIDDEN 403

/**
 * HTTP Status-Code 404: Not Found.
 */
#define  HTTP_NOT_FOUND 404

/**
 * HTTP Status-Code 405: Method Not Allowed.
 */
#define  HTTP_BAD_METHOD 405

/**
 * HTTP Status-Code 406: Not Acceptable.
 */
#define  HTTP_NOT_ACCEPTABLE 406

/**
 * HTTP Status-Code 407: Proxy Authentication Required.
 */
#define  HTTP_PROXY_AUTH 407

/**
 * HTTP Status-Code 408: Request Time-Out.
 */
#define  HTTP_CLIENT_TIMEOUT 408

/**
 * HTTP Status-Code 409: Conflict.
 */
#define  HTTP_CONFLICT 409

/**
 * HTTP Status-Code 410: Gone.
 */
#define  HTTP_GONE 410

/**
 * HTTP Status-Code 411: Length Required.
 */
#define  HTTP_LENGTH_REQUIRED 411

/**
 * HTTP Status-Code 412: Precondition Failed.
 */
#define  HTTP_PRECON_FAILED 412

/**
 * HTTP Status-Code 413: Request Entity Too Large.
 */
#define  HTTP_ENTITY_TOO_LARGE 413

/**
 * HTTP Status-Code 414: Request-URI Too Large.
 */
#define  HTTP_REQ_TOO_LONG 414

/**
 * HTTP Status-Code 415: Unsupported Media Type.
 */
#define  HTTP_UNSUPPORTED_TYPE 415

/* 5XX: server error */

/**
 * HTTP Status-Code 500: Internal Server Error.
 * @deprecated   it is misplaced and shouldn't have existed.
 */
#define  HTTP_SERVER_ERROR 500

/**
 * HTTP Status-Code 500: Internal Server Error.
 */
#define  HTTP_INTERNAL_ERROR 500

/**
 * HTTP Status-Code 501: Not Implemented.
 */
#define  HTTP_NOT_IMPLEMENTED 501

/**
 * HTTP Status-Code 502: Bad Gateway.
 */
#define  HTTP_BAD_GATEWAY 502

/**
 * HTTP Status-Code 503: Service Unavailable.
 */
#define  HTTP_UNAVAILABLE 503

/**
 * HTTP Status-Code 504: Gateway Timeout.
 */
#define  HTTP_GATEWAY_TIMEOUT 504

/**
 * HTTP Status-Code 505: HTTP Version Not Supported.
 */
#define  HTTP_VERSION 505

#endif