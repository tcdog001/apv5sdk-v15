#ifndef _APNP_TYPE_H
#define _APNP_TYPE_H

#ifndef APNP_FREE
#define APNP_FREE(obj_name)		{if(obj_name){free((obj_name)); (obj_name) = NULL;}}
#endif

#define APNP_FLAGS_STREAM_SOCKET   			(0x1)
#define	APNP_FLAGS_NETLINK_SOCKET   		(0x1 << 1)
#define APNP_FLAGS_LISTEN_SOCKET			(0x1 << 2)
#define APNP_FLAGS_UDP_SOCKET				(0x1 << 3)

#define APNP_MESSAGE_TYPE_RET				(0x1 << 4)


enum {
	APNP_SUCCESS,
	APNP_CONFIG_FAIL,
	APNP_MALLOC_ERROR,
	APNP_CONFIG_EXIST,
	APNP_CONFIG_NONEXIST,
	APNP_INPUT_TYPE_ERROR,
	APNP_CONFIG_REACH_MAX_NUM,
	APNP_FILE_OPEN_FAIL,
	AC_MANAGE_CONFIG_RELEVANCE,
	AC_MANAGE_SERVICE_STOP_FAIL,
	AC_MANAGE_CONFIG_FLUSH_FAIL,
	AC_MANAGE_CONFIG_SAVE_FAIL,
	AC_MANAGE_CONFIG_EXEC_FAIL,
};



#define APNP_ERR_SUCCESS		(0)
#define	APNP_ERR_OPEN_FAIL		(-1)
#define	APNP_ERR_BAD_ADDRESS	(-2)
#define	APNP_ERR_ADDR_MATCH		(-3)
#define APNP_ERR_BAD_PARSE		(-4)
#define APNP_ERR_MALLOC_FAIL	(-5)
#define APNP_ERR_BAD_PRE		(-6)
#define	APNP_ERR_BAD_INPUT		(-7)
#define	APNP_ERR_PACKET_PROXY	(-8)
#define	APNP_ERR_BAD_TYPE		(-9)
#define APNP_ERR_ACCEPT			(-10)





typedef struct apnp_session_s			apnp_session;

typedef int (*apnp_callback) (int, apnp_session *, 
									int, void *, void *);


typedef int (*task_method) (void *, size_t, 
								void **, size_t *);

#define MAX_PAYLOAD 1024

struct apnp_session_s {
	u_long	sessid; 

	u_long message_type;
	u_long	flags;

	u_char 	*local;
	size_t	  local_len;

	u_char 	*remote;
	size_t	  remote_len;

	apnp_callback callback;
	//task_method		Method_s;

	/** copy of system errno */
	int             s_errno;
	/** copy of library errno */
	int             s_manage_errno;   

};




#endif
