
list(APPEND SOURCE
    src/api/api_lib.c
    src/api/api_msg.c
    src/api/err.c
    src/api/netbuf.c
    src/api/netdb.c
    src/api/netifapi.c
    src/api/sockets.c
    src/api/tcpip.c
    src/core/def.c
    src/core/dhcp.c
    src/core/dns.c
    src/core/init.c
    src/core/mem.c
    src/core/memp.c
    src/core/netif.c
    src/core/pbuf.c
    src/core/raw.c
    src/core/stats.c
    src/core/sys.c
    src/core/tcp_in.c
    src/core/tcp_out.c
    src/core/tcp.c
    src/core/timers.c
    src/core/udp.c
    src/core/ipv4/autoip.c
    src/core/ipv4/icmp.c
    src/core/ipv4/igmp.c
    src/core/ipv4/inet_chksum.c
    src/core/ipv4/inet.c
    src/core/ipv4/ip.c
    src/core/ipv4/ip_addr.c
    src/core/ipv4/ip_frag.c
    #src/core/ipv6/icmp6.c
    #src/core/ipv6/inet6.c
    #src/core/ipv6/ip6_addr.c
    #src/core/ipv6/ip6.c
    src/core/snmp/asn1_dec.c
    src/core/snmp/asn1_enc.c
    src/core/snmp/mib_structs.c
    src/core/snmp/mib2.c
    src/core/snmp/msg_in.c
    src/core/snmp/msg_out.c)

add_library(lwipcore ${SOURCE})

target_include_directories(lwipcore
    PRIVATE ${LWIP_INCLUDE_DIRS}
    PUBLIC src/include/ipv4)

add_dependencies(lwipcore bugcodes xdk)

if(NOT MSVC)
    target_compile_options(lwipcore PRIVATE -fno-builtin-malloc -fno-builtin-free)
endif()
