<?php
//define("CONN_INFO", "postgres://vpyfxlkr:Sjq64v_di28D9xpqzF4Y_XIiVkv6Upza@kandula.db.elephantsql.com:5432/vpyfxlkr ");
define("CONN_INFO", "host=kandula.db.elephantsql.com port=5432 dbname=vpyfxlkr user=vpyfxlkr password=Sjq64v_di28D9xpqzF4Y_XIiVkv6Upza");

/**
 * 
 *
 * @return void
 */
function connect_db(string $params = CONN_INFO)
{
    $connected = pg_connect($params);
    //var_dump($connected);
    return $connected;
}

function make_query(string $query)
{
    $conn = connect_db();
    if(!$conn){
        echo "Une erreur s'est produite.\n";
        exit;
    }
    $result = pg_query($conn, $query);
    if(!$result){
        echo "Une erre s'est produite.\n";
        exit;
    }
    return $result;
}