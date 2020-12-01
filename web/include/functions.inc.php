<?php
require "include/postgresql.conf.inc.php";
define("HASHING_ALGO", "sha256");
define("DEBUG", false);

function is_visitor_id(string $str): bool
{
    //TODO
    return true;
}

function login_user($referrer = "index.php"): void
{
    if (isset($_POST["id"]) && isset($_POST["password"])) {
        $id = $_POST["id"];
        $password = $_POST["password"];
        $password_hash = hash(HASHING_ALGO, $password);
        if (is_visitor_id($id) === true) {
            $query = "SELECT visitor.visitor_id, visitor.visitor_password_hash
                         FROM visitor 
                         WHERE visitor.visitor_id = '" . $id . "'
                         AND visitor.visitor_password_hash = '" . $password_hash . "';";
            if (DEBUG) {
                var_dump($query);
            }
            $result = make_query($query);
            $nbrows = pg_numrows($result);
            if ($nbrows == 1) {
                set_session_user($id);
                header("location:".$referrer);
            }
        } else {
        }
    }
}

function set_session_user(string $id)
{
    unset($_SESSION["user"]);
    $_SESSION["user"] = $id;
}
