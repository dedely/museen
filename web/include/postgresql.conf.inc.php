<?php
define("ONLINE_CONN", "host=kandula.db.elephantsql.com port=5432 dbname=vpyfxlkr user=vpyfxlkr password=Sjq64v_di28D9xpqzF4Y_XIiVkv6Upza");
define("LOCAL_CONN", "host=localhost port=5432 dbname=postgres user=postgres password=chess");
define("BACKUP_DB", "host=postgresql-adelabbas.alwaysdata.net port=5432 dbname=adelabbas_museen user=adelabbas password=museen-artwork-secrets-exposed");
define("DEBUGDB", false);

function connect_db(string $params = BACKUP_DB)
{
    $connected = pg_connect($params);
    return $connected;
}

function make_query(string $query)
{
    $params = (DEBUGDB) ? LOCAL_CONN : BACKUP_DB;
    $conn = connect_db($params);
    if (!$conn) {
        echo "Une erreur s'est produite.\n";
        exit;
    }
    $result = pg_query($conn, $query);
    if (!$result) {
        echo "Une erreur s'est produite.\n";
        exit;
    }
    pg_close($conn);
    return $result;
}