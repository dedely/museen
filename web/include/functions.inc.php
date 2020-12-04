<?php
require_once "include/postgresql.conf.inc.php";
define("HASHING_ALGO", "sha256");
define("DEBUG", false);
define("AUTHKEY_LENGTH", 15);

/**
 * This function handles user signin
 *
 * @param string $referrer the page the user was on before accessing the signin form
 * @return void
 */
function signin(string $referrer = "index.php"): void
{
    if (isset($_POST["id"], $_POST["password"])) {
        $id = $_POST["id"];
        $password = $_POST["password"];
        $password_hash = hash(HASHING_ALGO, $password);
        if (is_visitor_id($id) === true) {
            $query = "SELECT visitor.visitor_id, visitor.visitor_password_hash
                         FROM visitor 
                         WHERE visitor.visitor_id = '" . $id . "'
                         AND visitor.visitor_password_hash = '" . $password_hash . "';";
            $result = make_query($query);
            $nbrows = pg_numrows($result);
            if ($nbrows == 1) {
                set_session_user($id);
                header("location:" . $referrer);
            }
        } else {
            //TODO
        }
    }
}

/**
 * This function handles user signup
 *
 * @return void
 */
function signup(): void
{
    if (isset($_POST["id"], $_POST["firstName"], $_POST["lastName"], $_POST["birthdate"], $_POST["password"])) {
        $id = $_POST["id"];
        $firstName = $_POST["firstName"];
        $lastName = $_POST["lastName"];
        $birthDate = $_POST["birthdate"];
        $password = $_POST["password"];

        $password_hash = hash("sha256", $password);
        $authkey = generate_random_string(AUTHKEY_LENGTH);
        $authkey_hash = hash("sha256", $authkey);

        if (is_visitor_id($id)) {
            $query = "INSERT INTO public.visitor (visitor_id, visitor_first_name, visitor_last_name, visitor_birthdate, visitor_password_hash,
            visitor_authkey_hash) VALUES ('" . $id . "', '" . $firstName . "', '" . $lastName . "', '" . $birthDate . "', '" . $password_hash . "', '" . $authkey_hash . "');";
            $result = make_query($query);
            $status = pg_result_status($result);
            if ($status == PGSQL_COMMAND_OK) {
                set_session_user($id);
                //Save the authkey to be able to display it later to the user.
                set_session_authkey($authkey);
                header("location:preferences.php");
            }
        }
        //TODO
    }
}

/**
 * Queries the database to get an array of the existing artistic movements
 *
 * @return array
 */
function get_artistic_movements(): array
{
    $query = "SELECT artistic_movement_name FROM artistic_movement;";
    $movements = array();
    if (($result = make_query($query)) != FALSE) {
        $rows = pg_num_rows($result);
        for ($i = 0; $i < $rows; $i++) {
            $movements = pg_fetch_row($result, $i);
        }
    }
    return $movements;
}

/*----------------Display----------------*/

function show_movements_form(array $movements = NULL): void
{
    if(is_null($movements)){
        $movements = get_artistic_movements();
    }
    $cpt = count($movements);
    //TODO
    
}

/*----------------$_SESSION setters----------------*/

function set_session_user(string $id): void
{
    unset($_SESSION["user"]);
    $_SESSION["user"] = $id;
}

function set_session_authkey(string $authkey): void
{
    unset($_SESSION["authkey"]);
    $_SESSION["authkey"] = $authkey;
}

/*----------------Utility----------------*/

function custom_stylesheet(string $stylesheet = NULL): void
{
    if (isset($stylesheet)) {
        echo "<link rel=\"stylesheet\" href=\"" . $stylesheet . "\" / >\n";
    }
}

/**
 * Generates a random string using a naive approach, i.e. pick a random index of the charset.
 *
 * @param integer $length the desired lenght of the result string
 * @return string
 */
function generate_random_string(int $length = 10): string
{
    $charset = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $str = '';

    for ($i = 0; $i < $length; $i++) {
        $index = rand(0, strlen($charset) - 1);
        $str .= $charset[$index];
    }

    return $str;
}

/**
 * Checks if the length of a given string is smaller than the provided max_length
 *
 * @param string $str
 * @param integer $max_length
 * @return boolean
 */
function is_smaller_str(string $str, int $max_length = 64): bool
{
    return strlen($str) <= $max_length;
}

        if (is_visitor_id($id)) {
            $query = "INSERT INTO public.visitor (visitor_id, visitor_first_name, visitor_last_name, visitor_birthdate, visitor_password_hash,
            visitor_authkey_hash) VALUES ('" . $id . "', '" . $firstName . "', '" . $lastName . "', '" . $birthDate . "', '" . $password_hash . "', '" . $authkey_hash . "');";
            $result = make_query($query);
            $status = pg_result_status($result);
            if ($status == PGSQL_COMMAND_OK) {
                set_session_user($id);
                //Save the authkey to be able to display it later to the user.
                set_session_authkey($authkey);
                header("location:preferences.php");
            }
        }
    }
}