<?php
define("ONLINE_CONN", "host=kandula.db.elephantsql.com port=5432 dbname=vpyfxlkr user=vpyfxlkr password=Sjq64v_di28D9xpqzF4Y_XIiVkv6Upza");
define("LOCAL_CONN", "host=localhost port=5432 dbname=postgres user=postgres password=chess");
define("DEBUGDB", false);

function connect_db(string $params = ONLINE_CONN)
{
    $connected = pg_connect($params);
    return $connected;
}

function make_query(string $query)
{
    $params = (DEBUGDB) ? LOCAL_CONN : ONLINE_CONN;
    $conn = connect_db($params);
    if (!$conn) {
        echo "Une erreur s'est produite.\n";
        //TODO
        exit;
    }
    $result = pg_query($conn, $query);
    if (!$result) {
        echo "Une erreur s'est produite.\n";
        //TODO
        exit;
    }
    return $result;
}