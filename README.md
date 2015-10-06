
# pg_redispub

    redispub(TEXT, TEXT)

sends [PUBLISH channel message](http://redis.io/commands/publish) to redis instance from postgresql.

## Installation

Requires [libhiredis-devel](https://github.com/redis/hiredis) package.

    $ make
    $ sudo make install

Use psql to load the extension:

    create extension if not exists "pg_redispub";
    select * from pg_extension;
    select * from redispub('seat', '100');

## SELinux Troubleshooting

If you see this warning: "WARNING:  failed to connect to redis: permission denied" then check audit.log, e.g. /var/log/audit/audit.log:

    avc:  denied  { name_connect } for  pid=4773 comm="postgres" dest=6379 scontext=system_u:system_r:postgresql_t:s0 tcontext=system_u:object_r:redis_port_t:s0 tclass=tcp_socket permissive=0

Use
[audit2allow](https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/6/html/Security-Enhanced_Linux/sect-Security-Enhanced_Linux-Fixing_Problems-Allowing_Access_audit2allow.html)
to fix this issue.

    # audit2allow -w -a

    type=AVC msg=audit(1444121144.166:604): avc:  denied  { name_connect } for  pid=4773 comm="postgres" dest=6379 scontext=system_u:system_r:postgresql_t:s0 tcontext=system_u:object_r:redis_port_t:s0 tclass=tcp_socket permissive=0
        Was caused by:
        type enforcement (TE) allow rule.

        can use audit2allow to generate a loadable module to allow this access.

    # audit2allow -a -M postgresredispol
    ******************** IMPORTANT ***********************
    To make this policy package active, execute:

    semodule -i postgresredispol.pp

    # semodule -i postgresredispol.pp

## License

pg_redispub is (c) Edwin Pratomo, 2015; it is distributed under the terms of the MIT License, see LICENSE for details.
