/*! \file httpc.h
 *  \brief HTTP Client Module
 *
 * The HTTP Client module implements the functionality to perform HTTP
 * operations like GET and POST on a remote web server. The HTTP client handles
 * chunked encoding making it transparent to the user. It also supports
 * persistent connections with the web server. Multiple HTTP requests can thus
 * be channelled through the same persistent connection with the server.
 *
 * \section httpc_usage Usage
 *
 * The typical HTTP Client usage scenario is as follows:
 *
 * -# Initiate a connection with the remote Web Server using the
 *    http_open_session() call.
 * -# Prepare the request with a call to http_prepare_req. The mandatory
 *    fields are filled depending on the members set in httpc_req_t. If some
 *    default fields are to be set, this is set by enabling the
 *    corresponding flag bits passed to http_prepare_req. If more custom
 *    headers are to be added please add them using the call add_header.
 * -# Send one or more HTTP requests to the server using the
 *    http_prepare_req and http_send_request pair together. . The \ref
 *    httpc_req_t structure should be populated with the desired fields for
 *    each call if changed
 * -# An optional call to http_get_response_hdr() can be made if the application
 *    wishes to determine certain characteristics of the HTTP response.
 * -# Mutiple calls to http_read_content() are made for reading the response of
 *    the remote Web Server.
 * -# Once all the HTTP requests have been made, the connection with the remote
 *    server is terminated using the http_close_session() call.
 *
 */

/*
 *  Copyright 2008-2012, Marvell International Ltd.
 *  All Rights Reserved.
 */

#ifndef _HTTPC_H_
#define _HTTPC_H_
#include <hferrno.h>
#include <time.h>

#ifdef CONFIG_ENABLE_TLS
#include <wm-tls.h>
#else
typedef void tls_init_config_t;
#endif /* CONFIG_ENABLE_TLS */

typedef int http_session_t;

/* Request methods */
typedef enum {
	HTTP_OPTIONS,		/* request to server for communication  options */
	HTTP_GET,		/* retrieve information */
	HTTP_HEAD,		/* get meta-info */
	HTTP_POST,		/* request to accept new sub-ordinate of resource */
	HTTP_PUT,		/* modify or create new resource referred to by URI */
	HTTP_DELETE,		/* delete the resource */
	HTTP_TRACE,		/* echo */
	HTTP_CONNECT,		/* do we need this  ? */
} http_method_t;

typedef enum {
	HTTP_VER_1_0,
	HTTP_VER_1_1,
} http_ver_t;


/*
 * The OR of zero or more flags below is passed to the function
 * \ref http_prepare_req. If the a flag is passed the corresponding HTTP
 * header field is added to the HTTP header. The values added will be
 * default ones.
 */
typedef enum {
	HDR_ADD_DEFAULT_USER_AGENT	= 0x0001,
	/* Note: This flag is not necessary to set up persistent
	 * connections in HTTP 1.1. However, if you want the server to
	 * respond with persistent connection timeout values you may need
	 * to add this flag. These timeout values are used to find out how
	 * long a persistent connection will be kept alive by the
	 * server. */
	HDR_ADD_CONN_KEEP_ALIVE		= 0x0002,
	HDR_ADD_CONN_CLOSE		= 0x0004,
	HDR_ADD_TYPE_CHUNKED            = 0x0008,
} http_hdr_field_sel_t;


#define STANDARD_HDR_FLAGS			\
	 (HDR_ADD_DEFAULT_USER_AGENT)

/*
 * Note 1: A resource is a part of the string immediately after the
 * hostname[:portno] part of the URL. In the URL,
 * [http://]hostname[:portno][/path/to/resource], 
 * "/path/to/resource" part is called as a resource. It starts with the
 * character '/'.
 */
/** The HTTP Client's Request structure
 *
 * \par
 * The application should fill up this structure and pass it to the
 * http_prepare_req() function while making an HTTP request. More name-value
 * fields can be added to HTTP header using http_add_header().
 */
typedef struct {
	/** The Type of HTTP Request */
	http_method_t type;
	/** The target resource for the HTTP Request. A complete URL is also
	 * accepted.
	 */
	const char *resource;
	/** The HTTP Protocol Version */
	http_ver_t version;
	const char *content;
	/** The length of the data pointed to by \a content above. This is
	 * don't-care if the content is set to NULL
	 */
	int content_len;
} httpc_req_t;

typedef struct {
	const char *protocol;
	http_ver_t version;
	int status_code;
	const char *reason_phrase;	/* verbose string describing status code */
	const char *server;
	time_t modify_time;	/* time when the resource was last modified */
	const char *content_type;
	const char *content_encoding;
	bool keep_alive_ack;
	int keep_alive_timeout;	/* Server timeout */
	int keep_alive_max;	/* Max time for which a persistent
				   connection will be open, irrespective of
				   connection activity  */
	bool chunked;
	uint32_t content_length;	/* This will be 0 if chunked == TRUE */
} http_resp_t;

typedef struct {
	char *name;
	char *value;
} http_header_pair_t;

typedef struct {
	const char *scheme;
	const char *hostname;
	unsigned portno;
	const char *resource;
} parsed_url_t;

/** HTTPC Error Codes **/

enum wm_httpc_errno {
	HF_E_HTTPC_ERRNO_START = MOD_ERROR_START(MOD_HTTPC),
	/* TCP connection failed (maybe due to unreachable server) */
	HF_E_HTTPC_TCP_CONNECT_FAIL,
	/* HTTP File not found */
	HF_E_HTTPC_FILE_NOT_FOUND,
	/* HTTP Bad Request */
	HF_E_HTTPC_BAD_REQUEST,
	/* TLS not enabled */
	HF_E_HTTPC_TLS_NOT_ENABLED,
};


/* Status codes */
#define HTTP_RESP_INFORMATIONAL(x) (x >=100 && < 200)
#define HTTP_RESP_SUCCESS(x) (x >= 200 && x < 300)
#define HTTP_RESP_REDIR(x) (x >= 300 && x < 400)
#define HTTP_RESP_CLIENT_ERR(x) (x >= 400 && x < 500)
#define HTTP_RESP_SERVER_ERR(x) (x >= 500 && x < 600)

/* 
 * These macros are not of any use to the HTTP client itself. They are used
 * by the users of the HTTP client. This list may be extended if required 
 */
#define HTTP_OK 200
#define HTTP_CREATED 201
#define HTTP_ACCEPTED  202
#define HTTP_FOUND 302
#define HTTP_NOT_MODIFIED 304

#define HTTP_BAD_REQUEST 400
#define HTTP_NOT_AUTH 401
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404

/**
 * If the given URL has a scheme field, and it is https then the http
 * client will auto-switch to SSL mode, irrespective of the TLS_ENABLE
 * flag below.
 */
typedef enum {
	/** Pass this flag when you want the connection to be SSL based */
	TLS_ENABLE = 0x01,
} http_open_flags_t;

/**
 * Start an HTTP session.
 *
 * @par
 * This function starts a new HTTP session. It will create a socket and
 * then connect to the server.
 *
 *  @param[in,out] handle Pointer to a handle for the session. Will be
 *  assigned by the callee.
 *  @param[in] hostname Host name or IP address.  The hostname should be in
 *  the format [http://]hostname[:portno][/path/to/resource]. The fields
 *  between the  square brackets above are optional. The part specifying
 *  the resource will be ignored. The resource detail is expected by the \a
 *  http_send_request API to be called later after calling current function.
 *  @param[in] flags This is OR of the flags mentioned in enum
 *  \ref http_open_flags_t. Please look at the declaration of the enum for
 *  more information.
 *  @param[in] cfg This is a pointer of the type
 *  \ref tls_init_config_t. This will be passed verbatim to the tls
 *  layer. Please look at the wm-tls.h for the structure members. \b If TLS
 *  (secure connection) is not needed please pass NULL.
 *  @param[in] retry_cnt On some systems with constrained network resources,
 *  only a limited sockets can be active at any given time. If the socket
 *  creation fails, http_open_session will wait for some time and try to create
 *  the socket again for retry_cnt number of times. If 0 is passed then it will
 *  try default number of times depending on the platform. 0 is the recommended
 *  option.
 *
 * @return WM_SUCCESS on success
 * @return -HF_E_INVAL if any of the arguments were invalid
 * @return -WM_FAIL if the function was unable to initiate an HTTP session with
 * the server.
 */
int http_open_session(http_session_t * handle, const char *hostname,
		      int flags, const tls_init_config_t *cfg,
		      int retry_cnt);

#define hfhttp_open_session	http_open_session

/**
 * Prepare the HTTP header.
 *
 * This function will prepare an HTTP request before sending out an HTTP
 * request using the function \ref http_send_request. Calling this function is
 * mandatory before calling \ref http_send_request.
 *
 * @pre \ref http_open_session
 *
 * @param[in] handle Handle returned from the call to \a http_open_session
 * @param[in] req Allocated and initialized \ref httpc_req_t structure.
 * @param[in] field_flags Bitwise OR of flags of enum \ref
 * http_hdr_field_sel_t as required by the caller.
 *
 * @note The field_flags parameter is a way to request the http client to
 * use default field values for standard fields. If the corresponding bits
 * are not set then the respective fields are not added. The user can add
 * any custom fields including the standard ones using the API
 * \ref http_add_header
 *
 * @return Standard hfsdk return codes.
 */
int http_prepare_req(http_session_t handle, const httpc_req_t *req,
		     http_hdr_field_sel_t field_flags);

#define hfhttp_prepare_req	http_prepare_req			 

/**
 * Add custom http headers to the partial generated header.
 *
 * This function allows the caller to add custom headers to the partial
 * http header generated by earlier call to \ref http_prepare_req.
 *
 * @note This function is optional and needs to be called only if custom
 * headers (or standard headers with non-default values) are to be added.
 *
 * @pre \ref http_prepare_req
 *
 * @param[in] handle Handle returned from the call to \a http_open_session
 * @param[in] name The name of the field. e.g User-Agent or
 * If-Modified-Since
 * @param[in] value Value associated with the name given above.
 *
 * @return Standard hfsdk value.
 */
int http_add_header(http_session_t handle, const char *name,
		const char *value);

#define hfhttp_add_header	http_add_header
/**
 * Perform an HTTP request. 
 *
 * @par
 * This function will peform an HTTP request.
 *
 * @note If this function fails and the caller decides to abandon this
 * session altogether it needs to call  http_close_session()
 * explicitly. Failure to do this will cause undefined behaviour.
 *
 * @pre \a http_prepare_req mandatory. \a http_add_header optional.
 *
 * @param[in] handle Handle returned from the call to \a http_open_session
 * @param[in] req The \httpc_req_t structure filled up with appropriate
 * parameters as sent earlier to \ref http_prepare_req
 *
 * @return WM_SUCCESS on success
 * @return -HF_E_IO if failed to send data to network
 * @return -HF_E_INVAL for an invalid arguments.
 */
int http_send_request(http_session_t handle, const httpc_req_t * req);

#define hfhttp_send_request	http_send_request
/**
 * Get the HTTP response header
 *
 * @par
 * This function parses the response header received from the server and
 * populates the \a http_resp_t structure and returns the pointer to this
 * structure. The structure \a http_resp_t is allocated by the callee.
 * 
 * @par
 * The caller is expected to analyse this information to find out the
 * result of the earlier request sent through http_send_request(). If the
 * earlier request was HTTP_GET then the caller will need to call
 * http_read_content() to read the data sent by the server.
 *
 * @note Calling this function is \b optional. If the caller does not want to
 * read/analyse the header and knows beforehand what to expect, s/he can
 * directly call http_read_content(). The function \a http_read_content
 * will internally read the HTTP response header. However, the function 
 * \a http_read_content will return an error if the HTTP status code
 * returned is not 200.  If you need to read content in cases where the
 * return status code is other than 200 you need to call \ref
 * http_get_response_hdr first.
 *
 * @note If this function fails and the caller decides to abandon this
 * session altogether it needs to call http_close_session()
 * explicitly. Failure to do this will cause undefined behaviour.
 * 
 * @pre \a http_send_request is called before this call.
 *
 * @param[in] handle Handle returned from the call to \a http_new_session
 * @param[in,out] resp Pointer to a pointer of type \a http_resp_t. The
 * structure will be allocted by the callee.
 *
 * @return WM_SUCCESS on success
 * @return -WM_FAIL on error
 */
int http_get_response_hdr(http_session_t handle, http_resp_t ** resp);

#define hfhttp_get_response_hdr		http_get_response_hdr

/**
 * Get a particular HTTP response header value.
 *
 * @par
 * This function parses the response header received from the server and
 * returns the value corresponding to a given name.
 *
 * @par
 * The caller is not expected to call \ref http_get_response_hdr before
 * this. However, calling \ref http_get_response_hdr is recommended so that
 * the user gets to know the exact status of the HTTP operation before he
 * tries to extract any field not present in structure \ref http_resp_t.
 *
 * @note Calling this function is \b optional. If the caller does not want to
 * read/analyse the header and knows beforehand what to expect, s/he can
 * directly call http_read_content(). The function \a http_read_content
 * will internally read the HTTP response header. However, the function
 * \a http_read_content will return an error if the HTTP status code
 * returned is not 200. If you need to read content in cases where the
 * return status code is other than 200 you need to call \ref
 * http_get_response_hdr or the current function first.
 *
 * @note If this function fails and the caller decides to abandon this
 * session altogether it needs to call http_close_session()
 * explicitly. Failure to do this will cause undefined behaviour.
 *
 * @pre \a http_send_request is called before this call. Calling \ref
 * http_get_response_hdr is \b not mandatory but recommended.
 *
 * @param[in] handle Handle returned from the call to \a http_new_session
 * @param[in] header_name The name of the field whose value you wish to
 * know
 * @param[in,out] value A pointer which will point to the value
 * corresponding to the name passed as second parameter.
 *
 * @return WM_SUCCESS on success
 * @return -WM_FAIL on error
 */

int http_get_response_hdr_value(http_session_t handle,
				const char *header_name, char **value);
				
#define hfhttp_get_response_hdr_value	http_get_response_hdr_value				
				
/**
 * Get requested number of HTTP response header name-value pairs.
 *
 * @par
 * This function parses the response header received from the server and
 * returns requested number of HTTP response header name-value pairs.
 *
 * @par
 * The caller is not expected to call \ref http_get_response_hdr before
 * this. However, calling \ref http_get_response_hdr is recommended so that
 * the user gets to know the exact status of the HTTP operation before he
 * tries to extract any field not present in structure \ref http_resp_t.
 *
 * @note Calling this function is \b optional. If the caller does not want to
 * read/analyse the header and knows beforehand what to expect, s/he can
 * directly call http_read_content(). The function \a http_read_content
 * will internally read the HTTP response header. However, the function
 * \a http_read_content will return an error if the HTTP status code
 * returned is not 200. If you need to read content in cases where the
 * return status code is other than 200 you need to call \ref
 * http_get_response_hdr or the current function first.
 *
 * @note If this function fails and the caller decides to abandon this
 * session altogether it needs to call http_close_session()
 * explicitly. Failure to do this will cause undefined behaviour.
 *
 * @pre \a http_send_request is called before this call. Calling \ref
 * http_get_response_hdr is \b not mandatory but recommended.
 *
 * @param[in] handle Handle returned from the call to \a http_new_session
 * @param[in,out] arr A pointer that points to an array of the
 * structure to hold the response header name-value pairs.
 * This array needs to be pre-allocated and its pointer is to be passed,
 * this pointer shall point to the array of requested number of
 * response header name-value pairs.
 * @param[in,out] count An interger pointer that points to the number of
 * response header name-value pairs that the user wants.
 * This has to be passed by the user and it is later updated to the
 * actual number of response header name-value pairs that were filled
 * in the array mentioned above.
 * @return WM_SUCCESS on success
 * @return -WM_FAIL on error
 */

int http_get_response_hdr_all(http_session_t handle, http_header_pair_t *arr,
			      int *count);
#define hfhttp_get_response_hdr_all	http_get_response_hdr_all
/**
 * Read data content from the stream.
 *
 *@par
 * This function will read the data sent by the server.
 * 
 * @par
 * The data transfer from the server to the client may be chunked. This
 * fact will be reported in the response structure \ref http_resp_t returned
 * in a previous call to http_get_response_hdr(). Notwithstanding this
 * information, chunked data handling is transparent to the
 * caller. Irrespective of the transfer type, this function should be called
 * repeatedly till the return value of the function is zero.
 *
 * @note If this function fails and the caller decides to abandon this
 * session altogether it needs to call http_close_session()
 * explicitly. Failure to do this will cause undefined behaviour.
 * 
 * @param[in] handle Handle returned from the call to http_new_session()
 * @param[in, out] buf Caller allocated buffer of size max_len
 * @param[in] Size of the buffer. The data read will be less than or equal
 * to this size, depending on the size of the resource.
 *
 * @return On success, the number of bytes read is returned. Return value
 * of zero is \a not an error case  and should be treated as a signal that
 * server is done with sending the data.
 * @return -WM_FAIL if this function is called again after the previous call
 * retuned 0 or for any other error.
 */

int http_read_content(http_session_t handle, void *buf, uint32_t max_len);

#define hfhttp_read_content(_h,_b,_l)	http_read_content(_h,_b,_l)

/**
 * Parse the given string into URL subcomponents.
 *
 * The hostname string syntax is:
 *[scheme://]hostname[:port][/path_to_resource?query_string#fragment_id]
 * The fields between the square brackets above are optional.
 *
 * @note <b>Buffer management</b>: To free the caller of the task of
 * allocating multiple buffers of magic size to store each of the
 * subcomponents we use the following strategy. This function takes a tmp
 * buffer, allocted (static or dynamic) by the caller to store the
 * subcomponents. The caller needs to allocate this buffer with size equal
 * to (strlen(URL) + 10). When this function returns, the pointers in the
 * parsed_url structure will point to appropriate address in this
 * buffer. The caller may free this buffer after usage of the
 * sub-components is complete. Freeing before that will cause undefined behavior.
 *
 * @param[in] URL Pointer to the string containing the URL. This function
 * will not modify this argument.
 * @param[in] tmp_buf A buffer where the strings will be stored after
 * parsing. Needs to be allocated by the caller and should be of size
 * atleast 10 greater than size of the URL string passed as first
 * argument.
 * @param[in] tmp_buf_len Size of the caller allocated, temporary buffer
 * sent as second argument. 
 * @param[in, out] Pointer to an allocated structure. The member pointers
 * will point to the appropriate address in the temporary buffer holding
 * the NULL terminated string corresponding to the subcomponent.
 *
 */
int http_parse_URL(const char *URL, char *tmp_buf, int tmp_buf_len,
		   parsed_url_t * parsed_url);

#define hfhttp_parse_URL(_url,_b,_bl,_purl)		http_parse_URL(_url,_b,_bl,_purl)
		   
/**
 * HTTP lowlevel read
 *
 * This API is present to serve clients which want to handle the
 * HTTP stream directly. This is useful for handling websockets for e.g.
 *
 * @warning This is not for general users of HTTP client API. Normal HTTP
 * transactions do not need this API.
 *
 * @note Once this API used no other API from the HTTP API set, except \a
 * http_close_session, can be used. This is because once this API is used
 * the HTTP client loses track of the stream and only the caller using the
 * API can know the state of the stream.
 *
 * @param[in] handle Handle returned from the call to http_new_session()
 * @param[in, out] buf Pointer to an allocated buffer of size equal to or
 * more than the value of the third parameter maxlen
 * @param[in] maxlen The maximum number of bytes to be read from the
 * network. Note that the actual read bytes can be less than this.
 *
 * @return Number of bytes read. -HF_E_INVAL is returned in case of invalid
 * parameters. Standard libc error codes are returned in case when there
 * other problems.
 */

int http_lowlevel_read(http_session_t handle, void *buf, unsigned maxlen);

#define hfhttp_lowlevel_read(_h,_b,_ml)	http_lowlevel_read(_h,_b,_ml)

/**
 * HTTP lowlevel write
 *
 * This API is present to serve clients which want to handle the
 * HTTP stream directly. This is useful for handling websockets for e.g.
 *
 * @warning This is not for general users of HTTP client API. Normal HTTP
 * transactions do not need this API.
 *
 * @note Once this API used no other API from the HTTP API set, except \a
 * http_close_session, can be used. This is because once this API is used
 * the HTTP client loses track of the stream and only the caller using the
 * API can know the state of the stream.
 *
 * @param[in] handle Handle returned from the call to http_new_session()
 * @param[in, out] buf Pointer to a buffer which has the data to be written
 * out.
 * @param[in] maxlen The number of bytes to be written to the
 * network.
 *
 * @return Number of bytes written. -HF_E_INVAL is returned in case of
 * invalid parameters. Standard libc error codes are returned in case when
 * there other problems.
 */

int http_lowlevel_write(http_session_t  handle, const void *buf, unsigned len);

#define hfhttp_lowlevel_write(_h,_b,_l)	http_lowlevel_write(_h,_b,_l)

/**
 * HTTP write chunked
 *
 * This API is used for chunked transfer encoding
 *
 * @note To end the chunked data transfer, this function should be called with
 * len=0, buf may be NULL
 *
 * @param[in] handle Handle retrieved from the call to http_open_session
 * @param[in] data Pointer to the buffer to be posted
 * @param[in] len The number of bytes to be posted
 * @return WM_SUCCESS is returned on success. -HF_E_INVAL is returned in case
 * of invalid parameters. -WM_FAIL is returned if session is invalid.
 * Standard libc error codes are returned when there are other problems.
 */

int httpc_write_chunked(http_session_t handle, const char *data, int len);
#define hfhttpc_write_chunked(_h,_d,_l)	httpc_write_chunked(_h,_d,_l)

/**
 * Close the session.
 *
 * @par
 * This function will close the session represented by the given
 * handle. The socket associated with this session will be closed and thus
 * the TCP connection with the server will be terminated. No requests on
 * this same session will succeed after this function is called.
 *
 * @note <b>The response structure \ref http_resp_t returned earlier from
 *  http_get_response_hdr() will be invalid after this call.</b>
 *
 * @param[in] handle Handle returned from the previous call to
 * http_new_session(). The handle will be set to NULL by the callee thus
 * effectively blocking futher requests on the same session handle.
 *
 */
void http_close_session(http_session_t * handle);

#define hfhttp_close_session(_h)	http_close_session(_h)

#endif /* _HTTPC_H_ */
