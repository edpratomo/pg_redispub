DROP FUNCTION IF EXISTS redispub(TEXT,TEXT);

CREATE FUNCTION redispub(TEXT,TEXT)
RETURNS boolean
AS 'pg_redispub', 'redispub'
LANGUAGE C STABLE STRICT;
