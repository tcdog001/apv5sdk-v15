#ifndef _APNP_TRANSPORT_H
#define _APNP_TRANSPORT_H

#define APNP_NETLINK_DOMAIN	"netlink"

#define APNP_TCP_DOMAIN	"tcp"
#define APNP_UDP_DOMAIN "udp"


typedef struct apnp_transport_s apnp_transport;
typedef struct apnp_tdomain_s   apnp_tdomain;




struct apnp_transport_s {

	int 			sock;

	u_long		flags;

	/** transport addr**/
	void			*addr;
	
	unsigned long 	msgMaxSize;
		
	int (*f_recv)		(struct apnp_transport_s *, void *,
						size_t, void **, size_t *);	
	int (*f_send)		(struct apnp_transport_s *, void *,
						size_t, void *, size_t );
	int (*f_close)		(struct apnp_transport_s *);

	/*  This callback is only necessary for stream-oriented transports.  */
	int (*f_accept)	(struct apnp_transport_s *);

};


struct apnp_tdomain_s {
    char *name;

	apnp_transport *(*f_create_from_chunk) (u_long);

	struct apnp_tdomain_s *next;
};


#endif
